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

#ifndef EnemyPlaneStrategy_hpp
#define EnemyPlaneStrategy_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "SpeedCC.h"
#include "TypeDef.h"

class EnemyPlaneStrategy : public SpeedCC::SCStrategy
{
public:
	SC_STRATEGY_COMMON_DEF(EnemyPlaneStrategy)

public:
	SC_AVOID_CLASS_COPY(EnemyPlaneStrategy)
	SC_DEFINE_CLASS_PTR(EnemyPlaneStrategy)

	virtual ~EnemyPlaneStrategy();

	virtual bool onUpdate(SpeedCC::SCStage* pStage, SpeedCC::SCMessage::Ptr ptrMsg) override;
	virtual void onInit() override;

private:
	void feedEnemyPlane(SpeedCC::SCStage* pStage);
	void onBulletHitsPlane(SpeedCC::SCStage* pStage, SpeedCC::SCMessage::Ptr ptrMsg);
	void onPlanesCollision(SpeedCC::SCStage* pStage, SpeedCC::SCMessage::Ptr ptrMsg);

	void createPlane(SpeedCC::SCStage* pStage,EEnemyPlaneType type);

	cocos2d::Animation* getEnemyExplosionAnimation();
private:
	
};


#endif /* EnemyPlaneStrategy_hpp */
