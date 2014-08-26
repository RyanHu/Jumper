//
//  Balloon.h
//  ComicJumper2
//
//  Created by Bai Ryan on 13-7-9.
//
//

#ifndef __ComicJumper2__Balloon__
#define __ComicJumper2__Balloon__

#include <iostream>
#include "cocos2d.h"

class Balloon : public cocos2d::CCSprite
{

public:
    virtual bool init();
    
    CREATE_FUNC(Balloon);

    CC_SYNTHESIZE(bool, m_enabled, Enabled);

};


class BalloonCache : public cocos2d::CCNode
{
public:
    
    
    virtual bool init();
    
    cocos2d::CCSpriteBatchNode *batch;
    
    int nextInactiveBalloon;
    
    Balloon *getNextBalloon();
    
    void resetAllBalloon();
    void resetNextInactiveBalloon();
    
    CREATE_FUNC(BalloonCache);
};

class BalloonWithTail: public cocos2d::CCSprite
{
public:
    virtual bool init();
    CREATE_FUNC(BalloonWithTail);

};




#endif /* defined(__ComicJumper2__Balloon__) */
