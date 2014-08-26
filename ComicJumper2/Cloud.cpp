//
//  Cloud.cpp
//  ComicJumper2
//
//  Created by Bai Ryan on 13-6-30.
//
//

#include "Cloud.h"

using namespace cocos2d;


Cloud::Cloud()
{

}

Cloud::~Cloud()
{

}

bool Cloud::init()
{
    if(!CCSprite::initWithFile("cloud_blur_1.png"))
    {
        return false;
    }
    
    return true;
}


bool Cloud1::init()
{
    if(!CCSprite::initWithFile("cloud_1.png"))
    {
        return false;
    }
    
    return true;
}


bool Cloud2::init()
{
    if(!CCSprite::initWithFile("cloud_2.png"))
    {
        return false;
    }
    
    return true;
}

bool Cloud3::init()
{
    if(!CCSprite::initWithFile("cloud_3.png"))
    {
        return false;
    }
    
    return true;
}



CloudCache::CloudCache()
{
    nextInactiveCloud1 = 0;
    nextInactiveCloud2 = 0;
    nextInactiveCloud3 = 0;
    
    batch1 = CCSpriteBatchNode::create("cloud_1.png");
    addChild(batch1);
    for (int i=0; i<5; i++) {
        Cloud1 *cloud = Cloud1::create();
    //    cloud->setScale(3.0f);
        cloud->setVisible(false);
        batch1->addChild(cloud);
    }
    
    batch2 = CCSpriteBatchNode::create("cloud_2.png");
    addChild(batch2);
    for (int i=0; i<5; i++) {
        Cloud2 *cloud = Cloud2::create();
     //   cloud->setScale(3.0f);
        cloud->setVisible(false);
        batch2->addChild(cloud);
    }

    batch3 = CCSpriteBatchNode::create("cloud_3.png");
    addChild(batch3);
    for (int i=0; i<5; i++) {
        Cloud3 *cloud = Cloud3::create();
    //    cloud->setScale(3.0f);
        cloud->setVisible(false);
        batch3->addChild(cloud);
    }
}

CloudCache::~CloudCache()
{
 //   batch1->removeAllChildrenWithCleanup(true);
 //   batch2->removeAllChildrenWithCleanup(true);
 //   batch3->removeAllChildrenWithCleanup(true);
}

cocos2d::CCSprite *CloudCache::pickARandomNextCloud()
{
    int type = arc4random()%3 + 1;
    return getNextCloudByType(type);
}

CCSprite *CloudCache::getNextCloudByType(int cloudType)
{
    CCArray *clouds = NULL;
    CCObject *obj  = NULL;
    if(cloudType == 1)
    {
        clouds = batch1->getChildren();
        obj = clouds->objectAtIndex(nextInactiveCloud1);
    }
    else if(cloudType == 2)
    {
        clouds = batch2->getChildren();
        obj = clouds->objectAtIndex(nextInactiveCloud2);
    }
    else if(cloudType == 3)
    {
        clouds = batch3->getChildren();
        obj = clouds->objectAtIndex(nextInactiveCloud3);
    }
    else
    {
        return NULL;
    }
    
    Cloud *cloud = (Cloud *)obj;
    
    if(cloudType == 1)
    {
        nextInactiveCloud1 ++;
        if(nextInactiveCloud1 >= clouds->count())
        {
            nextInactiveCloud1 = 0;
        }
    }
    else if(cloudType == 2)
    {
        nextInactiveCloud2 ++;
        if(nextInactiveCloud2 >= clouds->count())
        {
            nextInactiveCloud2 = 0;
        }
    }
    else if(cloudType == 3)
    {
        nextInactiveCloud3 ++;
        if(nextInactiveCloud3 >= clouds->count())
        {
            nextInactiveCloud3 = 0;
        }
    }
    return (CCSprite *)cloud;
}


void CloudCache::resetNextInactiveCloudByType(int cloudType)
{
    if(cloudType == 1)
    {
        nextInactiveCloud1 = 0;
    }
    else if(cloudType == 2)
    {
        nextInactiveCloud2 = 0;
    }
    else if(cloudType == 3)
    {
        nextInactiveCloud3 = 0;
    }
}






