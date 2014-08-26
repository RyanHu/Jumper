//
//  BackgroundLayer.h
//  ComicJumper2
//
//  Created by Bai Ryan on 13-6-29.
//
//

#ifndef __ComicJumper2__BackgroundLayer__
#define __ComicJumper2__BackgroundLayer__

#include <iostream>
#include "cocos2d.h"

class BackgroundLayer : public cocos2d::CCLayerColor
{
public:
    BackgroundLayer();
    ~BackgroundLayer();
  
    virtual bool init();
    CREATE_FUNC(BackgroundLayer);
    
    
    
private:
    
};

#endif /* defined(__ComicJumper2__BackgroundLayer__) */
