//
//  Gold.h
//  ComicCloudJumer
//
//  Created by Bai Ryan on 13-6-2.
//
//

#ifndef __ComicCloudJumer__Gold__
#define __ComicCloudJumer__Gold__

#include "cocos2d.h"
#include "Box2D.h"

class Gold : public cocos2d::CCSprite
{
    
public:
    Gold();
    ~Gold();
    
    //virtual bool isD
    
    CC_SYNTHESIZE(b2World *,parentWorld, ParentWorld);
    CC_SYNTHESIZE(b2Body *, body, Body);
    
    
    virtual bool init();
    
    CREATE_FUNC(Gold);
    
private:
    
    
};


class GoldCache : public cocos2d::CCNode
{

public:
    GoldCache();
    ~GoldCache();
    
    
    cocos2d::CCSpriteBatchNode *batch;
    
    int nextInactiveGold;
    
    Gold *getNextGold();
    
    void resetAllGold();
    void resetNextInactiveGold();
    void removeBodies(b2World *world);
    
  
};


#endif /* defined(__ComicCloudJumer__Gold__) */
