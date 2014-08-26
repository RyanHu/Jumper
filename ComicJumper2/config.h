//
//  config.h
//  ComicJumper2
//
//  Created by Bai Ryan on 13-6-30.
//
//

#ifndef ComicJumper2_config_h
#define ComicJumper2_config_h

#define kGroundBodyYAxisDefaultIphone5 100
#define kGroundBodyYAxisDefaultIphone4 50
#define kSideBodyYAxisDefault 300
#define kPlaySpriteYAxisDefault 450
#define kForceDownDirectionOnPlayer (-5.f)
#define kWorldGravity 10.0f

#define kMenuHidePositionX -160
#define kMenuPositionY 240
#define kMenuPositionYForIphone5 284
#define kMenuShowPositionX 160

#define kScoreLabelHidePositionX -100
#define kScoreLabelPositionY 450
#define kScoreLabelPositionYForIphone5 534
#define kScoreLabelShowPositionX 20




#define kMoveScreenUpperLimit 330
#define kMoveScreenUpperLimitForIpone5 400


#define kMoveScreenDownLimmitForIphone4 50
#define kMoveScreenDownLimmitForIphone5 100

//sprite tag
#define kNodeTagPlayer 1
#define kNodeTagGround 2
#define kNodeTagGoldCache 5
#define kNodeTagCloudCache 6
#define kNodeTagBalloonWithTail 7

#define kPauseButtonHidePositionX 360
#define kPauseButtonShowPositionX 280
#define kPauseButtonPositionY 440
#define kPauseButtonPositionYForIphone5 528

//    loseLifeCriticalYPos = m_farthestDistance - screenSize.height * 1.0f 
#define KCheckLoseLifeDistanceFromTopToDownIphone5 536
#define KCheckLoseLifeDistanceFromTopToDownIphone4 480
#define kCheckLoseLifeDistanceForDead 960

//Player Default Lives
#define kPlayerDefaultLives 1

#define kPlayerDieAndDropDownFromY
#define kBalloonFloatingDistanceMax 900

#define kBalloonIconMax 4
#define kBalloonIconPosXDefault 20
#define kBalloonIconPosYDefault 20
#define kBalloonIconSpace 25
#define kBalloonNextDistance 3000
#define kBalloonYPosOffSetRange 500

#endif
