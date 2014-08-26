//
//  GameLayer.h
//  ComicJumper2
//
//  Created by Bai Ryan on 13-6-29.
//
//

#ifndef __ComicJumper2__GameLayer__
#define __ComicJumper2__GameLayer__

#include <iostream>
#include "cocos2d.h"
#include "Box2D.h"

class Controller;
class GoldCache;
class CloudCache;
class Balloon; ///single ballon, no need to use cache
class GameContactListener;

class GameLayer : public cocos2d::CCLayer
{
public:
    
    GameLayer();
    ~GameLayer();

    virtual bool init();
    CREATE_FUNC(GameLayer);
    void update(float dt);

    virtual void draw();

    void startGameProcess();
    void endGameProcess();
    void restartGameProcess();
    void pauseGameProcess();
    void resumeGameProcess();
    
    int getPlayerLives();

private:
    b2World *m_world;
    b2Draw *m_debugDraw;
    cocos2d::CCPoint m_offSet;
    b2Body *m_groundBody;
    b2Body *m_sideBody;

    Controller *m_controller;
    GoldCache *m_goldCache;
    CloudCache *m_cloudCache;
    Balloon *m_theBalloon;

    GameContactListener *m_contactListener;

    cocos2d::CCPoint m_sideBodyDefaultOffSetFromPlayer;

    void initPhysics();
    void addWorld();
    void addGroundBoundaryByRestitution(float restitution);
    void addBoundary();
    void addPlayer();
    void addController();
    void addGoldSpriteBatch();
    void addCloudsBatch();
    void addBalloon();

    void refreshSpriteSpawn();
    void checkSpriteHit();
    void checkCollision();
    bool checkPlayerLoseLife();
    bool checkPlayerDead();
    void refreshBackgroundPosition();

    void refreshCloudSpawn();
    void refreshBalloonSpawn();
    
    void eatBalloonAction();
    
    void floatingPlayerUp();
    void floatingPlayerEnd();
    bool checkFloatingBalloonTimeOut();

    void resetBackgroundPosition();

    float m_farthestDistance;
    float m_topGoldPosition;
    float m_topCloudPosition;
    float m_topBalloonPosition;
    int m_nexGoldStandardRange;
    int m_floatBalloonStartY;
    
};




#endif /* defined(__ComicJumper2__GameLayer__) */
