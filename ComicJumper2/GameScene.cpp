//
//  GameScene.cpp
//  ComicJumper2
//
//  Created by Bai Ryan on 13-6-29.
//
//

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"
#include "BackgroundLayer.h"
#include "GameLayer.h"
#include "UILayer.h"

using namespace cocos2d;
using namespace CocosDenshion;

GameScene *GameScene::instanceOfGameScene = NULL;
CCScene *GameScene::basicScene = NULL;


bool GameScene::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    setScore(0);
    CCLog("game scene init");
    instanceOfGameScene = this;
    return true;
}

CCScene *GameScene::scene()
{
    CCScene *scene = CCScene::create();
    basicScene = scene;
    
    CCLayer *layer = GameScene::create();
    scene->addChild(layer);
    
    BackgroundLayer *backgroundlayer = BackgroundLayer::create();
    scene->addChild(backgroundlayer,-1,kCustomLayerBackgroundLayer);
    
    GameLayer *gamelayer = GameLayer::create();
    scene->addChild(gamelayer,0,kCustomLayerGameLayer);
    
    UILayer *uilayer = UILayer::create();
    scene->addChild(uilayer,0,kCustomLayerUILayer);
    
    return scene;    
}


GameScene *GameScene::sharedGameScene()
{
    return instanceOfGameScene;
}

GameScene::GameScene()
{
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bell.mp3", true);
}

GameScene::~GameScene()
{

}

GameLayer* GameScene::defaultGameLayer()
{
    GameLayer *layer = (GameLayer *)basicScene->getChildByTag(kCustomLayerGameLayer);
    return layer;
}

UILayer *GameScene::defaultUILayer()
{
    UILayer *layer = (UILayer *)basicScene->getChildByTag(kCustomLayerUILayer);
    return layer;
}

int GameScene::getScore()
{
    return m_score;
}

void GameScene::setScore(int var)
{
    m_score = var;
    if(m_gameStatus == GameStatusOn && defaultUILayer() != NULL)
    {
        defaultUILayer()->setScore(var);
    }
}

void GameScene::startGame()
{

    m_gameStatus = GameStatusOn;
    setScore(0);
    defaultUILayer()->setScore(m_score);
    defaultUILayer()->showScoreLabel();
    
    defaultGameLayer()->startGameProcess();
   // defaultGameLayer()->startGameHandling();
}

void GameScene::restartGame()
{
    m_gameStatus = GameStatusOn;
    
    setScore(0);
    
    defaultUILayer()->setScore(m_score);
    defaultGameLayer()->restartGameProcess();
}

void GameScene::resumeGame()
{
    m_gameStatus = GameStatusOn;
    defaultGameLayer()->resumeGameProcess();
}

void GameScene::pauseGame()
{
    m_gameStatus = GameStatusPaused;
    defaultGameLayer()->pauseGameProcess();
}

void GameScene::endGame()
{
    m_gameStatus = GameStatusOff;
    
    defaultUILayer()->showMenu(kEndMenu);
    defaultGameLayer()->endGameProcess();
    
  //  defaultGameLayer()->endGameHandling();
   // defaultUILayer()->showMenu(kEndMenu);
}

