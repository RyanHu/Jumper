//
//  GameLayer.cpp
//  ComicJumper2
//
//  Created by Bai Ryan on 13-6-29.
//
//

#include "GameLayer.h"
#include "config.h"
#include "GLES-Render.h"
#include "GameScene.h"
#include "PlayerSprite.h"
#include "Controller.h"
#include "Gold.h"
#include "SimpleAudioEngine.h"
#include "GameContactListener.h"
#include "Balloon.h"
#include "UILayer.h"

#include "Cloud.h"

#define PTM_RATIO 32.0f
//#define ENABLE_DEBUGDRAW


using namespace cocos2d;
using namespace CocosDenshion;


GameLayer::GameLayer()
{
    CCLOG("GameLayer constructor~");
    m_offSet = CCPointZero;
    m_farthestDistance = 0;
    m_nexGoldStandardRange = 120;
    m_topGoldPosition = 0;

    
//    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bell.mp3", true);
}

GameLayer::~GameLayer()
{
    delete m_world;
    m_world = NULL;
    
#ifdef ENABLE_DEBUGDRAW
    delete m_debugDraw;
    m_debugDraw = NULL;
#endif
    
    delete m_cloudCache;
    delete m_goldCache;
    

}

bool GameLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    initPhysics();
    addController();
    addPlayer();
    addGoldSpriteBatch();
    addCloudsBatch();
    addBalloon();
    scheduleUpdate();
    
    return true;
}


void GameLayer::update(float dt)
{
    int velocityIterations = 8;
    int positionIterations = 1;
    
    
    //bool isTouched = m_controller->getIsTouched();
    /*if(isTouched == true)
     {
     castForceOnPlayer();
     }*/
    
    GameScene *gameScene = GameScene::sharedGameScene();
    if (gameScene == NULL) {
        return;
    }
    
    if(GameScene::sharedGameScene()->getGameStatus() == GameStatusPaused)
    {
        return;
    }
    
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    m_world->Step(dt, velocityIterations, positionIterations);
    
    PlayerSprite *player = (PlayerSprite *) this->getChildByTag(kNodeTagPlayer);
    
    //Iterate over the bodies in the physics world
    for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            //Synchronize the AtlasSprites position and rotation with the corresponding body
            CCSprite* myActor = (CCSprite*)b->GetUserData();
            myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            //    myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
            if(player->getBody() == b)
            {
                
                b->SetTransform(b->GetPosition(), 0);
            }
        }
    }
   
    if(GameScene::sharedGameScene()->getGameStatus() == GameStatusOn)
    {
        b2Body *playerBody = player->getBody();
        CCPoint velocity = m_controller->getM_velocity();
        //use no lineardamping, mannually cast reverse force by player's speed
        b2Vec2 playerVelocity = playerBody->GetLinearVelocityFromLocalPoint(b2Vec2(0,0));
    
        if(playerVelocity.x >0 && player->isFlipX() == false)
        {
            player->setFlipX(true);
        }
        else if(playerVelocity.x <0 && player->isFlipX() == true)
        {
            player->setFlipX(false);
        }
        
        b2Vec2 dampingForce = b2Vec2(-playerVelocity.x,0);
        //  CCLog("playervelocity x :%f",playerVelocity.x);
        //  playerBody->ApplyForce(b2Vec2(velocity.x,0), playerBody->GetPosition());
        float ratio = 3.0f;
        b2Vec2 force = b2Vec2(velocity.x*ratio + dampingForce.x*1.5f,0);
        playerBody->ApplyForce(force, playerBody->GetPosition());

        checkSpriteHit();
        refreshSpriteSpawn();
        
        if(player->getPlayerStatus() == kPlayerStatusFloating)
        {
            float32 mass = playerBody->GetMass();
            playerBody->ApplyForce(b2Vec2(0.0f,mass*12.0f), playerBody->GetPosition());
            
            refreshBackgroundPosition();
            refreshBalloonSpawn();
            
            if(checkFloatingBalloonTimeOut() == true)
            {
                floatingPlayerEnd();
            }
         /*   PlayerSprite *player = (PlayerSprite *) this->getChildByTag(kNodeTagPlayer);
            b2Body *playerBody = player->getBody();
            playerBody->SetLinearVelocity(b2Vec2(0,2.0f));
            */
        }
        else if(player->getPlayerStatus() == kPlayerStatusNormal)
        {
            refreshBackgroundPosition();
            refreshBalloonSpawn();
            if(checkPlayerLoseLife() == true)
            {
                player->setPlayerStatus(kPlayerStatusLosingLife);
            }
            else
            {
                
            }
        }
        else if(player->getPlayerStatus() == kPlayerStatusLosingLife)
        {
            if(checkPlayerDead() == true)
            {
                CCLog("player lose one life");
                
                int lives = player->getPlayerLives();
                lives --;
                player->setPlayerLives(lives);
                GameScene::sharedGameScene()->defaultUILayer()->lostLifeRefreshIcon(lives);
                if(player->getPlayerLives() <= 0)
                {
                    CCLog("setting player dead");
                    player->setPlayerStatus(kPlayerStatusDead);
                    GameScene::sharedGameScene()->endGame();
                }
                else
                {
                    player->setPlayerStatus(kPlayerStatusFloating);
                    floatingPlayerUp();
                }
            }
        }
        else if(player->getPlayerStatus() == kPlayerStatusDead)
        {
            checkCollision();
        }
        
    }
    
    
    if(player->getPlayerStatus() == kPlayerStatusDead)
    {
        checkCollision();
    }
    else if(player->getPlayerStatus() == kPlayerStatusLosingLife)
    {
        checkCollision();
    }
    
}

void GameLayer::floatingPlayerUp()
{
    PlayerSprite *player = (PlayerSprite *) this->getChildByTag(kNodeTagPlayer);
    b2Body *playerBody = player->getBody();
    playerBody->SetLinearVelocity(b2Vec2(0,2.0f));
    
    BalloonWithTail *balloon = BalloonWithTail::create();
    player->addChild(balloon,1,kNodeTagBalloonWithTail);
    
    CCSize playerSize= player->getTextureRect().size;
    balloon->setPosition(ccp(playerSize.width/2,playerSize.height * 1.3f));
    
    
    m_farthestDistance = player->getPosition().y;
    m_floatBalloonStartY = player->getPosition().y;
}

bool GameLayer::checkFloatingBalloonTimeOut()
{
    PlayerSprite *player = (PlayerSprite *) this->getChildByTag(kNodeTagPlayer);
    if(player->getPlayerStatus() == kPlayerStatusFloating)
    {
        if(player->getPosition().y > kBalloonFloatingDistanceMax + m_floatBalloonStartY)
        {
            player->removeChildByTag(kNodeTagBalloonWithTail, true);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

void GameLayer::floatingPlayerEnd()
{
    PlayerSprite *player = (PlayerSprite *) this->getChildByTag(kNodeTagPlayer);
    player->setPlayerStatus(kPlayerStatusNormal);
    
}


void GameLayer::initPhysics()
{
    addWorld();
    addBoundary();
}

void GameLayer::addWorld()
{
    b2Vec2 gravity;
    gravity.Set(0, -kWorldGravity);
    m_world = new b2World(gravity);
    m_world->SetAllowSleeping(true);
    m_world->SetContinuousPhysics(true);
    
#ifdef ENABLE_DEBUGDRAW
    m_debugDraw = new GLESDebugDraw(PTM_RATIO);
    m_world->SetDebugDraw(m_debugDraw);
    
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    flags += b2Draw::e_aabbBit;
    flags += b2Draw::e_pairBit;
    flags += b2Draw::e_centerOfMassBit;
    m_debugDraw->SetFlags(flags);
#endif

    m_contactListener = new GameContactListener();
    m_world->SetContactListener(m_contactListener);

}

void GameLayer::addGroundBoundaryByRestitution(float restitution)
{
    m_groundBody->DestroyFixture(m_groundBody->GetFixtureList());
    CCSprite *spriteGround = (CCSprite *)getChildByTag(kNodeTagGround);
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    b2EdgeShape groundBox;
    groundBox.Set(b2Vec2(0,0), b2Vec2(screenSize.width/PTM_RATIO,0));
    b2Fixture *fixture = m_groundBody->CreateFixture(&groundBox,0);
    fixture->SetUserData(spriteGround);
    fixture->SetRestitution(restitution);
}

void GameLayer::addBoundary()
{
    
    ///ground boundary
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    float groundYPos = 0;
    if(screenSize.height == 480)
    {
        groundYPos = kGroundBodyYAxisDefaultIphone4;
    }
    else
    {
        groundYPos = kGroundBodyYAxisDefaultIphone5;
    }
    
    CCSprite *spriteGround = CCSprite::create("ground.png");
    spriteGround->setTag(kNodeTagGround);
    spriteGround->setScaleX(2.0f);
    this->addChild(spriteGround);
    spriteGround->setAnchorPoint(ccp(0,0.5f));
    spriteGround->setPosition(ccp(0,groundYPos));
    
    
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, groundYPos/PTM_RATIO);
    b2Body *groundBody = m_world->CreateBody(&groundBodyDef);
    m_groundBody = groundBody;
    m_groundBody->SetUserData(spriteGround);
    
    b2EdgeShape groundBox;
    groundBox.Set(b2Vec2(0,0), b2Vec2(screenSize.width/PTM_RATIO,0));
    b2Fixture *fixture = groundBody->CreateFixture(&groundBox,0);
    fixture->SetUserData(spriteGround);
    fixture->SetRestitution(1.0f);
    
    ///side boundary
    
    b2BodyDef sidesBodyDef;
    sidesBodyDef.position.Set(0, 0);
    b2Body *sidesBody = m_world->CreateBody(&sidesBodyDef);
    m_sideBody = sidesBody;
    
    b2EdgeShape sidesBox;
    // left
    sidesBox.Set(b2Vec2(0,0), b2Vec2(0,screenSize.height/PTM_RATIO));
    sidesBody->CreateFixture(&sidesBox,0);
    
    // right
    sidesBox.Set(b2Vec2(screenSize.width/PTM_RATIO,0), b2Vec2(screenSize.width/PTM_RATIO-0.5f,screenSize.height/PTM_RATIO));
    sidesBody->CreateFixture(&sidesBox,0);    
    
}

void GameLayer::addPlayer()
{
    PlayerSprite *player = PlayerSprite::createWithWorld(m_world);
    player->setPosition(ccp(260, 300));
    this->addChild(player,2,kNodeTagPlayer);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    CCPoint pos = player->getPosition();
    bodyDef.position.Set(pos.x/PTM_RATIO, pos.y/PTM_RATIO);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.1f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.05f;
    
    CCSize playerSize = player->getTextureRect().size;
    playerSize = CCSizeMake(playerSize.width*player->getScale(), playerSize.height*player->getScale());
    b2PolygonShape *shape = new b2PolygonShape();
    shape->SetAsBox(playerSize.width/PTM_RATIO/2, playerSize.height/PTM_RATIO/2);
    fixtureDef.shape = shape;
    bodyDef.userData = player;
    b2Body *body = m_world->CreateBody(&bodyDef);
    body->CreateFixture(&fixtureDef);
    player->setBody(body);
}

void GameLayer::addBalloon()
{
    m_theBalloon = Balloon::create();
    //m_theBalloon->setVisible(false);
    int yPos = arc4random()%500 - 1000 + kBalloonNextDistance;
    m_theBalloon->setPosition(ccp(20,yPos));
    m_topBalloonPosition = yPos;
    this->addChild(m_theBalloon);
    
    CCActionInterval *moveRight = CCMoveTo::create(2.0f, ccp(320,yPos));
    CCEaseInOut *moveRightAction = CCEaseInOut::create(moveRight,1.0f);
    
    CCActionInterval *moveLeft = CCMoveTo::create(2.0f, ccp(20,yPos));
    CCEaseInOut *moveLeftAction = CCEaseInOut::create(moveLeft, 1.0f);
    
    CCFiniteTimeAction *seq = CCSequence::create(moveRightAction,moveLeftAction,NULL);
    CCActionInterval *repeat = CCRepeatForever::create((CCActionInterval *)seq);
    m_theBalloon->runAction(repeat);

    
 //   CCSprite *sprite = CCSprite::create("blank.png");
 //   sprite->setTextureRect(<#const cocos2d::CCRect &rect#>);
 //   sprite->setColor(ccc3(200, 200, 100));
}


void GameLayer::addCloudsBatch()
{
    m_cloudCache = new CloudCache();
    addChild(m_cloudCache,0,kNodeTagCloudCache);
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    float firstCloudPositionY = screenSize.height * 2;
    
    CCSprite *cloud = m_cloudCache->pickARandomNextCloud();
    
    int firstCloudPositionX = arc4random()%200 + 60;
    
    cloud->setPosition(ccp(firstCloudPositionX,firstCloudPositionY));
    cloud->setVisible(true);
    
    m_topCloudPosition = firstCloudPositionY;
    
//    Cloud *cloud = m_cloudCache->getNextCloudByType(1);
//    cloud->setPosition(ccp(160, 400));
//    cloud->setVisible(true);
}

void GameLayer::addGoldSpriteBatch()
{
    if(m_goldCache == NULL)
    {
        m_goldCache = new GoldCache();
        addChild(m_goldCache,1,kNodeTagGoldCache);
    }
    
    //place gold on the screen first
    float firstGoldYPos = 300;
 //   int rand= arc4random()%50-25;
 //   firstGoldYPos += rand;
    float firstGoldXPos = 20.0f + arc4random()%300;
    CCPoint firstGoldPos= ccp(firstGoldXPos,firstGoldYPos);
    Gold *gold = m_goldCache->getNextGold();
    gold->setPosition(firstGoldPos);
    gold->setVisible(true);
    CCLog("placing gold %f %f",gold->getPosition().x,gold->getPosition().y);
    
    
    m_farthestDistance = kPlaySpriteYAxisDefault;
    m_topGoldPosition = firstGoldYPos;
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    while (m_topGoldPosition < m_farthestDistance + screenSize.height) {
        Gold *gold = m_goldCache->getNextGold();
        
        int range = m_nexGoldStandardRange*2/3;
        int rangeOffset = arc4random()%range - range;
        float nextGoldYPos = m_topGoldPosition + m_nexGoldStandardRange + rangeOffset;
        float nextGoldXPos = 20.0f + arc4random()%300;
        
        gold->setPosition(ccp(nextGoldXPos,nextGoldYPos));
        CCLog("placing gold %f %f",gold->getPosition().x,gold->getPosition().y);
        gold->setVisible(true);
        
        m_topGoldPosition = nextGoldYPos;
    }

    
}

void GameLayer::addController()
{
    m_controller = new Controller();
    this->addChild(m_controller);
}


void GameLayer::draw()
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    
    m_world->DrawDebugData();
    
    kmGLPopMatrix();
}

void GameLayer::refreshBalloonSpawn()
{
   // PlayerSprite *player = (PlayerSprite*)this->getChildByTag(kNodeTagPlayer);
   // if(player->getPosition().y > m_topBalloonPosition)
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    if(m_farthestDistance > m_topBalloonPosition + screenSize.width*2.0f)
    {
        m_theBalloon->setVisible(true);
        
        m_theBalloon->stopAllActions();
        float yPos = m_topBalloonPosition + arc4random()%kBalloonYPosOffSetRange- kBalloonYPosOffSetRange*2 + kBalloonNextDistance;
        m_theBalloon->setPosition(ccp(20,yPos));
        m_topBalloonPosition = yPos;
    
        CCActionInterval *moveRight = CCMoveTo::create(2.0f, ccp(320,yPos));
        CCEaseInOut *moveRightAction = CCEaseInOut::create(moveRight,1.0f);
        
        CCActionInterval *moveLeft = CCMoveTo::create(2.0f, ccp(20,yPos));
        CCEaseInOut *moveLeftAction = CCEaseInOut::create(moveLeft, 1.0f);
        
        CCFiniteTimeAction *seq = CCSequence::create(moveRightAction,moveLeftAction,NULL);
        CCActionInterval *repeat = CCRepeatForever::create((CCActionInterval *)seq);
        m_theBalloon->runAction(repeat);
    
    }

}

void GameLayer::refreshBackgroundPosition()
{
  //  if (GameScene::sharedGameScene()->getGameStatus() != GameStatusOn) {
  //      return;
  //  }
    
    PlayerSprite *player = (PlayerSprite*)this->getChildByTag(kNodeTagPlayer);
    if(player != NULL)
    {
        CCPoint offset = m_offSet;
        
        b2Body *playerBody = player->getBody();
        
        CCPoint pos= ccp(playerBody->GetPosition().x*PTM_RATIO,playerBody->GetPosition().y*PTM_RATIO);
        CCPoint spritePos = player->getPosition();
        
        CCPoint posOnScreen = ccp(pos.x+this->getPosition().x, pos.y + this->getPosition().y);
        //  CCLog("y axis body pos:%f , spritePos: %f",pos.y,spritePos.y);
        
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        float upperLimmit = 0;
        if(screenSize.height == 480)
        {
            upperLimmit = kMoveScreenUpperLimit;
        }
        else
        {
            upperLimmit = kMoveScreenUpperLimitForIpone5;
        }
        
        if(posOnScreen.y >= upperLimmit)
        {
            b2Vec2 speed = playerBody->GetLinearVelocityFromLocalPoint(b2Vec2(0,0));
            if(speed.y >=0)
            {
                offset.y = upperLimmit - pos.y;
            }
        }
        
        float moveScreenDownLimmit = 0;
        if(screenSize.height == 480)
        {
           moveScreenDownLimmit = kMoveScreenDownLimmitForIphone4;
        }
        else
        {
            moveScreenDownLimmit = kMoveScreenDownLimmitForIphone5;
        }
        
        if(posOnScreen.y <moveScreenDownLimmit)
        {
            
            /*
            b2Vec2 speed = playerBody->GetLinearVelocityFromLocalPoint(b2Vec2(0,0));
            if(speed.y <=0)
            {
                offset.y = upperLimmit - pos.y;
            }*/
            if(player->getPlayerStatus() == kPlayerStatusNormal)
            {
                offset.y = moveScreenDownLimmit - pos.y;
            }
            
        }
        if(offset.y != m_offSet.y)
        {
            m_offSet = offset;
            this->setPosition(m_offSet);
        }
        
        //    BackgroundLayer *backgroundLayer = GameScene::sharedGameScene()->defaultBackgroundLayer();
        //    backgroundLayer->setPosition(m_offSet);
        
        //    b2Vec2 groundPos = b2Vec2(pos.x/PTM_RATIO,pos.y/PTM_RATIO);
        //    m_groundBody->SetTransform(groundPos, m_groundBody->GetAngle());
        b2Vec2 sidesPos = b2Vec2(0,(spritePos.y - 50)/PTM_RATIO);
        m_sideBody->SetTransform(sidesPos, 0);
    }
}

void GameLayer::resetBackgroundPosition()
{
    this->setPosition(CCPointZero);
    
    b2Vec2 sidesPos = b2Vec2(0,0);
    m_sideBody->SetTransform(sidesPos, 0);

}


void GameLayer::refreshSpriteSpawn()
{
    float playerPos = 0;
    PlayerSprite *player = (PlayerSprite*)this->getChildByTag(kNodeTagPlayer);
    if(player != NULL)
    {
        playerPos = player->getPositionY();
    }
    
    if(playerPos > m_farthestDistance)
    {
        m_farthestDistance = playerPos;
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        while (m_topGoldPosition < m_farthestDistance + screenSize.height) {
            Gold *gold = m_goldCache->getNextGold();
            
            int range = m_nexGoldStandardRange*2/3;
            int rangeOffset = arc4random()%range - range;
            float nextGoldYPos = m_topGoldPosition + m_nexGoldStandardRange + rangeOffset;
            float nextGoldXPos = 20.0f + arc4random()%300;
            
            gold->setPosition(ccp(nextGoldXPos,nextGoldYPos));
            gold->setVisible(true);
            
            m_topGoldPosition = nextGoldYPos;
        }
        
        while (m_topCloudPosition < m_farthestDistance + screenSize.height) {
            CCSprite *cloud = m_cloudCache->pickARandomNextCloud();
            
            float nextCloudYPos = m_topCloudPosition + arc4random()%400 + 100;
            float nextCloudXPos = arc4random()%300;
            
            cloud->setPosition(ccp(nextCloudXPos,nextCloudYPos));
            cloud->setVisible(true);
            
            m_topCloudPosition = nextCloudYPos;
        }
    }
}

void GameLayer::refreshCloudSpawn()
{
 //   CCSprite *
}

bool GameLayer::checkPlayerLoseLife()
{
    PlayerSprite *player = (PlayerSprite*)this->getChildByTag(kNodeTagPlayer);
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    float loseLifeCriticalYPos;
    
    ///player has more than 1 life
    if(player->getPlayerLives() > 1)
    {
        if(screenSize.height == 480)
        {
            loseLifeCriticalYPos = m_farthestDistance - KCheckLoseLifeDistanceFromTopToDownIphone4;
        }
        else
        {
            loseLifeCriticalYPos = m_farthestDistance -KCheckLoseLifeDistanceFromTopToDownIphone5;
        }
    }
    else //player has 1 life
    {
        loseLifeCriticalYPos =  m_farthestDistance - screenSize.width * 3.0f;
    }
    
    
    if(player->getPosition().y < loseLifeCriticalYPos)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

bool GameLayer::checkPlayerDead()
{
    PlayerSprite *player = (PlayerSprite*)this->getChildByTag(kNodeTagPlayer);
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    float moveScreenDownLimmit = 0;
    if(screenSize.height == 480)
    {
        moveScreenDownLimmit = kMoveScreenDownLimmitForIphone4;
    }
    else
    {
        moveScreenDownLimmit = kMoveScreenDownLimmitForIphone5;
    }
    
    float loseLifeCriticalYPos;
    if(player->getPlayerLives() > 1)
    {
        if(screenSize.height == 480)
        {
            loseLifeCriticalYPos = m_farthestDistance - KCheckLoseLifeDistanceFromTopToDownIphone4
                        - moveScreenDownLimmit ;
        }
        else
        {
            loseLifeCriticalYPos = m_farthestDistance -KCheckLoseLifeDistanceFromTopToDownIphone5
                        - moveScreenDownLimmit;
        }
    }
    else //player has 1 life
    {
        loseLifeCriticalYPos =  m_farthestDistance - kCheckLoseLifeDistanceForDead - moveScreenDownLimmit;
    }
    

    
    if(player->getPosition().y < loseLifeCriticalYPos )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void GameLayer::checkCollision()
{
    std::vector<b2Body *>bodies;
    std::vector<GameContact>::iterator pos;
    
    for (pos = m_contactListener->_contacts.begin(); pos != m_contactListener->_contacts.end(); pos++) {
        
        GameContact contact = *pos;
        
        if(contact.fixtureA == NULL || contact.fixtureB == NULL)
        {
            return;
        }
        b2Body *bodyA = contact.fixtureA->GetBody();
        b2Body *bodyB = contact.fixtureB->GetBody();
        
        if (bodyA->GetUserData() != NULL && bodyB->GetUserData() != NULL)
        {
            CCSprite *spriteA = (CCSprite *) bodyA->GetUserData();
            CCSprite *spriteB = (CCSprite *) bodyB->GetUserData();
            int iTagA = spriteA->getTag();
            int iTagB = spriteB->getTag();
            if ((iTagA == kNodeTagPlayer && iTagB == kNodeTagGround)
                || (iTagA == kNodeTagGround && iTagB == kNodeTagPlayer))
            {
                PlayerSprite *player = (PlayerSprite*)this->getChildByTag(kNodeTagPlayer);
        /*    b2Body *playerBody = player->getBody();
                b2Vec2 speed = playerBody->GetLinearVelocityFromLocalPoint(b2Vec2(0,0));
                CCLog("player hits ground with speed %f",speed.y);
              */
                if(player->getPlayerStatus() == kPlayerStatusDead)
                {
                 //   playerBody->SetActive(false);
                 //   player->setTexture(<#cocos2d::CCTexture2D *texture#>)
              //      player->setTexture(CCTextureCache::sharedTextureCache()->addImage("cat_die.png"));
                    player->setFlipX(false);
              //      player->setScaleX(1.0f);
                    player->setRotation(-90);
                }
                else if(player->getPlayerStatus() == kPlayerStatusLosingLife)
                {
                    //player losing life but hit ground, very rare to happen at small height
                    CCLog("setting player dead");
                    player->setPlayerStatus(kPlayerStatusDead);
                    b2Body *body = player->getBody();
                    body->SetLinearVelocity(b2Vec2(0,0.5f));
                    GameScene::sharedGameScene()->endGame();
                    player->setFlipX(false);
                    //      player->setScaleX(1.0f);
                    player->setRotation(-90);
                    return;
                }
            }
        }
    }
}


void GameLayer::eatBalloonAction()
{
    m_theBalloon->stopAllActions();
    m_theBalloon->setVisible(false);
    
    CCPoint layerPos = this->getPosition();
    CCPoint spritePos = m_theBalloon->getPosition();
    CCPoint pos = ccp(spritePos.x,spritePos.y + m_offSet.y);
    CCLog("layerPos : %f %f , m_offset y : %f",layerPos.x,layerPos.y,m_offSet.y);
    UILayer *uilayer = GameScene::sharedGameScene()->defaultUILayer();
    uilayer->addBallonAtPosition(pos);
}

void GameLayer::checkSpriteHit()
{
   // if(GameScene::sharedGameScene()->getGameStatus() != GameStatusOn)
   //     return;
    
    PlayerSprite *player = (PlayerSprite*)this->getChildByTag(kNodeTagPlayer);
    //   CCRect playerRect = player->getTextureRect()
    CCPoint centerPos = player->getPosition();
    CCSize playerSize = player->getTextureRect().size;
    CCPoint playerPos = ccp(centerPos.x-playerSize.width/2,centerPos.y-playerSize.height/2);
    CCRect playerRect = CCRectMake(playerPos.x, playerPos.y, playerSize.width, playerSize.height);
    
    
    if(m_theBalloon->isVisible() == true )
    {
     /*   CCPoint balloonPos = m_theBalloon->getPosition();
        CCSize balloonSize = m_theBalloon->getTextureRect().size;
        
        CCRect balloonRect = CCRectMake(balloonPos.x-balloonSize.width/2, balloonPos.y-balloonSize.height/2, balloonSize.width, balloonSize.height);
       */
        if(player->boundingBox().intersectsRect(m_theBalloon->boundingBox()))
        {
            eatBalloonAction();

            int lives = player->getPlayerLives();
            if(lives <= kBalloonIconMax)
            {
            //    m_theBalloon->setVisible(false);
                
                lives ++;
                player->setPlayerLives(lives);
            }
          //  else
            {
       /*         CCActionInterval *fade = CCTurnOffTiles::create(ccg(48,22), 1.0f);
                CCFiniteTimeAction *seq = CCSequence::create(fade,CCCallFunc::create(this, callfunc_selector(GameLayer::balloonExpansionTurnOffEnd)),NULL);
                m_theBalloon->runAction(fade);
        */
        }
            
            
            
            SimpleAudioEngine::sharedEngine()->playEffect("eat2.mp3");
            
            
            b2Body *playerBody = player->getBody();
            
            b2Vec2 speed = playerBody->GetLinearVelocityFromLocalPoint(b2Vec2(0,0));
            float v = speed.y;
            //   if(v <= 0)
            {
                
                
                float h = 500/PTM_RATIO;
                float g = kWorldGravity;
                float32 mass= playerBody->GetMass();
                
                float impulse = 2*(v*v/(2*g) - h)*mass*g/(-v - sqrt(2*h*g));
                if(impulse > 0)
                {
                    playerBody->ApplyLinearImpulse(b2Vec2(0,impulse), playerBody->GetPosition());
                    //       m_playerForceCastLock = ENABLED;
                }
                
                if(player->getPlayerStatus() == kPlayerStatusFloating)
                {
                    player->setPlayerStatus(kPlayerStatusNormal);
                    player->removeChildByTag(kNodeTagBalloonWithTail, true);
                }
            }
        }
    }
    
    
    GoldCache *goldCache = (GoldCache *)getChildByTag(kNodeTagGoldCache);
    CCArray *golds = goldCache->batch->getChildren();
    
    for (int i=0; i<golds->count(); i++) {
        CCObject *obj = golds->objectAtIndex(i);
        Gold *gold = (Gold *)obj;
        if(gold->isVisible() == true)
        {
            CCPoint goldPos = gold->getPosition();
            CCSize goldSize = gold->getTextureRect().size;
            
            CCRect goldRect = CCRectMake(goldPos.x-goldSize.width/2, goldPos.y-goldSize.height/2, goldSize.width, goldSize.height);
            
            if(player->boundingBox().intersectsRect(gold->boundingBox()))
            {
                CCLog("collides!!!!!!");
                
                int score = GameScene::sharedGameScene()->getScore();
                GameScene::sharedGameScene()->setScore(score+10);
                
                SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
                
                gold->setVisible(false);
                gold->setPosition(CCPointZero);
                
                b2Body *playerBody = player->getBody();
                
                b2Vec2 speed = playerBody->GetLinearVelocityFromLocalPoint(b2Vec2(0,0));
                float v = speed.y;
                //   if(v <= 0)
                {
                    float h = 300/PTM_RATIO;
                    float g = kWorldGravity;
                    float32 mass= playerBody->GetMass();
                    
                    float impulse = 2*(v*v/(2*g) - h)*mass*g/(-v - sqrt(2*h*g));
                    if(impulse > 0)
                    {
                        playerBody->ApplyLinearImpulse(b2Vec2(0,impulse), playerBody->GetPosition());
                 //       m_playerForceCastLock = ENABLED;
                    }
                    
                    if(player->getPlayerStatus() == kPlayerStatusFloating)
                    {
                        player->setPlayerStatus(kPlayerStatusNormal);
                        player->removeChildByTag(kNodeTagBalloonWithTail, true);
                    }
                }
            }
        }
    }
}

void GameLayer::startGameProcess()
{
    PlayerSprite *player = (PlayerSprite*)this->getChildByTag(kNodeTagPlayer);
    
   // CCActionInterval *fade = CCTurnOffTiles::create(ccg(48,22), 1.0f);
   // player->runAction(fade);
    
   // CCActionInterval *shaky3d = CCShaky3D::create(15, false, ccg(10,10), 4);
   // player->runAction(shaky3d);
    
    player->setPlayerLives(kPlayerDefaultLives);
    player->setPlayerStatus(kPlayerStatusNormal);
}

void GameLayer::endGameProcess()
{
    resetBackgroundPosition();
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    PlayerSprite *player = (PlayerSprite*)this->getChildByTag(kNodeTagPlayer);
    if(player->getPosition().y > screenSize.height * 2.0f)
    {
        b2Body *playerBody = player->getBody();
        b2Vec2 bodypos = b2Vec2(playerBody->GetPosition().x,screenSize.height*1.3f/PTM_RATIO);
        playerBody->SetTransform(bodypos, 0);
    }
    
    addGroundBoundaryByRestitution(0.0f);
}

void GameLayer::restartGameProcess()
{
    m_offSet = CCPointZero;
    m_farthestDistance = 0;
    m_nexGoldStandardRange = 120;
    m_topGoldPosition = 0;
    m_topBalloonPosition = 0;
    
    addGroundBoundaryByRestitution(1.0f);
    
    m_goldCache->resetAllGold();
    
    addGoldSpriteBatch();
    
    PlayerSprite *player = (PlayerSprite*)this->getChildByTag(kNodeTagPlayer);
    player->setPlayerLives(kPlayerDefaultLives);
    player->setPlayerStatus(kPlayerStatusNormal);
    player->setRotation(0);
    player->setPosition(ccp(160, 300));
    CCPoint pos = ccp(160,300);
    b2Body *playerBody = player->getBody();
    b2Vec2 bodyVec2 = b2Vec2(pos.x/PTM_RATIO,pos.y/PTM_RATIO);
    playerBody->SetTransform(bodyVec2, 0);
}

void GameLayer::pauseGameProcess()
{
  //  CCDirector::sharedDirector()->pause();
}

void GameLayer::resumeGameProcess()
{
  //  CCDirector::sharedDirector()->resume();
}

int GameLayer::getPlayerLives()
{
    PlayerSprite *player = (PlayerSprite*)this->getChildByTag(kNodeTagPlayer);
    if(player == NULL)
        return 0;
    return player->getPlayerLives();
}


