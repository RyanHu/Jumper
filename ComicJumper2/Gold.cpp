//
//  Gold.cpp
//  ComicCloudJumer
//
//  Created by Bai Ryan on 13-6-2.
//
//

#include "Gold.h"


using namespace cocos2d;



Gold::Gold()
{
    
}

Gold::~Gold()
{
    
}

bool Gold::init()
{
    if(!CCSprite::initWithFile("cake.png"))
    {
        return false;
    }
    
    return true;
    
}


GoldCache::GoldCache()
{
    nextInactiveGold = 0;
  //  CCSprite *textureSprite = CCSprite::create("hat.png");
  //  CCSpriteFrame *frame = CCSpriteFrame::createWithTexture(textureSprite->getTexture(), textureSprite->getTextureRect());
    batch = CCSpriteBatchNode::create("cake.png");
    addChild(batch);
    
    for (int i=0; i<50; i++) {
        Gold *gold = Gold::create();
        gold->setScale(0.3f);
        gold->setVisible(false);
        
        
        batch->addChild(gold);
    }
    
}

GoldCache::~GoldCache()
{
  //  batch->removeAllChildrenWithCleanup(true);
    delete batch;
    batch = NULL;
}

Gold * GoldCache::getNextGold()
{
    CCArray *golds = batch->getChildren();
    CCObject *obj = golds->objectAtIndex(nextInactiveGold);
    Gold *gold = (Gold *)obj;
    
    nextInactiveGold++;
    
    if(nextInactiveGold >= golds->count())
        
    {
        nextInactiveGold = 0;
    }
    return gold;
}

void GoldCache::resetAllGold()
{
    nextInactiveGold = 0;
    
    CCArray *golds = batch->getChildren();
    
    
    for (int i=0;i<golds->count();i++) {
        
        CCObject *obj = golds->objectAtIndex(i);
        
        Gold *gold = (Gold *)obj;
        gold->setVisible(false);
        gold->setPosition(CCPointZero);
    }
}

void GoldCache::resetNextInactiveGold()
{
    nextInactiveGold = 0;
}

void GoldCache::removeBodies(b2World *world)
{
    CCArray *golds = batch->getChildren();
    
    
    for (int i=0;i<golds->count();i++) {
        
        CCObject *obj = golds->objectAtIndex(i);
        
        Gold *gold = (Gold *)obj;
        if (gold->getBody() != NULL) {
            gold->setVisible(false);
            world->DestroyBody(gold->getBody());
            gold->setBody(NULL);
        }
    }
    
    resetNextInactiveGold();

}











