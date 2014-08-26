//
//  Cloud.h
//  ComicJumper2
//
//  Created by Bai Ryan on 13-6-30.
//
//

#ifndef __ComicJumper2__Cloud__
#define __ComicJumper2__Cloud__

#include <iostream>
#include "cocos2d.h"
#include "Box2D.h"

class Cloud : public cocos2d::CCSprite
{
public:
    Cloud();
    ~Cloud();
    
    virtual bool init();
    
    CREATE_FUNC(Cloud);
};

class Cloud1 : public cocos2d::CCSprite
{
public:
    
    virtual bool init();
    
    CREATE_FUNC(Cloud1);
};

class Cloud2 : public cocos2d::CCSprite
{
public:
    
    virtual bool init();
    
    CREATE_FUNC(Cloud2);
};


class Cloud3 : public cocos2d::CCSprite
{
public:
    virtual bool init();
    
    CREATE_FUNC(Cloud3);
};




class CloudCache : public cocos2d::CCNode
{
public:
    CloudCache();
    ~CloudCache();

    cocos2d::CCSprite *pickARandomNextCloud();
    
    cocos2d::CCSpriteBatchNode *batch1;
    cocos2d::CCSpriteBatchNode *batch2;
    cocos2d::CCSpriteBatchNode *batch3;
    
    int nextInactiveCloud1;
    int nextInactiveCloud2;
    int nextInactiveCloud3;
    
    cocos2d::CCSprite *getNextCloudByType(int cloudType);
    void resetNextInactiveCloudByType(int cloudType);
};


#endif /* defined(__ComicJumper2__Cloud__) */
