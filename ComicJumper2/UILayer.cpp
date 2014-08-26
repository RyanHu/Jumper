//
//  UILayer.cpp
//  Cocos2dxHello
//
//  Created by Bai Ryan on 13-5-31.
//
//

#include "UILayer.h"
#include "CCLocalizedString.h"
#include "config.h"
#include "GameScene.h"
#include "Balloon.h"
#include "BalloonIcon.h"
#include "GameLayer.h"

using namespace cocos2d;

UILayer::UILayer()
{
    CCLOG("UILayer constructor!");
}

UILayer::~UILayer()
{
    
}

void UILayer::draw()
{
    /*
    glLineWidth(16);
    glColorMask(155, 155, 200, 1.0);
    ccDrawColor4B(0,0,255,128);
    ccDrawLine(CCPointMake(0, 0), CCPointMake(320, 536));
    
    CCPoint vertices [] = {ccp(50,100),ccp(300,100),ccp(100,300)};
    ccDrawPoly(vertices, 3, true);
    */
}


typedef struct Node
{
    int data;
    Node *next;
}SNode;

SNode *createNodes(int l)
{
    SNode *head = NULL;

    SNode *cur = NULL;
    
    
    for (int i=0; i<l; i++) {
        if(head == NULL)
        {
            head = new SNode;
            head->next = NULL;
            head->data = arc4random()%10;
            cur = head;
        }
        else
        {
            SNode *newNode = new SNode;
            newNode->next = NULL;
            newNode->data = arc4random()%10;
            
            cur->next = newNode;
            cur = newNode;
        }
    }
    
    return head;
}

void printNode(SNode *head)
{
    int i = 0;
    SNode *cur = head;
    while (cur) {
        i++;
        CCLog("node value : %i",cur->data);
        cur = cur->next;
        if(i > 30)
            break;
    }
}

SNode *reverser(SNode *p,SNode *&head)
{
    SNode *tempHead = head;
    if(p == NULL || p->next == NULL)
    {
        head = p;
        return p;
    }
    else
    {
        SNode *temp = reverser(p->next, head);
        temp->next = p;
        if(tempHead == p)
        {
            p->next = NULL;
        }
        return p;
    
    }

}

SNode *reverser2(SNode *head)
{
    if(head == NULL || head->next == NULL)
    {
        return head;
    }
    
    //deal with nodes less than 2 or 3
    {
    
    }
    
    
    SNode *first = head;
    SNode *second = head->next;
    
    SNode *temp = second->next;
    first->next = NULL;
    
    while (1) {
        
        second->next = first;
        first = second;
        second = temp;
        temp = second->next;
        
        if(temp == NULL)
        {
            break;
        }
    }
    
    second->next = first;
    head = second;
    
    return head;
}

SNode *merge(SNode *head1, SNode *head2)
{

    

}

int find_sec_max(int data[],int count)
{
    int max;
    int sec_max;

    
    for (int i = 0; i< count; i++) {
        if(i == 0)
        {
            max = data[0];
            sec_max = max;
        }
        else if (i == 1)
        {
            if (data[i] >= max) {
                sec_max = max;
                max = data[i];
            }
            else
            {
                sec_max = data[i];
            }
        }
        else
        {
            if(data[i] >= max)
            {
                sec_max = max;
                max = data[i];
            }
            else if(data[i] > sec_max)
            {
                sec_max = data[i];
            }
        }
    }
    return sec_max;
}


bool UILayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    if(screenSize.height == 568)
    {
        menuHidePos = ccp(kMenuHidePositionX,kMenuPositionYForIphone5);
        menuShowPos = ccp(kMenuShowPositionX,kMenuPositionYForIphone5);
        
        scoreLabelHidePos = ccp(kScoreLabelHidePositionX,kScoreLabelPositionYForIphone5);
        scoreLabelShowPos = ccp(kScoreLabelShowPositionX,kScoreLabelPositionYForIphone5);
    }
    else
    {
        menuHidePos = ccp(kMenuHidePositionX,kMenuPositionY);
        menuShowPos = ccp(kMenuShowPositionX,kMenuPositionY);
    
        scoreLabelHidePos = ccp(kScoreLabelHidePositionX,kScoreLabelPositionY);
        scoreLabelShowPos = ccp(kScoreLabelShowPositionX,kScoreLabelPositionY);
    }
    
    m_currentMenuShown = NULL;
    
    addMenus();
    addLabels();
    addBalloonIcons();
    addPauseButton();
    
    cpptest();
    
    
 //   SNode *linkhead = createNodes(10);
 //   printNode(linkhead);

  //  reverser(linkhead, linkhead);
  // printNode(linkhead);
    
    
  //  linkhead = reverser2(linkhead);
  //  printNode(linkhead);
   
    
    SNode *linkhead_1 = createNodes(10);
    printNode(linkhead_1);

    CCLog(".........");
    SNode *linkhead_2 = createNodes(15);
    printNode(linkhead_2);
    
    CCLog(".........");

    int data[] = {25,12,1,3,5,3,1,3,643,64,23,45};
    int res = find_sec_max(data, sizeof(data)/4);
    CCLog("second max is %i",res);
    
    return true;
    
}

void UILayer::addBalloonIcons()
{
    m_balloonIconNode = BalloonIconNode::create();
    addChild(m_balloonIconNode);
}

void UILayer::balloonExpansionTurnOffEnd()
{
    this->removeChildByTag(kBalloonSprite, true);
}


void UILayer::addBallonAtPosition(cocos2d::CCPoint pos)
{
    if(m_balloonIconNode->isMaxCount() == true)
   // if(1)
    {
        Balloon *balloon = Balloon::create();
        //m_theBalloon->setVisible(false);
        balloon->setPosition(pos);
        this->addChild(balloon,1,kBalloonSprite);
        
        CCScaleTo *scaleTo   = CCScaleTo::create(0.5f, 1.5f);
        balloon->runAction(scaleTo);
        
        CCFadeOut *fadeout = CCFadeOut::create(0.8f);
        CCFiniteTimeAction *seq = CCSequence::create(fadeout,CCCallFunc::create(this, callfunc_selector(UILayer::balloonExpansionTurnOffEnd)),NULL);
        balloon->runAction(seq);
    //    CCActionInterval *turnoffTiles = CCTurnOffTiles::create(ccg(10,10), 2.0f);
     //   CCFiniteTimeAction *seq = CCSequence::create(turnoffTiles,CCCallFunc::create(this, callfunc_selector(UILayer::balloonExpansionTurnOffEnd)),NULL);
    /*    CCActionInterval *wave = CCWaves::create(4, 20, true, true, ccg(16,12), 10.9f);
        CCActionInterval *shaky = CCShaky3D::create(10, false, ccg(10,10), 15.0f);
        balloon->runAction(wave);
    */
     }
    else
    {
        Balloon *balloon = Balloon::create();
        //m_theBalloon->setVisible(false);
        balloon->setPosition(pos);
        this->addChild(balloon,1,kBalloonSprite);
        
        // int iconCount = m_balloonIconNode->getIconCount();
        CCPoint desPos = m_balloonIconNode->getNextInvisibleIconPos();
        CCActionInterval *scaleTo = CCScaleTo::create(0.6f, 0.5f);
        balloon->runAction(scaleTo);
        
        CCActionInterval *tintTo = CCTintTo::create(0.6f, 100, 100, 100);
        balloon->runAction(tintTo);
        
        CCActionInterval *moveTo = CCMoveTo::create(0.8f, desPos);
        CCEaseInOut *ease = CCEaseInOut::create(moveTo, 1.0f);
        
        CCFiniteTimeAction *seq = CCSequence::create(ease,CCCallFunc::create(this, callfunc_selector(UILayer::balloonEaseEnd)),NULL);
        
        balloon->runAction(seq);    
    }
    
}

void UILayer::lostLifeRefreshIcon(int lives)
{
    m_balloonIconNode->balloonIconDisplayByCount(lives -1);
}

void UILayer::balloonEaseEnd()
{
    this->removeChildByTag(kBalloonSprite, true);
    
    int lives = GameScene::sharedGameScene()->defaultGameLayer()->getPlayerLives();
    m_balloonIconNode->balloonIconDisplayByCount(lives - 1);
}



void UILayer::addMenus()
{
    addMainMenu();
    addPauseMenu();
    addEndMenu();
    showMenu(kMainMenu);
}

void UILayer::addMainMenu()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    CCMenuItemFont::setFontName("Courier");
    
    CCLabelTTF *label1 = CCLabelTTF::create(CCLocalizedString("MENU_ITEM_START", NULL), "Helvetica", 42);
    CCMenuItemLabel *item1 = CCMenuItemLabel::create(label1, this, menu_selector(UILayer::menuItemCallBackStartGame));
    item1->setColor(ccc3(0, 0, 0));
    
    CCLabelTTF *label2 = CCLabelTTF::create(CCLocalizedString("MENU_ITEM_LEADERBOARD", NULL), "Marker Felt", 42);
    CCMenuItemLabel *item2 = CCMenuItemLabel::create(label2, this, menu_selector(UILayer::menuItemCallBackLeaderboard));
    item2->setColor(ccc3(0, 0, 0));
    
    CCLabelTTF *label3 = CCLabelTTF::create(CCLocalizedString("MENU_ITEM_HELP", NULL), "Marker Felt", 42);
    CCMenuItemLabel *item3 = CCMenuItemLabel::create(label3, this, menu_selector(UILayer::menuItemCallBackHelp));
    item3->setColor(ccc3(0, 0, 0));
    
    m_mainMenu = CCMenu::create(item1,item2,item3,NULL);
    m_mainMenu->alignItemsVerticallyWithPadding(30.0f);
    addChild(m_mainMenu);
    m_mainMenu->setPosition(ccp(screenSize.width/2,screenSize.height/2));
    m_mainMenu->setPosition(menuHidePos);
    CCLog("screensize height:%f",screenSize.height);
}

void UILayer::addPauseMenu()
{
    CCMenuItemFont::setFontName("Courier");
    
    CCLabelTTF *label1 = CCLabelTTF::create(CCLocalizedString("MENU_ITEM_RESUME", NULL), "Helvetica", 42);
    CCMenuItemLabel *item1 = CCMenuItemLabel::create(label1, this, menu_selector(UILayer::menuItemCallBackResume));
    item1->setColor(ccc3(0, 0, 0));
    
    CCLabelTTF *label2 = CCLabelTTF::create(CCLocalizedString("MENU_ITEM_RESTART", NULL), "Marker Felt", 42);
    CCMenuItemLabel *item2 = CCMenuItemLabel::create(label2, this, menu_selector(UILayer::menuItemCallBackRestart));
    item2->setColor(ccc3(0, 0, 0));
    
    CCLabelTTF *label3 = CCLabelTTF::create(CCLocalizedString("MENU_ITEM_EXIT", NULL), "Marker Felt", 42);
    CCMenuItemLabel *item3 = CCMenuItemLabel::create(label3, this, menu_selector(UILayer::menuItemCallBackExit));
    item3->setColor(ccc3(0, 0, 0));
    
    m_pauseMenu = CCMenu::create(item1,item2,item3,NULL);
    m_pauseMenu->alignItemsVerticallyWithPadding(30.0f);
    addChild(m_pauseMenu);
    m_pauseMenu->setPosition(menuHidePos);
}

void UILayer::addEndMenu()
{
    CCMenuItemFont::setFontName("Courier");
    
    CCLabelTTF *label1 = CCLabelTTF::create(CCLocalizedString("MENU_ITEM_RESTART", NULL), "Helvetica", 42);
    CCMenuItemLabel *item1 = CCMenuItemLabel::create(label1, this, menu_selector(UILayer::menuItemCallBackRestart));
    item1->setColor(ccc3(0, 0, 0));
    
    CCLabelTTF *label2 = CCLabelTTF::create(CCLocalizedString("MENU_ITEM_LEADERBOARD", NULL), "Marker Felt", 42);
    CCMenuItemLabel *item2 = CCMenuItemLabel::create(label2, this, menu_selector(UILayer::menuItemCallBackLeaderboard));
    item2->setColor(ccc3(0, 0, 0));
    
    CCLabelTTF *label3 = CCLabelTTF::create(CCLocalizedString("MENU_ITEM_EXIT", NULL), "Marker Felt", 42);
    CCMenuItemLabel *item3 = CCMenuItemLabel::create(label3, this, menu_selector(UILayer::menuItemCallBackExit));
    item3->setColor(ccc3(0, 0, 0));
    
    m_endMenu = CCMenu::create(item1,item2, NULL);
    m_endMenu->alignItemsVerticallyWithPadding(30.0f);
    addChild(m_endMenu);
    m_endMenu->setPosition(menuHidePos);

}

void UILayer::setScore(int i)
{
    char str[64];
    sprintf(str, "%i",i);
    m_scoreLabel->setString(str);
}

void UILayer::addLabels()
{
    m_scoreLabel = CCLabelTTF::create("0", "Marker Felt", 26);
    m_scoreLabel->setPosition(scoreLabelHidePos);
    m_scoreLabel->setAnchorPoint(ccp(0,0));
    m_scoreLabel->setColor(ccc3(0, 0, 0));
    addChild(m_scoreLabel);
}

void UILayer::showScoreLabel()
{
    CCActionInterval *moveTo = CCMoveTo::create(0.5f, scoreLabelShowPos);
    CCEaseExponentialOut *ease = CCEaseExponentialOut::create(moveTo);
    m_scoreLabel->runAction(ease);
}

void UILayer::hideScoreLabel()
{
    CCActionInterval *moveTo = CCMoveTo::create(0.5f, scoreLabelHidePos);
    CCEaseExponentialOut *ease = CCEaseExponentialOut::create(moveTo);
    m_scoreLabel->runAction(ease);

}

void UILayer::showRankLabel()
{

}

void UILayer::hideRankLabel()
{

}

void UILayer::showPauseIcon()
{
    CCActionInterval *moveTo = CCMoveTo::create(0.5f, pauseShowPos);
    CCEaseExponentialOut *ease = CCEaseExponentialOut::create(moveTo);
    m_pauseButtonMenu->setEnabled(true);
    m_pauseButtonMenu->runAction(ease);
}

void UILayer::hidePauseIcon()
{
    CCActionInterval *moveTo = CCMoveTo::create(0.5f, pauseHidePos);
    CCEaseExponentialOut *ease = CCEaseExponentialOut::create(moveTo);
    m_pauseButtonMenu->setEnabled(false);
    m_pauseButtonMenu->runAction(ease);

}


void UILayer::showMenu(int type)
{
    cout<<"UILayer showMenu~";
    CCMenu *menu = getMenuByType(type);
    CCActionInterval *moveTo = CCMoveTo::create(0.5f, menuShowPos);
    CCEaseExponentialOut *ease = CCEaseExponentialOut::create(moveTo);
    m_currentMenuShown = menu;
    menu->runAction(ease);
}
                                                    
                                                     
void UILayer::hideMenu()
{
    cout<<"UILayer hideMenu~";

    if(m_currentMenuShown == NULL)
        return;
    CCActionInterval *moveTo = CCMoveTo::create(0.5f, menuHidePos);
    CCEaseExponentialOut *ease = CCEaseExponentialOut::create(moveTo);
    m_currentMenuShown->runAction(ease);
    m_currentMenuShown = NULL;
}

CCMenu *UILayer::getMenuByType(int type)
{
    CCMenu *menu = NULL;
    switch (type) {
        case kMainMenu:
            menu = m_mainMenu;
            break;
            
        case kPauseMenu:
            menu = m_pauseMenu;
            break;
            
        case kEndMenu:
            menu = m_endMenu;
            break;
            
        default:
            break;
    }
    return menu;
}
 

void UILayer::menuItemCallback1()
{
    hideMenu();
    
//这里注释掉了，看起来这个startGame没怎么用
    GameScene::sharedGameScene()->startGame();
}

void UILayer::menuItemCallback2()
{
    
}

void UILayer::menuItemCallback3()
{
    hideMenu();
}



void UILayer::menuItemCallBackStartGame()
{
//这里注释掉了，看起来这个startGame没怎么用
    GameScene::sharedGameScene()->startGame();
    hideMenu();
    showPauseIcon();
}

void UILayer::menuItemCallBackLeaderboard()
{

}

void UILayer::menuItemCallBackHelp()
{

}

void UILayer::menuItemCallBackResume()
{
    GameScene::sharedGameScene()->resumeGame();
    hideMenu();
    showPauseIcon();
}

void UILayer::menuItemCallBackExit()
{

}

void UILayer::menuItemCallBackRestart()
{
    GameScene::sharedGameScene()->restartGame();
    hideMenu();
    showPauseIcon();
}

void UILayer::pauseButtonCallback()
{
    if(m_pauseButtonMenu->isEnabled() == true)
    {
        GameScene::sharedGameScene()->pauseGame();
        showMenu(kPauseMenu);
        hidePauseIcon();
    }
}


/////


void UILayer::addPauseButton()
{
    CCMenuItemSprite *pauseButton;
    
    CCSprite *normalSprite = CCSprite::create("pause.png");
    CCSprite *selectedSprite = CCSprite::create("pause_pressed.png");
    
    
    pauseButton = CCMenuItemSprite::create(normalSprite, selectedSprite, this, menu_selector(UILayer::pauseButtonCallback));
    pauseButton->setScale(2.0f);
    m_pauseButtonMenu = CCMenu::create(pauseButton,NULL);
    //m_pauseButtonMenu->setScale(2.0f);
    m_pauseButtonMenu->setColor(ccc3(0, 255, 150));
    m_pauseButtonMenu->setAnchorPoint(ccp(0.5,0.5));
    addChild(m_pauseButtonMenu);
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    if(screenSize.height == 568)
    {
        pauseHidePos = ccp(kPauseButtonHidePositionX,kPauseButtonPositionYForIphone5);
        pauseShowPos = ccp(kPauseButtonShowPositionX,kPauseButtonPositionYForIphone5);
    }
    else
    {
        pauseHidePos = ccp(kPauseButtonHidePositionX,kPauseButtonPositionY);
        pauseShowPos = ccp(kPauseButtonShowPositionX,kPauseButtonPositionY);
    }

    m_pauseButtonMenu->setPosition(pauseHidePos);
}












void UILayer::cpptest()
{
    CCLog("cpptest code");
    
    Foo *f1 = new Foo(15);
    Foo *f2 = new Foo(10);
    int sum = f1->foobar(f2);
    CCLog("sum of f1 f2 is : %i",sum);
    
    delete f1;
    delete f2;
    
    
    Gamer *g = new Gamer(10);
   // g->age;
    
    delete g;
}


Foo::Foo(int x)
{
    a = x;
}

int Foo::foobar(Foo *f)
{
    return this->a + f->a;
}


Person::Person(int x)
{
    age = x;
}

int Gamer::getAge(Person *p)
{
    return getA(p);
}












