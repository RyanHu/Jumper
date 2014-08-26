//
//  UILayer.h
//  Cocos2dxHello
//
//  Created by Bai Ryan on 13-5-31.
//
//

#ifndef __Cocos2dxHello__UILayer__
#define __Cocos2dxHello__UILayer__

#include "cocos2d.h"

typedef enum
{
    kMainMenu = 1,
    kPauseMenu = 2,
    kEndMenu = 3
}kMenuType;

typedef enum {
    kBalloonSprite = 4,
    kBalloonIcon = 5

}kSpriteAndIcons;

class BalloonIconNode;

class UILayer : public cocos2d::CCLayer
{
public:
    
    UILayer();
    ~UILayer();
    
    virtual bool init();
    
    virtual void draw();
    
    CREATE_FUNC(UILayer);
    
    void showMenu(int type);
    void hideMenu();

    void showScoreLabel();
    void hideScoreLabel();
    
    void showRankLabel();
    void hideRankLabel();
    
    void showPauseIcon();
    void hidePauseIcon();
    
    void setScore(int i);
    
    void addBallonAtPosition(cocos2d::CCPoint pos);
    void lostLifeRefreshIcon(int lives);

private:
    void addBalloonIcons();
    void addMenus();
    
    void addMainMenu();
    void addPauseMenu();
    void addEndMenu();
    
    void addPauseButton();
    
    void addLabels();
    
    BalloonIconNode *m_balloonIconNode;
    
    cocos2d::CCMenu *m_mainMenu;
    cocos2d::CCMenu *m_pauseMenu;
    cocos2d::CCMenu *m_endMenu;
    
    cocos2d::CCMenu *m_currentMenuShown;
    
    cocos2d::CCMenu *m_pauseButtonMenu;
    
    cocos2d::CCLabelTTF *m_scoreLabel;
    cocos2d::CCLabelTTF *m_rankLabel;
   // cocos2d::CCLabelTTF *m_
    
    
    void menuItemCallback1();
    void menuItemCallback2();
    void menuItemCallback3();
    
    void menuItemCallBackStartGame();
    void menuItemCallBackLeaderboard();
    void menuItemCallBackHelp();
    
    void menuItemCallBackResume();
    void menuItemCallBackExit();
    
    void menuItemCallBackRestart();
    
    void pauseButtonCallback();
    
    cocos2d::CCMenu *getMenuByType(int type);
    
    cocos2d::CCPoint menuHidePos;
    cocos2d::CCPoint menuShowPos;
    
    cocos2d::CCPoint pauseHidePos;
    cocos2d::CCPoint pauseShowPos;
    
    cocos2d::CCPoint scoreLabelHidePos;
    cocos2d::CCPoint scoreLabelShowPos;
    
    
    void balloonEaseEnd();
    void balloonExpansionTurnOffEnd();

    void cpptest();

    
};




class Foo
{
public:
    Foo(int);
    int foobar(Foo *f);
    
private:
    int a;
};


class Person
{
public:
    Person(int);
    static int getA(Person *p){return p->age;}

//private:
protected:
    int age;

};

class Gamer : public Person
{
public:
    Gamer(int x):Person(x){};
    
    int getAge(Person *p);
    
};


#endif /* defined(__Cocos2dxHello__UILayer__) */
