//
//  BackgroundLayer.cpp
//  ComicJumper2
//
//  Created by Bai Ryan on 13-6-29.
//
//

#include "BackgroundLayer.h"

using namespace cocos2d;

BackgroundLayer::BackgroundLayer()
{

}

BackgroundLayer::~BackgroundLayer()
{

}

bool BackgroundLayer::init()
{
    if(!CCLayerColor::initWithColor(ccc4(245, 245,245, 255)))
   // if(!CCLayerColor::initWithColor(ccc4(20, 20,20, 255)))
        
    {
        return false;
    }
    
    return true;
}

