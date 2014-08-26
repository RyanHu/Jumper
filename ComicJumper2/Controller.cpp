//
//  Controller.cpp
//  Cocos2dxHello
//
//  Created by Bai Ryan on 13-6-1.
//
//

#include "Controller.h"
#include "func.h"
#include "GameLayer.h"

using namespace cocos2d;

Controller::Controller()
{
    resetAllPoint();
}

Controller::~Controller()
{

}



void Controller::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    CCNode::onEnter();

}

void Controller::updateVelocity(CCPoint position)
{
    CCPoint vector = vectorDistanceBetweenPoints(position, tempPos);
    CCPoint drift = vectorDistanceBetweenPoints(position, m_touchBeganPosition);
    
    m_velocity = vector;
    tempPos = position;
    
    m_drift = drift;
}

void Controller::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool Controller::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    CCPoint point = touch->getLocation();
    
    m_touchBeganPosition = point;
    tempPos = point;
    
    isTouched = true;
    
    CCLog("ccTouchBegan");
    return true;
}

void Controller::ccTouchMoved(CCTouch *touch, CCEvent *event)
{
    CCPoint point = touch->getLocation();
    updateVelocity(point);
    CCLog("ccTouchMoved");
}

void Controller::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
    isTouched = false;
    
    resetAllPoint();
}

void Controller::ccTouchCancelled(CCTouch *touch, CCEvent *event)
{
    isTouched = false;
    resetAllPoint();
}

void Controller::resetAllPoint()
{
    m_touchBeganPosition = CCPointZero;
    m_velocity = CCPointZero;
    tempPos = CCPointZero;
    m_drift = CCPointZero;
    tempPos2 = CCPointZero;
}








