//
//  BalloonIcon.cpp
//  ComicJumper2
//
//  Created by Bai Ryan on 13-7-9.
//
//

#include "BalloonIcon.h"
#include "config.h"

using namespace cocos2d;


bool BalloonIconNode::init()
{
    batch = CCSpriteBatchNode::create("balloon_icon.png");
    addChild(batch);
    iconMax = kBalloonIconMax;
    iconCount = 0;
    for (int i=0; i<iconMax; i++) {
        CCSprite *sprite = CCSprite::create("balloon_icon.png");
        batch->addChild(sprite);
        sprite->setVisible(false);
        CCPoint pos = ccp(kBalloonIconPosXDefault + kBalloonIconSpace*i,kBalloonIconPosYDefault);
        sprite->setPosition(pos);
        sprite->setScale(2.0f);
    }
    
    return true;
}

CCPoint BalloonIconNode::getNextInvisibleIconPos()
{
    CCPoint desPos;
    CCArray *icons = batch->getChildren();
    if(iconCount >= 0 && iconCount < iconMax)
    {
        CCObject *obj = icons->objectAtIndex(iconCount);
        CCSprite *sprite = (CCSprite *)obj;
        desPos = sprite->getPosition();
    }
    else if(iconCount >= iconMax)
    {
        CCObject *obj = icons->objectAtIndex(0);
        CCSprite *sprite = (CCSprite *)obj;
        desPos = sprite->getPosition();
    }
    return desPos;
}


int BalloonIconNode::getIconCount()
{
    return iconCount;
}

bool BalloonIconNode::isMaxCount()
{
    if(iconCount >= iconMax)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void BalloonIconNode::balloonIconDisplayByCount(int count)
{
    if(count < 0)
    {
        CCLog("display count not right");
        return;
    }
    else if(count > iconMax)
    {
        return;
    }
    else
    {
        iconCount = count;
        CCArray *icons = batch->getChildren();
        
        for (int i=0; i<iconMax; i++) {
            CCObject *obj = icons->objectAtIndex(i);
            CCSprite *sprite = (CCSprite *)obj;
            
            if(i >= 0 && i < count)
            {
                if(sprite->isVisible() == false)
                {
                    sprite->setVisible(true);
                }
            }
            else
            {
                if(sprite->isVisible() == true)
                {
                    sprite->setVisible(false);
                }
            }
        }
    }
    
    
}