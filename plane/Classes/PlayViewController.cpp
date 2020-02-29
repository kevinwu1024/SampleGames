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

#include "PlayViewController.h"
#include "ui/CocosGUI.h"
#include "MenuViewController.h"
#include "HeroPlaneStrategy.h"
#include "BulletStrategy.h"
#include "CollisionStrategy.h"
#include "EnemyPlaneStrategy.h"


USING_NS_CC;

enum EActionTag
{
    kActionTagHeroMove = 100,
};


using namespace SpeedCC;

enum ENodeID
{
    kNodeIDHeroMasterSprite = 1,
	kNodeIDHeroSlaveSprite,
	kNodeIDWeaponLayer,
	kNodeIDEnemyLayer,
	kNodeIDPowerUpPropertyLayer,
    kNodeIDGameplayLayer,
};


void PlayViewController::onCreate(SCDictionary parameters)
{
    // call parent class method first
    SCViewController::onCreate(parameters);
    
    _ptrScoreWatchNumber = SCWatchInt::create();
    
    // setup UI and stage logic flow
    this->setupUI();
    this->setupStage();
    
    this->listenMessage(SCID::Msg::kMsgViewEnter, SC_MAKE_FUNC(onViewEnter, this));
}

bool PlayViewController::onViewEnter(SCMessage::Ptr ptrMsg)
{
    _ptrBackgroundVC->run();
	_ptrCloudVC->run();
    
    return true;
}

void PlayViewController::setupUI()
{
	SCDictionary dic;
	std::vector<SCValue> imageArray2 = { "bg_cloud1.png" ,"bg_cloud2.png" ,"bg_cloud1.png" };
	dic["image"] = imageArray2;
	dic["speed"] = 3;
	_ptrCloudVC = SCVCCreatorT<BackgroundViewController>::createVCPtr(dic);


	std::vector<SCValue> imageArray1 = { "bg_level1.jpg" ,"bg_level1.jpg" ,"bg_level1.jpg" };
	dic["image"] = imageArray1;
	dic["speed"] = 1;
    _ptrBackgroundVC = SCVCCreatorT<BackgroundViewController>::createVCPtr(dic);

    auto ptrSceneBvr = SCBehaviorViewGoto::create<MenuViewController>(SCViewNavigator::kLayerModal);    
    float fOffsetWidth = 0.24;
    
	SC_BEGIN_PARENT_ROOT(&_ptrViewBuilder, nullptr, SCWinSize())
		SC_INSERT_USER_NODE(_ptrBackgroundVC->getView(), "x=0.5; y=0.5;") // background
		SC_INSERT_USER_NODE(_ptrCloudVC->getView(), "x=0.5; y=0.5;") // cloud

		SC_INSERT_LAYER(kNodeIDEnemyLayer, "dock=bottom|mid-x;", SCGamePlaySize()) // enemy ui layer
		SC_INSERT_LAYER(kNodeIDPowerUpPropertyLayer, "dock=bottom|mid-x;", SCGamePlaySize()) // power up property layer
		SC_INSERT_LAYER(kNodeIDWeaponLayer, "dock=bottom|mid-x;", SCGamePlaySize()) // weapon ui layer

		SC_BEGIN_PARENT_LAYER(kNodeIDGameplayLayer, "dock=bottom|mid-x;", SCGamePlaySize())
			SC_INSERT_SPRITE(kNodeIDHeroMasterSprite, "x=0.5; y-by=100; z-order=1;", "plane_hero.png")
			SC_INSERT_SPRITE(kNodeIDHeroSlaveSprite, "x=0.5; y-by=100; z-order=1; visible=false;", "plane_hero.png")
        SC_END_PARENT
    SC_END_PARENT
}

void PlayViewController::setupStage()
{
	// for touch message
	_ptrEventEmitter = SCEventEmitter::create(SCEventEmitter::EEventType::kSingleTouch, this->getView());

	_ptrStage = SCStage::create();
	auto pStage = _ptrStage.getRawPointer();

	auto ptrHeroStgy = HeroPlaneStrategy::create(pStage, kStgyIDHeroPlane);
	pStage->addStrategy(ptrHeroStgy);

	auto ptrBulletStgy = BulletStrategy::create(pStage, kStgyIDBullet);
	ptrBulletStgy->setActive(false);
	pStage->addStrategy(ptrBulletStgy);

	auto ptrCollisionStgy = CollisionStrategy::create(pStage, kStgyIDCollision);
	ptrCollisionStgy->setActive(false);
	pStage->addStrategy(ptrCollisionStgy);

	auto ptrEnenyPlaneStgy = EnemyPlaneStrategy::create(pStage, kStgyIDEnemyPlane);
	ptrEnenyPlaneStgy->setActive(false);
	pStage->addStrategy(ptrEnenyPlaneStgy);

	// game director component
	auto ptrDirectorEnt = _ptrStage->newEntity(kEntityIDGameplayDirector);
	pStage->addEntity(ptrDirectorEnt);
	
	// game director FSM component
	ptrDirectorEnt->addComponent<SGameDirectorComp>();
	GameStateComponent gameStateComp;
	gameStateComp.nCurrentState = kGameStateStop; // origin state is stop
	ptrDirectorEnt->addComponent(gameStateComp);

	// game play scene
	auto ptrGameplaySceneEnt = _ptrStage->newEntity(kEntityIDGameplayScene);
	pStage->addEntity(ptrGameplaySceneEnt);
	
	SGameSceneComp gameSceneComp;
	gameSceneComp.pPlayViewController = this;
	gameSceneComp.pViewBuilder = _ptrViewBuilder.getRawPointer();
	ptrGameplaySceneEnt->addComponent(gameSceneComp);

	SUILayerComp uiLayerComp;
	uiLayerComp.pEnemyLayer = dynamic_cast<Layer*>(_ptrViewBuilder->getNodeByID(kNodeIDEnemyLayer));
	uiLayerComp.pPowerUpPropertyLayer = dynamic_cast<Layer*>(_ptrViewBuilder->getNodeByID(kNodeIDPowerUpPropertyLayer));
	uiLayerComp.pWeaponLayer = dynamic_cast<Layer*>(_ptrViewBuilder->getNodeByID(kNodeIDWeaponLayer));
	ptrGameplaySceneEnt->addComponent(uiLayerComp);

	// create hero plane entity
	SHeroPlaneComp heroInfo;
	//heroInfo.type = kPlaneHeroMaster;
	heroInfo.pMasterSprite = dynamic_cast<Sprite*>(_ptrViewBuilder->getNodeByID(kNodeIDHeroMasterSprite));
	heroInfo.pSlaveSprite = dynamic_cast<Sprite*>(_ptrViewBuilder->getNodeByID(kNodeIDHeroSlaveSprite));
	auto ptrHeroEntity = _ptrStage->newEntity(kEntityIDPlaneHero);
	ptrHeroEntity->addComponent(heroInfo);
	pStage->addEntity(ptrHeroEntity);

	// create enemy plane manager entity
	auto ptrEnemyManagerEnt = _ptrStage->newEntity(kEntityIDEnemyPlaneManager);
	pStage->addEntity(ptrEnemyManagerEnt);

	SEnemyPlaneManagerComp enemyPlaneManagerComp;
	enemyPlaneManagerComp.fLastNewPlaneTimeStamp = 0;
	ptrEnemyManagerEnt->addComponent(enemyPlaneManagerComp);

	this->setupGameplayFlow();
}


void PlayViewController::setupGameplayFlow()
{
	auto pStage = _ptrStage.getRawPointer();
	const std::vector<int> stgyIDVet = { kStgyIDBullet, kStgyIDCollision, kStgyIDEnemyPlane };

	auto ptrStgyEnable = SpeedCC::SCBehaviorStrategyActive::create(stgyIDVet, true);
	auto ptrStgyDisable = SpeedCC::SCBehaviorStrategyActive::create(stgyIDVet, false);

	// game director
	SpeedCC::SCFSMRunner::Ptr ptrRunnerDirector;
	SC_BEGIN_FSM(&ptrRunnerDirector, GameStateComponent)

		AT_STATE(kGameStatePlay)
			ON_ENTER_STATE(ptrStgyEnable)
			ON_EXIT_STATE(ptrStgyDisable)

			ON_MSG_NEXT_STATE(SC_MSG_GROUP(SCID::Msg::kMsgTouchEnded, SCID::Msg::kMsgTouchCancelled), kGameStatePause)
			ON_MSG_BEHAVIOR(kPlaneMsgHeroPlaneDie, SCBehaviorDeliverMessage::create(false, kPlaneMsgGameOver))
			ON_MSG_NEXT_STATE(kPlaneMsgGameOver, kGameStateStop)
			//ON_MSG_NEXT_STATE(kPlaneMsgGameOver, kGameStateStop)
		ENDAT_STATE

		AT_STATE(kGameStatePause)
			ON_MSG_NEXT_STATE(SCID::Msg::kMsgTouchBegan, kGameStatePlay)
			//ON_MSG_NEXT_STATE(kPlaneMsgGameOver, kGameStateStop)
			//ON_ENTER_STATE(ptrStgyEnable)
		ENDAT_STATE

		AT_STATE(kGameStateStop)
			ON_ENTER_STATE(ptrStgyDisable)
			ON_MSG_NEXT_STATE(SCID::Msg::kMsgTouchBegan, kGameStatePlay)
		ENDAT_STATE

	SC_END_FSM

	auto ptrStrategyDirector = SCStrategyFSM::create(pStage, ptrRunnerDirector, kEntityIDGameplayDirector);
	pStage->addStrategy(ptrStrategyDirector);

}





