//
//  Balloon.cpp
//  ComicJumper2
//
//  Created by Bai Ryan on 13-7-9.
//
//

#include "Balloon.h"

using namespace cocos2d;

bool Balloon::init()
{
    if(!CCSprite::initWithFile("balloon.png"))
    {
        m_enabled = true;
        return false;
    }
    
    
    return true;




}

bool BalloonCache::init()
{
    nextInactiveBalloon = 0;
    
    batch = CCSpriteBatchNode::create("balloon.png");
    addChild(batch);
    
    for (int i=0; i<50; i++) {
    //    Balloon *balloon = Balloon::createWithTexture(batch->getTexture());
        Balloon *balloon = Balloon::create();
    //    balloon->setScale(0.3f);
        balloon->setVisible(false);
        batch->addChild(balloon);
    }

    
    return true;
}


bool BalloonWithTail::init()
{
    if(!CCSprite::initWithFile("balloon_straightTail.png"))
    {
    
        return false;
    }
    return true;
}













