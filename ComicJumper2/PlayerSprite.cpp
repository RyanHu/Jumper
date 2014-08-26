//
//  PlaySprite.cpp
//  Cocos2dxHello
//
//  Created by Bai Ryan on 13-5-31.
//
//

#include "PlayerSprite.h"

using namespace cocos2d;


PlayerSprite :: PlayerSprite()
{
    m_lives = 1;
    m_playerStatus = kPlayerStatusNormal;
}

PlayerSprite :: ~PlayerSprite()
{
    
  //  CCSpriteFrameCache::sharedSpriteFrameCache()->purgeSharedSpriteFrameCache();
}

bool PlayerSprite::initWithWorld(b2World *world)
{
    if(!CCSprite::initWithFile("cat.png"))
    {
        parentWorld = world;
        
        
        return false;
    }
    
  //  this->setTextureRect(CCRectMake(0, 0, 30, 30));
    this->setScale(0.5f);
    
    return true;
}

PlayerSprite* PlayerSprite::createWithWorld(b2World *world)
{
    PlayerSprite *pRet = new PlayerSprite();
    
    if (pRet && pRet->initWithWorld(world))
    {
       pRet->autorelease(); 
       return pRet; 
    }
    else
    {
       delete pRet;
       pRet = NULL;
       return NULL; 
    }
}











