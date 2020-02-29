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

#include "BulletStrategy.h"
#include "TypeDef.h"

using namespace cocos2d;
using namespace SpeedCC;


BulletStrategy::~BulletStrategy()
{
	for (int i = 0; i < kBulletTotalNumber; ++i)
	{
		_bulletSpriteFrameArray[i]->release();
	}
}

void BulletStrategy::onInit()
{
	_bulletDetectSize = Size(3,6);

	for (int i = 0; i < kBulletTotalNumber; ++i)
	{
		_bulletSpriteFrameArray[i] = SCNodeUtils::createSprite("bullet_hero_blue.png")->getSpriteFrame();
		_bulletSpriteFrameArray[i]->retain();
	}
}

void BulletStrategy::generateBullet(SCStage* pStage)
{
	auto ptrDirectorEnt = pStage->getEntityByID(kEntityIDGameplayDirector);
	auto ptrGameConfigComp = ptrDirectorEnt->getComponent<SGameDirectorComp>();

	auto ptrGameSceneEnt = pStage->getEntityByID(kEntityIDGameplayScene);
	auto ptrDirectorComp = ptrGameSceneEnt->getComponent<SGameSceneComp>();

	auto ptrLayerComp = ptrGameSceneEnt->getComponent<SUILayerComp>();
	SC_RETURN_V_IF(ptrLayerComp == nullptr);

	do
	{
		// generate hero bullet
		auto ptrPlaneEntity = pStage->getEntityByID(kEntityIDPlaneHero);
		auto ptrPlaneInfo = ptrPlaneEntity->getComponent<SHeroPlaneComp>();

		SC_BREAK_IF(ptrPlaneInfo == nullptr || !ptrPlaneInfo->bEnableFire);

		SCDateTime now;

		auto timeStamp = now.getStampInMilleSec();
		SC_BREAK_IF((timeStamp - ptrPlaneInfo->fLastBulletTimeStamp) < ptrGameConfigComp->config.fHeroBulletInterval); // generate bullet frequence

		ptrPlaneInfo->fLastBulletTimeStamp = timeStamp;

		float fSpeed = 500;
		float fOffsetX = 20;

		auto bulletType = this->getBulletType(ptrPlaneInfo->nPowerUpType);
		auto pp = ptrPlaneInfo->pSlaveSprite;

		Sprite* planeSpriteArray[] = { ptrPlaneInfo->pMasterSprite ,ptrPlaneInfo->pSlaveSprite };
		
		for (int i = 0; i < SC_ARRAY_LENGTH(planeSpriteArray); ++i)
		{
			auto pPlaneSprite = planeSpriteArray[i];
			SC_CONTINUE_IF(pPlaneSprite == nullptr || !pPlaneSprite->isVisible());

			auto planePos = pPlaneSprite->getPosition();
			auto newPosY = SCWinSize().height + 20;
			float fTime = newPosY / fSpeed;

			// create bullet ui
			auto pBulletSprite = this->getBulletSprite(bulletType);

			ptrLayerComp->pWeaponLayer->addChild(pBulletSprite);
			float x = planePos.x;
			pBulletSprite->setPosition(Vec2(x, planePos.y + 10));

			SBulletComp bulletInfo;
			bulletInfo.pSprite = pBulletSprite;
			bulletInfo.type = kBulletTypeHero;
			bulletInfo.fPower = 3;
			auto ptrBulletEnt = pStage->newEntity();
			ptrBulletEnt->addComponent(bulletInfo);
			pStage->addEntity(ptrBulletEnt);

			auto nEntityID = ptrBulletEnt->getID();
			SCLog("bullet entity id: %d", nEntityID);
			auto pAction = ASequence(AMoveTo(fTime, Vec2(x, newPosY)),
				ACallFunc([nEntityID]()
				{
					// bullet move out of screen
					auto ptrMsg = SCMessage::create();
					ptrMsg->nMsgID = kPlaneMsgBulletOutsideScreen;
					ptrMsg->parameters["bullet-ent-id"] = nEntityID;

					SCMsgDisp()->postMessage(ptrMsg);
				}), 
				nullptr);
			//pAction->setTag(kActionIDOfBulletMove);
			pBulletSprite->runAction(pAction);
		}
	} 
	while (0);

	do
	{
		// generate enemy bullet
		auto enemyPlaneEntArray = pStage->getEntityByComponent<SEnemyPlaneComp>();
		for (auto ptrPlaneEnt : enemyPlaneEntArray)
		{
			auto ptrPlaneComp = ptrPlaneEnt->getComponent<SEnemyPlaneComp>();
		}
	} 
	while (0);
}

cocos2d::Sprite* BulletStrategy::getBulletSprite(int bulletType)
{
	Sprite* pRet = nullptr;
	SpriteFrame* pBulletSpriteFrame = _bulletSpriteFrameArray[bulletType];

	if (_freeBulletList.empty())
	{// create new sprite
		pRet = Sprite::createWithSpriteFrame(pBulletSpriteFrame);
		auto ptrSprite = SCRef2Ptr::create(pRet);
		_bulletPoolList.push_back(ptrSprite);
	}
	else
	{// release from free pool
		pRet = _freeBulletList.front(); 
		pRet->setSpriteFrame(pBulletSpriteFrame);
		_freeBulletList.pop_front();
	}

	//_workingBulletList.push_back(pRet); // add to working pool

	return pRet;
}

BulletStrategy::EBulletType BulletStrategy::getBulletType(int nPowerUpType)
{
	// bullet type
	EBulletType bulletType = kBulletNormal;

	if (SC_BIT_HAS_OR(nPowerUpType, kPowerUpBulletPower))
	{
		bulletType = kBulletPower; // bullet power up

		if (SC_BIT_HAS_OR(nPowerUpType, kBulletEarnCoin))
		{
			bulletType = kBulletEarnCoinPower; // bullet power up with earn coin
		}
	}
	else if (SC_BIT_HAS_OR(nPowerUpType, kBulletEarnCoin))
	{
		bulletType = kBulletEarnCoin; // bullet earn coin only
	}

	return bulletType;
}

void BulletStrategy::onBulletHitsPlane(SpeedCC::SCStage* pStage, SpeedCC::SCMessage::Ptr ptrMsg)
{
	auto ptrBulletEnt = ptrMsg->parameters["bullet-ent"].getObject<SCEntity::Ptr>();
	auto ptrBulletComp = ptrBulletEnt->getComponent<SBulletComp>();

	pStage->removeEntity(ptrBulletEnt->getID());
	_freeBulletList.push_back(ptrBulletComp->pSprite); // recycle sprite
	ptrBulletComp->pSprite->removeFromParent();
	//_workingBulletList.remove(ptrBulletComp->pSprite);
}

void BulletStrategy::onBulletOutsideScreen(SpeedCC::SCStage* pStage, SpeedCC::SCMessage::Ptr ptrMsg)
{
	int nEntID = ptrMsg->parameters["bullet-ent-id"].getInt();
	auto ptrBulletEnt = pStage->getEntityByID(nEntID);
	SC_RETURN_V_IF(ptrBulletEnt == nullptr);

	auto ptrBulletComp = ptrBulletEnt->getComponent<SBulletComp>();
	auto pBulletSprite = ptrBulletComp->pSprite;

	pBulletSprite->removeFromParent();
	pStage->removeEntity(nEntID); // remove bullet entity from stage
	_freeBulletList.push_back(pBulletSprite); // recycle sprite
}


bool BulletStrategy::onUpdate(SCStage* pStage, SCMessage::Ptr ptrMsg)
{
	switch (ptrMsg->nMsgID)
	{
		case SCID::Msg::kMsgFrame:
			this->generateBullet(pStage); // 
			break;

		case kPlaneMsgBulletHitEnemyPlane: // bullet hits enemy plane
		case kPlaneMsgBulletHitHeroPlane: // bullet hits hero plane
			this->onBulletHitsPlane(pStage, ptrMsg);
			break;

		case kPlaneMsgBulletOutsideScreen:
			this->onBulletOutsideScreen(pStage, ptrMsg);
			break;

		default:
			break;
	}
    
	return true;
}
