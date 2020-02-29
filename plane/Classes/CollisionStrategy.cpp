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

#include "CollisionStrategy.h"
#include "TypeDef.h"

using namespace cocos2d;
using namespace SpeedCC;

CollisionStrategy::~CollisionStrategy()
{
	
}

void CollisionStrategy::onInit()
{
	SCLoadSpriteFrame("wsparticle_p01.plist");
}

void CollisionStrategy::detectCollision(SpeedCC::SCStage* pStage)
{
	auto bulletEntArray = pStage->getEntityByComponent<SBulletComp>();
	auto enemyPlaneEntArray = pStage->getEntityByComponent<SEnemyPlaneComp>();
	auto ptrHeroPlaneEnt = pStage->getEntityByID(kEntityIDPlaneHero);
	auto ptrHeroPlaneComp = ptrHeroPlaneEnt->getComponent<SHeroPlaneComp>();
	auto heroPlaneRect = ptrHeroPlaneComp->pMasterSprite->getBoundingBox();

	// check collision between bullet and plane
	for (auto ptrBulletEnt : bulletEntArray)
	{
		auto ptrBulletComp = ptrBulletEnt->getComponent<SBulletComp>();
		SC_CONTINUE_IF(ptrBulletComp == nullptr);

		if (ptrBulletComp->type == kBulletTypeHero)
		{// bullet from hero
			for (auto ptrEnemyPlaneEnt : enemyPlaneEntArray)
			{
				auto ptrEnemyComp = ptrEnemyPlaneEnt->getComponent<SEnemyPlaneComp>();
				auto planeContentSize = ptrEnemyComp->pSprite->getContentSize();
				auto rect = ptrEnemyComp->pSprite->getBoundingBox();
				if (rect.containsPoint(ptrBulletComp->pSprite->getPosition()))
				{
					// hero bullet hits enemy plane,
					// and generate hero bullet hits enemy plane message
					auto ptrMsg = SCMessage::create();
					ptrMsg->nMsgID = kPlaneMsgBulletHitEnemyPlane;
					ptrMsg->parameters["bullet-ent"] = ptrBulletEnt;
					ptrMsg->parameters["plane-ent"] = ptrEnemyPlaneEnt;

					SCMsgDisp()->postMessage(ptrMsg);
				}
			}
		}
		else if(ptrHeroPlaneComp != nullptr)
		{// bullet from enemy
			
			if (heroPlaneRect.containsPoint(ptrBulletComp->pSprite->getPosition()))
			{
				// ememy bullet hits hero hero plane, 
				// and generate enemy bullet hits hero plane message
				auto ptrMsg = SCMessage::create();
				ptrMsg->nMsgID = kPlaneMsgBulletHitHeroPlane;
				ptrMsg->parameters["bullet-ent"] = ptrBulletEnt;
				ptrMsg->parameters["plane-ent"] = ptrHeroPlaneEnt;

				SCMsgDisp()->postMessage(ptrMsg);
			}
		}
	}

	// check collision plane between hero and enemy
	for (auto ptrEnemyPlaneEnt : enemyPlaneEntArray)
	{
		auto ptrEnemyComp = ptrEnemyPlaneEnt->getComponent<SEnemyPlaneComp>();
		auto enemyRect = ptrEnemyComp->pSprite->getBoundingBox(); 
		if (enemyRect.intersectsRect(heroPlaneRect))
		{
			// ememy bullet hits hero hero plane, 
			// and generate enemy bullet hits hero plane message
			auto ptrMsg = SCMessage::create();
			ptrMsg->nMsgID = kPlaneMsgEnemyPlaneHitHeroPlane;
			ptrMsg->parameters["enemy-plane-ent"] = ptrEnemyPlaneEnt;
			ptrMsg->parameters["hero-plane-ent"] = ptrHeroPlaneEnt;

			SCMsgDisp()->postMessage(ptrMsg);
		}
	}
	
}


bool CollisionStrategy::onUpdate(SCStage* pStage, SCMessage::Ptr ptrMsg)
{
	switch (ptrMsg->nMsgID)
	{
		case SCID::Msg::kMsgFrame:
			this->detectCollision(pStage);
			break;

		default:
			break;
	}
    
	return true;
}
