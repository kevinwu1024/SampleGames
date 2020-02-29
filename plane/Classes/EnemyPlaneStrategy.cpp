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

#include "EnemyPlaneStrategy.h"
#include "TypeDef.h"

using namespace cocos2d;
using namespace SpeedCC;

enum EHeroPlaneActionTag
{
};

EnemyPlaneStrategy::~EnemyPlaneStrategy()
{
	
}

void EnemyPlaneStrategy::onInit()
{
	SCLoadSpriteFrame("wsparticle_p01.plist");
}

void EnemyPlaneStrategy::feedEnemyPlane(SpeedCC::SCStage* pStage)
{
	auto ptrDirectorEnt = pStage->getEntityByID(kEntityIDGameplayDirector);
	auto ptrGameConfigComp = ptrDirectorEnt->getComponent<SGameDirectorComp>();

	auto ptrEnemyManagerEnt = pStage->getEntityByID(kEntityIDEnemyPlaneManager);
	auto ptrEnemyManagerComp = ptrEnemyManagerEnt->getComponent<SEnemyPlaneManagerComp>();

	SCDateTime now;

	do
	{
		auto timeStamp = now.getStampInMilleSec();
		SC_BREAK_IF((timeStamp - ptrEnemyManagerComp->fLastNewPlaneTimeStamp) < ptrGameConfigComp->config.fEnemyPlaneFeedInterval);

	} while (0);
	
}

void EnemyPlaneStrategy::createPlane(SpeedCC::SCStage* pStage,EEnemyPlaneType type)
{
	SCString strSpriteImage;
	float fLife = 0;
	float fBulletPower = 0;
}


void EnemyPlaneStrategy::onBulletHitsPlane(SpeedCC::SCStage* pStage, SpeedCC::SCMessage::Ptr ptrMsg)
{
	auto ptrBulletEnt = ptrMsg->parameters["bullet-ent"].getObject<SCEntity::Ptr>();
	auto ptrPlaneEnt = ptrMsg->parameters["plane-ent"].getObject<SCEntity::Ptr>();

	auto ptrBulletComp = ptrBulletEnt->getComponent<SBulletComp>();
	auto ptrPlaneComp = ptrPlaneEnt->getComponent<SEnemyPlaneComp>();

	ptrPlaneComp->fCurLife -= ptrBulletComp->fPower; // reduce plane life

	if (ptrPlaneComp->fCurLife <= 0)
	{// plane die, and perform explosion effect
		auto pAniAction = Animate::create(this->getEnemyExplosionAnimation());
		ptrPlaneComp->pSprite->runAction(ASequence(pAniAction, ADelayTime(0.1), ARemoveSelf(), nullptr));
		pStage->removeEntity(ptrPlaneEnt->getID());
	}
	else
	{// plane alive, and shows spark in plane

	}
}

cocos2d::Animation* EnemyPlaneStrategy::getEnemyExplosionAnimation()
{
	auto pAnimation = Animation::create();
	for (int i = 0; i < 8; ++i)
	{
		SpriteFrame * frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(SCString(0, "a_00%d.png", i + 1).c_str());
		pAnimation->addSpriteFrame(frame);
	}
	return pAnimation;
}

void EnemyPlaneStrategy::onPlanesCollision(SpeedCC::SCStage* pStage, SpeedCC::SCMessage::Ptr ptrMsg)
{
	
}

bool EnemyPlaneStrategy::onUpdate(SCStage* pStage, SCMessage::Ptr ptrMsg)
{
	switch (ptrMsg->nMsgID)
	{
		//case SCID::Msg::kMsgFrame:
		//	//this->detectCollision(pStage);
		//	break;

		case kPlaneMsgEnemyPlaneHitHeroPlane: // hero and enemy plane collision
			this->onPlanesCollision(pStage, ptrMsg);
			break;

		case kPlaneMsgBulletHitEnemyPlane: // bullet hits enemy plane
			this->onBulletHitsPlane(pStage, ptrMsg);
			break;

		default:
			break;
	}
    
	return true;
}
