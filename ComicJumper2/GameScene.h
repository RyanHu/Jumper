//
//  GameScene.h
//  ComicJumper2
//
//  Created by Bai Ryan on 13-6-29.
//
//

#ifndef __ComicJumper2__GameScene__
#define __ComicJumper2__GameScene__

#include <iostream>
#include "cocos2d.h"
#include "Box2D.h"

class GameLayer;
class UILayer;

typedef enum
{
    GameStatusOff = 1,
    GameStatusOn = 2,
    GameStatusPaused = 3
}GAME_STATUS;



typedef enum
{
    kCustomLayerGameSceneLayer = 1,
    kCustomLayerBackgroundLayer = 2,
    kCustomLayerGameLayer = 3,
    kCustomLayerUILayer = 4
}CUSTOMLAYER;



class GameScene : cocos2d::CCLayer
{

public:
    GameScene();
    ~GameScene();
    
    static cocos2d::CCScene *scene();
    
    virtual bool init();
    
    CREATE_FUNC(GameScene);
    
    static GameScene *sharedGameScene();
    static GameScene *instanceOfGameScene;
    CC_SYNTHESIZE(GAME_STATUS, m_gameStatus, GameStatus);
    
    int m_score;
    void setScore(int var);
    int getScore(void);
    
    void startGame();
    void restartGame();
    void pauseGame();
    void resumeGame();
    void endGame();
    
    GameLayer *defaultGameLayer();
    UILayer *defaultUILayer();


private:
    
    static cocos2d::CCScene *basicScene;
    
    

};



#endif /* defined(__ComicJumper2__GameScene__) */
