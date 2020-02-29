/****************************************************************************
 
 Copyright (c) 2017-2020 Kevin Wu (Feng Wu) All rights reserved.
 
 Licensed under the MIT License (the "License"); you may not use this file except
 in compliance with the License. You may obtain a copy of the License at
 
 http://opensource.org/licenses/MIT
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ****************************************************************************/

#ifndef TypeDef_h
#define TypeDef_h

#include "cocos2d.h"
#include "SpeedCC.h"

enum EEnemyPlaneType
{
    kPlaneHeroMaster = 0,
	kPlaneHeroSlave,
    kPlaneEnemyStrong,
    kPlaneEnemySpawn,
    kPlaneEnemyTypeTotalNumber
};

enum EBulletType
{
	kBulletTypeHero = 1,
	kBulletTypeEnemy1
};

enum EGameStateType
{
	kGameStateStop,
	kGameStatePause,
	kGameStatePlay,
};

enum EPowerUpType
{
	// + power up
	kPowerUpBulletPower = 0,
	kPowerUpBulletStrength,
	kPowerUpExtraWeapon,
	kPowerUpEarnCoin,
	kPowerUpFireBackEnemy,
	kPowerUpSlowDownEnemy,

	// - power up
	kPowerUpEnemyBigger,
	kPowerUpSlowDownHero,
	kPowerUpSpeedUpEnemy,
};

struct GameStateComponent : public SpeedCC::SFSMStateComponent
{
};

struct SUILayerComp : public SpeedCC::SCComponent
{
	cocos2d::Layer*		pEnemyLayer;
	cocos2d::Layer*		pPowerUpPropertyLayer;
	cocos2d::Layer*		pWeaponLayer;

	SUILayerComp()
	:pEnemyLayer(nullptr)
	,pPowerUpPropertyLayer(nullptr)
	,pWeaponLayer(nullptr)
	{}
};

struct SHeroPlaneComp : public SpeedCC::SCComponent
{
    cocos2d::Sprite*    pMasterSprite; // master plane
	cocos2d::Sprite*    pSlaveSprite; // slave plane
	cocos2d::Vec2		ptTouchPos;
    bool                bEnableFire;
	int					nBulletNumber;
	int					nPowerUpType;
	INT64				fLastBulletTimeStamp;
	float				fCurLife;
	float				fMaxLife;
    
    SHeroPlaneComp()
    :pMasterSprite(nullptr)
	,pSlaveSprite(nullptr)
    ,bEnableFire(false)
	,fLastBulletTimeStamp(0)
	,nPowerUpType(0)
	,nBulletNumber(1)
    {}
};

struct SEnemyPlaneComp : public SpeedCC::SCComponent
{
	cocos2d::Sprite*    pSprite; // plane
	EEnemyPlaneType		type;
	float				fCurLife;
	float				fMaxLife;
};

struct SEnemyPlaneManagerComp : public SpeedCC::SCComponent
{
	INT64				fLastNewPlaneTimeStamp;

	SEnemyPlaneManagerComp()
	:fLastNewPlaneTimeStamp(0)
	{
	}
};

struct SBulletComp : public SpeedCC::SCComponent
{
    cocos2d::Sprite*    pSprite;
    float               fPower;
	EBulletType			type;
};

struct SGameplayConfig
{
	float	fHeroPlaneInterval; // plane speed, the less the faster
	float	fHeroBulletInterval; // plane bullet speed, the less the faster
	float	fEnemyPlaneFeedInterval;

	SGameplayConfig()
	{
		fHeroPlaneInterval = 0.05;
		fHeroBulletInterval = 100;
		fEnemyPlaneFeedInterval = 1.0; // feed enemy plane every seconds
	}
};

struct SGameDirectorComp : public SpeedCC::SCComponent
{
	SGameplayConfig config;
	EGameStateType	gameState;

	SGameDirectorComp():gameState(kGameStateStop)
	{
	}
};

struct SGameSceneComp : public SpeedCC::SCComponent
{
    SpeedCC::SCViewController*   pPlayViewController;
    SpeedCC::SCViewBuilder*      pViewBuilder;
 
	SGameSceneComp()
    :pPlayViewController(nullptr)
    ,pViewBuilder(nullptr)
    {
    }
};

// message ids
enum ETWOKMessageType
{
    kPlaneMsgTouchMovePosition = SpeedCC::SCID::kUserID + 1, // must begin with this value
    kPlaneMsgBulletHitEnemyPlane, // {"bullet-ent":<SCEntity::Ptr>,"plane-ent":<SCEntity::Ptr>}
    kPlaneMsgBulletHitHeroPlane, // {"bullet-ent":<SCEntity::Ptr>,"plane-ent":<SCEntity::Ptr>}
	kPlaneMsgEnemyPlaneHitHeroPlane, // {"enemy-plane-ent":<SCEntity::Ptr>,"hero-plane-ent":<SCEntity::Ptr>}
    kPlaneMsgEnemyPlaneDie,
	kPlaneMsgHeroPlaneDie,
    
    kPlaneMsgBulletOutsideScreen, // {"bullet-ent-id":<int>}
    //kPlaneMsgBulletHitObject,
    
    kPlaneMsgNewGame,
	kPlaneMsgGamePause,
    kPlaneMsgGameOver,
    
    //kPlaneMsgGameTimeOneSeconds,
};

// stage objects ids
enum
{
    //--------------- entity id
    kEntityIDPlaneHero = 65536,
    kEntityIDPlaneEnemy,
    
    kEntityIDBulletHero,
    kEntityIDBulletEnemy,
    
    //kEntityIDHeroPlane,
    kEntityIDEnemyPlaneManager,
	kEntityIDGameplayScene,
    kEntityIDGameplayDirector,

	//--------------- strategy id
	kStgyIDHeroPlane,
	kStgyIDBullet,
	kStgyIDCollision,
	kStgyIDEnemyPlane,
};


#endif /* TypeDef_h */
