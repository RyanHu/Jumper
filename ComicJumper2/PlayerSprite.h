//
//  PlaySprite.h
//  Cocos2dxHello
//
//  Created by Bai Ryan on 13-5-31.
//
//

#ifndef __Cocos2dxHello__PlaySprite__
#define __Cocos2dxHello__PlaySprite__

#include "cocos2d.h"
#include "Box2D.h"

typedef enum
{
    kPlayerStatusNormal = 1,
    kPlayerStatusLosingLife = 2,
    kPlayerStatusDead = 3,
    kPlayerStatusFloating = 4
}PlayerStatus;

class PlayerSprite : public cocos2d::CCSprite
{

public:
    PlayerSprite();
    ~PlayerSprite();

    //virtual bool isD
    
    CC_SYNTHESIZE(b2World *,parentWorld, ParentWorld);
    CC_SYNTHESIZE(b2Body *, body, Body);
    
    CC_SYNTHESIZE(int, m_lives, PlayerLives);
    CC_SYNTHESIZE(int, m_playerStatus, PlayerStatus);
    
    virtual bool initWithWorld(b2World *world);
    
  //  CREATE_FUNC(PlaySprite);
    static PlayerSprite* createWithWorld(b2World *world);
    
private:
    

};


#endif /* defined(__Cocos2dxHello__PlaySprite__) */
