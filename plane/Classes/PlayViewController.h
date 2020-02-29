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

#ifndef __PLAY_SCENE_H__
#define __PLAY_SCENE_H__

#include "TypeDef.h"
#include "BackgroundViewController.h"

class PlayViewController : public SpeedCC::SCViewController
{
public:
    
    virtual void onCreate(SpeedCC::SCDictionary parameters) override;
    
    void setupUI();
    void setupStage();
	void setupGameplayFlow();
    
    bool onViewEnter(SpeedCC::SCMessage::Ptr ptrMsg);
    void onScheduleOneSeconds(float fDelta);
    
    void onStgyHeroBullet(SpeedCC::SCStage* pActor,SpeedCC::SCMessage::Ptr ptrMsg);

    //void onStgyHeroPlane(SpeedCC::SCStage* pStage, SpeedCC::SCMessage::Ptr ptrMsg);

private:
    SpeedCC::SCEventEmitter::Ptr        _ptrEventEmitter;
    SpeedCC::SCWatchInt::Ptr            _ptrScoreWatchNumber;
    
    BackgroundViewController::Ptr       _ptrBackgroundVC;
	BackgroundViewController::Ptr       _ptrCloudVC;

    SpeedCC::SCViewBuilder::Ptr         _ptrViewBuilder;
	SpeedCC::SCStage::Ptr				_ptrStage;
};

#endif // __TESTHOME_SCENE_H__
