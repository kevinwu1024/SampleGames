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

#ifndef BulletStrategy_hpp
#define BulletStrategy_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "SpeedCC.h"

class BulletStrategy : public SpeedCC::SCStrategy
{
private:
	enum EBulletType
	{
		kBulletNormal = 0,
		kBulletPower,
		kBulletEarnCoin,
		kBulletEarnCoinPower,

		kBulletTotalNumber
	};

	enum ETag
	{
		kActionIDOfBulletMove = 10,
	};

public:
	SC_STRATEGY_COMMON_DEF(BulletStrategy)

public:
	SC_AVOID_CLASS_COPY(BulletStrategy)
	SC_DEFINE_CLASS_PTR(BulletStrategy)

	virtual ~BulletStrategy();
	virtual bool onUpdate(SpeedCC::SCStage* pStage, SpeedCC::SCMessage::Ptr ptrMsg) override;

	virtual void onInit() override;

private:
	void generateBullet(SpeedCC::SCStage* pStage);
	cocos2d::Sprite* getBulletSprite(int bulletType);
	EBulletType getBulletType(int nPowerUpType);

	void onBulletHitsPlane(SpeedCC::SCStage* pStage, SpeedCC::SCMessage::Ptr ptrMsg);
	void onBulletOutsideScreen(SpeedCC::SCStage* pStage, SpeedCC::SCMessage::Ptr ptrMsg);

private:
	cocos2d::Size						_bulletDetectSize;
	std::list<cocos2d::Sprite*>			_freeBulletList;
	std::list<SpeedCC::SCRef2Ptr::Ptr>	_bulletPoolList;
	cocos2d::SpriteFrame*				_bulletSpriteFrameArray[kBulletTotalNumber];
};


#endif /* BulletStrategy_hpp */
