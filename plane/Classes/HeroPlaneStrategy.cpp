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

#include "HeroPlaneStrategy.h"
#include "TypeDef.h"

using namespace cocos2d;
using namespace SpeedCC;

enum EHeroPlaneActionTag
{
	//kActionTagBulletMove = 100,
};

HeroPlaneStrategy::~HeroPlaneStrategy()
{
	
}

void HeroPlaneStrategy::onInit()
{
	
}


void HeroPlaneStrategy::updatePosition(SpeedCC::SCStage* pStage, SpeedCC::SCMessage::Ptr ptrMsg)
{
	auto ptrPlaneEntity = pStage->getEntityByID(kEntityIDPlaneHero);
	auto ptrPlaneInfo = ptrPlaneEntity->getComponent<SHeroPlaneComp>();

	auto ptrDirector = pStage->getEntityByID(kEntityIDGameplayScene);
	auto ptrDirectorComp = ptrDirector->getComponent<SGameSceneComp>();

	switch (ptrMsg->nMsgID)
	{
	case SCID::Msg::kMsgTouchBegan:
	{
		ptrMsg->parameters.setValue(SC_KEY_RESULT, true);

		ptrPlaneInfo->bEnableFire = true; // begin to fire
		cocos2d::Director::getInstance()->getAnimationInterval();

		auto pBeginTouch = (cocos2d::Touch*)ptrMsg->parameters.getValue(SC_KEY_TOUCH).getPointer();
		auto touchBeginPos = ptrDirectorComp->pPlayViewController->getView()->convertTouchToNodeSpace(pBeginTouch);
		ptrPlaneInfo->ptTouchPos = touchBeginPos;
	}
	break;

	case SCID::Msg::kMsgTouchMoved:
	{
		// get touch position
		auto pMoveTouch = (cocos2d::Touch*)ptrMsg->parameters.getValue(SC_KEY_TOUCH).getPointer();
		auto touchMovePos = ptrDirectorComp->pPlayViewController->getView()->convertTouchToNodeSpace(pMoveTouch);

		// move plane
		auto ptrPlaneInfo = ptrPlaneEntity->getComponent<SHeroPlaneComp>();

		auto ptCurPos = ptrPlaneInfo->pMasterSprite->getPosition();
		auto ptNewPos = Vec2(ptCurPos.x + touchMovePos.x - ptrPlaneInfo->ptTouchPos.x,
			ptCurPos.y + touchMovePos.y - ptrPlaneInfo->ptTouchPos.y);

		// prevent hero plane outside screen
		auto winSize = SCWinSize();
		auto contentSize = ptrPlaneInfo->pMasterSprite->getContentSize();
		Vec2 ptArray[] = { Vec2(ptNewPos.x - contentSize.width / 2,ptNewPos.y - contentSize.height / 2),
			Vec2(ptNewPos.x - contentSize.width / 2,ptNewPos.y + contentSize.height / 2),
			Vec2(ptNewPos.x + contentSize.width / 2,ptNewPos.y + contentSize.height / 2),
			Vec2(ptNewPos.x + contentSize.width / 2,ptNewPos.y - contentSize.height / 2) };

		bool bInside = true;

		for (auto pt : ptArray)
		{
			if (pt.x < 0 || pt.x > winSize.width || pt.y < 0 || pt.y > winSize.height)
			{
				bInside = false;
				break;
			}
		}

		if (bInside)
		{// update hero plane position if new position inside screen
			//ptrPlaneInfo->pSprite->stopActionByTag(kActionTagHeroMove);
			
			auto moveAction = AMoveTo(0.05, ptNewPos);
			//moveAction->setTag(kActionTagHeroMove);
			ptrPlaneInfo->pMasterSprite->runAction(moveAction);
			//ptrPlaneInfo->pSprite->setPosition(ptNewPos);
			ptrPlaneInfo->ptTouchPos = touchMovePos;
		}
	}
	break;

	case SCID::Msg::kMsgTouchEnded:
	case SCID::Msg::kMsgTouchCancelled:
		ptrPlaneInfo->bEnableFire = false; // end to fire
		break;
	}
}

void HeroPlaneStrategy::onBulletHitsHeroPlane(SpeedCC::SCStage* pStage, SpeedCC::SCMessage::Ptr ptrMsg)
{
	auto ptrBulletEnt = ptrMsg->parameters["bullet-ent"].getObject<SCEntity::Ptr>();
	auto ptrPlaneEnt = ptrMsg->parameters["plane-ent"].getObject<SCEntity::Ptr>();

	auto ptrBulletComp = ptrBulletEnt->getComponent<SBulletComp>();
	auto ptrPlaneComp = ptrPlaneEnt->getComponent<SHeroPlaneComp>();

	ptrPlaneComp->fCurLife -= ptrBulletComp->fPower; // reduce plane life

	if (ptrPlaneComp->fCurLife <= 0)
	{// hero plane die, and game over

	}
	else
	{

	}
}


cocos2d::Animation* HeroPlaneStrategy::getEnemyExplosionAnimation()
{
	auto pAnimation = Animation::create();
	for (int i = 0; i < 8; ++i)
	{
		SpriteFrame * frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(SCString(0, "a_00%d.png", i + 1).c_str());
		pAnimation->addSpriteFrame(frame);
	}
	return pAnimation;
}

void HeroPlaneStrategy::onPlanesCollision(SpeedCC::SCStage* pStage, SpeedCC::SCMessage::Ptr ptrMsg)
{

}

bool HeroPlaneStrategy::onUpdate(SCStage* pStage, SCMessage::Ptr ptrMsg)
{
	switch (ptrMsg->nMsgID)
	{
		case SCID::Msg::kMsgFrame:
			
			break;

		case kPlaneMsgEnemyPlaneHitHeroPlane: // hero and enemy plane collision
			this->onPlanesCollision(pStage, ptrMsg);
			break;

		case kPlaneMsgBulletHitHeroPlane: // bullet hits hero plane
			this->onBulletHitsHeroPlane(pStage, ptrMsg);
			break;

		//case SCID::Msg::kMsgFSMStateChanged:
		//	break;

		case SCID::Msg::kMsgTouchBegan:
		case SCID::Msg::kMsgTouchMoved:
		case SCID::Msg::kMsgTouchEnded:
		case SCID::Msg::kMsgTouchCancelled:
			this->updatePosition(pStage, ptrMsg);
			break;

		default:
			break;
	}
    
	return true;
}
