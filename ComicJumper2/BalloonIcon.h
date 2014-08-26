//
//  BalloonIcon.h
//  ComicJumper2
//
//  Created by Bai Ryan on 13-7-9.
//
//

#ifndef __ComicJumper2__BalloonIcon__
#define __ComicJumper2__BalloonIcon__

#include <iostream>

#include "cocos2d.h"


class BalloonIconNode : public cocos2d::CCNode
{
public:
    
    virtual bool init();
    
    CREATE_FUNC(BalloonIconNode);    
    
    void balloonIconDisplayByCount(int count);
    
    int getIconCount();
    bool isMaxCount();
    cocos2d::CCPoint getNextInvisibleIconPos();
    
private:
    
    cocos2d::CCSpriteBatchNode *batch;
    int iconMax;
    int iconCount;
};


#endif /* defined(__ComicJumper2__BalloonIcon__) */
