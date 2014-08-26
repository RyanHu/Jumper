//
//  Controller.h
//  Cocos2dxHello
//
//  Created by Bai Ryan on 13-6-1.
//
//

#ifndef __Cocos2dxHello__Controller__
#define __Cocos2dxHello__Controller__

#include "cocos2d.h"

class Controller : public cocos2d::CCNode,public cocos2d::CCTargetedTouchDelegate
{
public:
    
    Controller();
    ~Controller();
    
   // CREATE_FUNC(Controller);
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    
    virtual void onEnter();
    virtual void onExit();
    
    CC_SYNTHESIZE(cocos2d::CCPoint, m_touchBeganPosition, M_touchBeganPosition);
    CC_SYNTHESIZE(cocos2d::CCPoint, m_velocity, M_velocity);
    CC_SYNTHESIZE(cocos2d::CCPoint, tempPos, TempPos);
    
    CC_SYNTHESIZE(cocos2d::CCPoint, m_drift, M_drift);
    CC_SYNTHESIZE(cocos2d::CCPoint, tempPos2, TempPos2);
    
    CC_SYNTHESIZE(bool, isTouched, IsTouched);
    
    
  //  CC_SYNTHESIZE(void *, m_parentLayer, M_ParentLayer);
    
private:
    void updateVelocity(cocos2d::CCPoint position);
    void resetAllPoint();
};


#endif /* defined(__Cocos2dxHello__Controller__) */
