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

class PlayController : public SpeedCC::SCViewController
{
public:
    virtual void onCreate(SpeedCC::SCDictionary parameters) override;
    
    void setupUI();
    void setupStage();
    
    void onStrategySwipRecognize(SpeedCC::SCStage* pStage,SpeedCC::SCMessage::Ptr ptrMsg);
//    void onStrategyDirector(SpeedCC::SCStage* pStage,SpeedCC::SCMessage::Ptr ptrMsg);
    
    void onBvrDirectorNewGame(const SpeedCC::SCDictionary& par);
    void onBvrDirectorSwipe(const SpeedCC::SCDictionary& par);
    void onBvrDirectorCheckGameOver(const SpeedCC::SCDictionary& par);
    void calculateBlockSituation(SBlock* pNumber,
                                 const int nSize,
                                 std::map<int,int>& moveMap, // id => step
                                 std::list<int>& removeList, // id
                                 std::map<int,int>& mergeMap); // index => number
    void spawnBlock(SpeedCC::SCObjPtrT<SBlockMatrix> blockPtr,const bool bStart=false);
    void addBlock(const std::list<SBlockUpdate>& blockList);
    
    void onBvrNumberBlockNewGame(const SpeedCC::SCDictionary& par);
    void onStrategyNumberBlocks(SpeedCC::SCStage* pStage,SpeedCC::SCMessage::Ptr ptrMsg);
    
private:
    SBlockUIInfo getColorByNumber(const int nNumber) const;
    
private:
    cocos2d::Sprite*                    _pPanelSprite;
    SpeedCC::SCEventEmitter::Ptr        _ptrEventEmitter;
    cocos2d::Vec2                       _blockPosArray[4][4];
    SpeedCC::SCWatchInt::Ptr            _ptrScoreWatchNumber;
	SpeedCC::SCStage::Ptr				_ptrStage;
};

#endif // __TESTHOME_SCENE_H__
