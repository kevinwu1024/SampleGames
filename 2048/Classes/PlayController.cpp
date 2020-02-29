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

#include "PlayController.h"
#include "ui/CocosGUI.h"
#include "MenuController.h"


USING_NS_CC;

using namespace SpeedCC;


#define BestScoreSetting\
    SCSetting::getInstance()->getWatchInt("best_score")


#define TWOK_ARG_BLOCK_UPDATE_LIST      "block_update"

void PlayController::onCreate(SCDictionary parameters)
{
    // call parent class method first
    SCViewController::onCreate(parameters);
    
	std::srand(unsigned(time(0)));

    _ptrScoreWatchNumber = SCWatchInt::create();
    
    // setup UI and stage logic flow
    this->setupUI();
    this->setupStage();
}

void PlayController::setupUI()
{
    auto ptrSceneBvr = SCBehaviorViewGoto::create<MenuController>(SCViewNavigator::kLayerModal);
    float fOffsetWidth = 0.24;
    
    SC_BEGIN_PARENT_ROOT(nullptr,nullptr,SCWinSize())
    
        SC_INSERT_LAYER_COLOR(nullptr,"dock=center;",SCWinSize(),Color4B::WHITE)
    
        SC_BEGIN_PARENT_LAYER(nullptr,"dock=bottom|mid-x;",SCGamePlaySize())
    
            SC_BEGIN_PARENT_ALIGNMENT(nullptr, "dock=top|mid-x; y-by=-10;", true, 17, SCNodeUtils::EDockType::kDockCenter)
    
                // game title
                SC_BEGIN_PARENT_LAYER(nullptr, nullptr, Size(90,90))
                    SC_INSERT_SPRITE(nullptr,"dock=center; color=(236,196,0); scale=0.9;","block.png")
                    SC_INSERT_LABEL(nullptr,"dock=center;", "2048", "", 30)
                SC_END_PARENT
    
                // record
                SC_BEGIN_PARENT_LAYER(nullptr, nullptr, Size(90,90))
    
                    SC_INSERT_SPRITE(nullptr, "dock=top|mid-x; scale-x=0.9; scale-y=0.55; color=(187,173,160);", "block.png")
                    SC_INSERT_LABEL(nullptr,"dock=top|mid-x; y-by=-7; text-color=(238,228,218);", "SCORE", "", 14)
                    SC_INSERT_LABEL(nullptr,"dock=center; y-by=6;", _ptrScoreWatchNumber, "", 24)
    
                    SC_INSERT_SPRITE(nullptr,"dock=bottom|mid-x; scale-x=0.9; scale-y=0.23; color=(236, 141, 83);", "block.png")
                    SC_INSERT_BUTTON_LABEL(nullptr,"dock=bottom|mid-x; y-by=3;", "menu", "", 17, ptrSceneBvr)
    
                SC_END_PARENT
    
                // best record
                SC_BEGIN_PARENT_LAYER(nullptr, nullptr, Size(90,90))
                    SC_INSERT_SPRITE(nullptr, "dock=top|mid-x; scale-x=0.9; scale-y=0.55; color=(187,173,160);", "block.png")
                    SC_INSERT_LABEL(nullptr,"dock=top|mid-x; y-by=-7; text-color=(238,228,218);", "BEST", "", 14)
                    SC_INSERT_LABEL(nullptr,"dock=center; y-by=6;", BestScoreSetting, "", 24)
                SC_END_PARENT
    
            SC_END_PARENT
    
    
            // digital panel
            SC_BEGIN_PARENT_SPRITE(&_pPanelSprite, "x=0.5; y=0.38; scale=3; color=(187,173,160);","block.png")
                for(int i=0; i<4; ++i)
                {
                    for(int j=0; j<4; ++j)
                    {
                        Sprite* pSprite = nullptr;
                        auto strPos = SCString(0,"x=%.3f; y=%.3f;",0.141+j*fOffsetWidth,0.141+i*fOffsetWidth);
                        SC_INSERT_SPRITE(&pSprite, strPos + "scale=0.2; color=(204,192,180);", "block.png")
                        _blockPosArray[i][j] = pSprite->getPosition();
                    }
                }
    
            SC_END_PARENT
        SC_END_PARENT
    SC_END_PARENT
    
}

void PlayController::setupStage()
{
	_ptrEventEmitter = SCEventEmitter::create(SCEventEmitter::EEventType::kSingleTouch, _pPanelSprite);
	_ptrStage = SCStage::create();
	auto pStage = _ptrStage.getRawPointer();

    auto ptrSwipeBvr = SCBehaviorCallFunc::create(SC_MAKE_FUNC(onBvrDirectorSwipe, this));
    auto ptrCheckOverBvr = SCBehaviorCallFunc::create(SC_MAKE_FUNC(onBvrDirectorCheckGameOver, this));
    auto ptrNewGameBvr = SCBehaviorCallFunc::create(SC_MAKE_FUNC(onBvrDirectorNewGame, this));
    
    SCDictionary dic = {"score",_ptrScoreWatchNumber};
    auto preGameOverBvr = SCBehaviorViewGoto::create<GameOverController>(SCViewNavigator::kSceneReplace,dic);
    
    // game director
    auto ptrEntityDirector = SCEntity::create(pStage, kEntityIDDirector);
    ptrEntityDirector->addComponent<SBlockMatrix>();
    ptrEntityDirector->addComponent<SSwipeRecogInfo>();
    ptrEntityDirector->addComponent<SBlockRole>();
	pStage->addEntity(ptrEntityDirector);
    
    SpeedCC::SCFSMRunner::Ptr ptrRunnerDirector;
    SC_BEGIN_FSM_LITE(&ptrRunnerDirector)
        ON_MSG_BEHAVIOR(SC_MSG_GROUP(kTWOKMsgSwipeLeft,
                                     kTWOKMsgSwipeRight,
                                     kTWOKMsgSwipeUp,
                                     kTWOKMsgSwipeDown),
                        ptrSwipeBvr)
    
        ON_MSG_BEHAVIOR(SC_MSG_GROUP(kTWOKMsgNewGame,
                                     SpeedCC::SCID::Msg::kMsgViewEnter),
                        ptrNewGameBvr)
    

        ON_MSG_BEHAVIOR(kTWOKMsgBlocksReady, ptrCheckOverBvr)
		ON_MSG_BEHAVIOR(kTWOKMsgSwipeLeft, ptrCheckOverBvr)
    
        // manages other entity
        ON_MSG_STRATEGY_ACTIVE(kTWOKMsgBlocksReady,kStgyIDSwipeRecog,true)
        ON_MSG_STRATEGY_ACTIVE(SC_MSG_GROUP(kTWOKMsgGameOver,
                                        kTWOKMsgUpdateBlocks),kStgyIDSwipeRecog,false)
    
        ON_MSG_BEHAVIOR(kTWOKMsgGameOver, preGameOverBvr)
    SC_END_FSM
    
    auto ptrStrategyDirector = SCStrategyFSM::create(pStage,ptrRunnerDirector,kEntityIDDirector);
	pStage->addStrategy(ptrStrategyDirector);
    
    auto ptrStrategyRecog = SCStrategyFunc::create(pStage,SC_MAKE_FUNC(onStrategySwipRecognize, this),kStgyIDSwipeRecog);
	pStage->addStrategy(ptrStrategyRecog);
    
    auto ptrStrategyBlock = SCStrategyFunc::create(pStage,SC_MAKE_FUNC(onStrategyNumberBlocks, this));
	pStage->addStrategy(ptrStrategyBlock);
}


void PlayController::onStrategyNumberBlocks(SCStage* pStage,SCMessage::Ptr ptrMsg)
{
    SC_RETURN_V_IF(ptrMsg->nMsgID != kTWOKMsgUpdateBlocks);
    
    bool bResult = false;
    auto* pList = ptrMsg->parameters.getValue(TWOK_ARG_BLOCK_UPDATE_LIST).getRefObject<std::list<SBlockUpdate>>(&bResult);
    
    auto ptrEntityDirector = pStage->getEntityByID(kEntityIDDirector);
    
    SC_RETURN_V_IF((pList == nullptr || !bResult));
    
    for(const auto it : *pList)
    {
		auto ptrBlockEntity = pStage->getEntityByID(it.nID);
        auto block = ptrBlockEntity->getComponent<SBlockProperty>();
		SCASSERT(block != nullptr);
            
        switch(it.update)
        {
            case SBlockUpdate::kMove:
            {
                auto ptrMove = ptrEntityDirector->getComponent<SBlockRole>();
                auto pEntityDirector = ptrEntityDirector.getRawPointer();
                ++ptrMove->nCounter;
                block->pNode->runAction(AMoveTo(0.2,_blockPosArray[it.nPosY][it.nPosX]));
                this->delayExecute(0.2, [this,pEntityDirector]()
                                    {
                                        auto ptrMove = pEntityDirector->getComponent<SBlockRole>();
                                        if(--(ptrMove->nCounter)==0)
                                        {
                                            SCMsgDisp()->sendMessage(kTWOKMsgBlocksReady);
                                        }
                                    });
            }
                break;
                    
            case SBlockUpdate::kRemove:
            {
                block->pNode->removeFromParent();
                ptrBlockEntity->removeFromStage();
                SCMsgDisp()->postMessage(kTWOKMsgBlocksReady);
            }
                break;
        }
    }
    
}

void PlayController::onStrategySwipRecognize(SCStage* pStage,SCMessage::Ptr ptrMsg)
{
    SC_RETURN_V_IF(ptrMsg->nMsgID != SCID::Msg::kMsgTouchBegan
                   && ptrMsg->nMsgID != SCID::Msg::kMsgTouchMoved
                   && ptrMsg->nMsgID != SCID::Msg::kMsgTouchEnded);
    
    auto ptrEntity = pStage->getEntityByID(kEntityIDDirector);
    auto ptrRecog = ptrEntity->getComponent<SSwipeRecogInfo>();
    
    switch(ptrMsg->nMsgID)
    {
        case SCID::Msg::kMsgTouchBegan:
        {
            auto pTouch = (cocos2d::Touch*)ptrMsg->parameters.getValue(SC_KEY_TOUCH).getPointer();
            ptrRecog->beginPos = _pPanelSprite->convertTouchToNodeSpace(pTouch);
            bool bInside = Rect(Vec2(0,0),_pPanelSprite->getContentSize()).containsPoint(ptrRecog->beginPos);
            
            ptrMsg->parameters.setValue(SC_KEY_RESULT,bInside); // must set true if want to receive SCID::Msg::kMsgTouchMoved
            ptrRecog->bDidMove = false;
        }
            break;
            
        case SCID::Msg::kMsgTouchMoved:
        {
            ptrRecog->bDidMove = true;
        }
            break;
            
        case SCID::Msg::kMsgTouchEnded:
        {
            if(ptrRecog->bDidMove)
            {
                auto pTouch = (cocos2d::Touch*)ptrMsg->parameters.getValue(SC_KEY_TOUCH).getPointer();
                auto touchPos = _pPanelSprite->convertTouchToNodeSpace(pTouch);
                
                auto posTrans = touchPos - ptrRecog->beginPos;
                
                const int nMinFingerMoveDistance = 2;
                
                if(fabs(posTrans.y)>=nMinFingerMoveDistance || fabs(posTrans.x)>=nMinFingerMoveDistance)
                {
                    float fAngle = atan2f(posTrans.y,posTrans.x) ;
                    int fAngle2 = ((int)((fAngle * (180/3.141592654))+360)) % 360 ;
                    
                    if((fAngle2 >=0 && fAngle2<=45) || (fAngle2>315 && fAngle2<=360))
                    {// swipe to right
                        SCLog("swipe right");
                        SCMsgDisp()->postMessage(kTWOKMsgSwipeRight);
                    }
                    else if(fAngle2>45 && fAngle2<=135)
                    {// swipe to up
                        SCLog("swipe up");
                        SCMsgDisp()->postMessage(kTWOKMsgSwipeUp);
                    }
                    else if(fAngle2>135 && fAngle2<=225)
                    {// swipe to left
                        SCLog("swipe left");
                        SCMsgDisp()->postMessage(kTWOKMsgSwipeLeft);
                    }
                    else
                    {// swipe to down
                        SCLog("swipe down");
                        SCMsgDisp()->postMessage(kTWOKMsgSwipeDown);
                    }
                }
            }
        }
            break;
    }
}

void PlayController::onBvrNumberBlockNewGame(const SpeedCC::SCDictionary& par)
{
    auto ptrEntity = par.getValue(SC_KEY_ENTITY).getObject<SCEntity::Ptr>();
    
    auto property = ptrEntity->getComponent<SBlockProperty>();
    property->pNode->removeFromParent();
    ptrEntity->removeFromStage();
}


void PlayController::calculateBlockSituation(SBlock* pBlock,
                                             const int nSize,
                                             std::map<int,int>& moveMap,
                                             std::list<int>& removeList,
                                             std::map<int,int>& mergeMap)
{
    SC_RETURN_V_IF(nSize<=1);
    
    int nLastNumber = -1;
    for(int i=0;i<nSize;++i)
    {
        if(pBlock[i].nNumber>0 && nLastNumber!=-1)
        {
            if(nLastNumber==0)
            {// free block
                if(moveMap.find(pBlock[i].nID)==moveMap.end())
                {
                    moveMap[pBlock[i].nID] = 1;
                }
                else
                {
                    moveMap[pBlock[i].nID] += 1;
                }
                std::swap(pBlock[i], pBlock[i-1]);
                this->calculateBlockSituation(pBlock,i,moveMap,removeList,mergeMap);
            }
            else if(pBlock[i].nNumber==nLastNumber && pBlock[i-1].nID>0 && pBlock[i].nID>0)
            {// merge block
                if(moveMap.find(pBlock[i].nID)==moveMap.end())
                {
                    moveMap[pBlock[i].nID] = 1;
                }
                else
                {
                    moveMap[pBlock[i].nID] += 1;
                }
                removeList.push_back(pBlock[i-1].nID);
                removeList.push_back(pBlock[i].nID);
                
                pBlock[i].nNumber = 0;
                pBlock[i].nID = 0;
                
                pBlock[i-1].nNumber = nLastNumber*2;
                pBlock[i-1].nID = 0;
                
                mergeMap[i-1] = pBlock[i-1].nNumber;
            }
        }
        
        nLastNumber = pBlock[i].nNumber;
    }
}

void PlayController::spawnBlock(SCObjPtrT<SBlockMatrix> ptrBlockMatrix,const bool bStart)
{
    // look for free place
    std::vector<Vec2> freePlaceList;
    for(int i=0;i<4;++i)
    {
        for(int j=0; j<4; ++j)
        {
            if(ptrBlockMatrix->block[i][j].nNumber==0)
            {
                freePlaceList.push_back(Vec2(j,i));
            }
        }
    }
    
	// free place
    std::random_shuffle(freePlaceList.begin(), freePlaceList.end());
    
    std::list<SBlockUpdate> blockUpdateList;
    SBlockUpdate bu;
    
    int nCount = bStart ? 2 : 1;
    
    for(int i=0; i<nCount; ++i)
    {
        auto place = freePlaceList.back();
        int nPosX = (int)place.x;
        int nPosY = (int)place.y;
        ++ptrBlockMatrix->nBlockIDCounter;
        ptrBlockMatrix->block[nPosY][nPosX].nNumber = 2;
        ptrBlockMatrix->block[nPosY][nPosX].nID = ptrBlockMatrix->nBlockIDCounter;
        
        bu.nNumber = ptrBlockMatrix->block[nPosY][nPosX].nNumber;
        bu.nPosX = nPosX;
        bu.nPosY = nPosY;
        bu.nID = ptrBlockMatrix->nBlockIDCounter;
        blockUpdateList.push_back(bu);
        
        freePlaceList.pop_back();
    }
    
    this->addBlock(blockUpdateList);
}

void PlayController::addBlock(const std::list<SBlockUpdate>& blockList)
{
	auto pStage = _ptrStage.getRawPointer();

    for(const auto it : blockList)
    {
        Sprite* pSprite = nullptr;
        Label* pLabel = nullptr;
        auto pos = SCNodeUtils::posA2P(_blockPosArray[it.nPosY][it.nPosX], _pPanelSprite->getContentSize());
        SCString strPos = SCString(0,"x=%.2f; y=%.2f;",pos.x, pos.y);
        SC_BEGIN_PARENT_ROOT_EX(nullptr,nullptr,_pPanelSprite->getContentSize(),_pPanelSprite,this)
            SC_BEGIN_PARENT_SPRITE(&pSprite, strPos + "scale=0.2;", "block.png")
                SC_INSERT_LABEL(&pLabel, "dock=center;", it.nNumber, "", 50)
            SC_END_PARENT
        SC_END_PARENT
        
        auto blockUI = this->getColorByNumber(it.nNumber);
        pSprite->setColor(blockUI.blockColor);
        pLabel->setColor(blockUI.fontColor);
        pLabel->setSystemFontSize(blockUI.fFontSize);
        
        SBlockProperty block;
        block.pNode = pSprite;
        block.nPosX = it.nPosX;
        block.nPosY = it.nPosY;
        
        auto ptrEntity = SCEntity::create(pStage, it.nID);
        ptrEntity->addComponent(block);
		pStage->addEntity(ptrEntity);
    }
}

void PlayController::onBvrDirectorNewGame(const SCDictionary& par)
{
    std::list<int> removeEntityIDList;
    
	_ptrStage->foreachEntity([&removeEntityIDList](SCEntity::Ptr& ptrEntity) -> bool {
		auto block = ptrEntity->getComponent<SBlockProperty>();
		if (block != nullptr)
		{
			block->pNode->removeFromParent();
			removeEntityIDList.push_back(ptrEntity->getID());
		}

		return true;
	});
    
    // remove all enities of block
    for(auto id : removeEntityIDList)
    {
		_ptrStage->removeEntity(id);
    }
    
    auto ptrEntity = par.getValue(SC_KEY_ENTITY).getObject<SCEntity::Ptr>();
    
    (*_ptrScoreWatchNumber) = 0;
    auto ptrBlockMatrix = ptrEntity->getComponent<SBlockMatrix>();
    
    SCASSERT(ptrBlockMatrix != nullptr);
    
    for(int i=0; i<4; ++i)
    {
        for(int j=0; j<4; ++j)
        {
            ptrBlockMatrix->block[i][j].nNumber = 0;
            ptrBlockMatrix->block[i][j].nID = 0;
        }
    }
    
    ptrBlockMatrix->nBlockIDCounter = kEntityIDBlockBegin;
    
    this->spawnBlock(ptrBlockMatrix,true);
}

void PlayController::onBvrDirectorCheckGameOver(const SpeedCC::SCDictionary& par)
{
    auto ptrStage = par.getValue(SC_KEY_STAGE).getObject<SCStage::Ptr>();
	auto ptrEntity = ptrStage->getEntityByID(kEntityIDDirector);

	auto ptrBlockMatrix = ptrEntity->getComponent<SBlockMatrix>();

	for (int i = 0; i<4; ++i)
	{
		int nPreNumber = 0;
		for (int j = 0; j<4; ++j)
		{
			SC_RETURN_V_IF((ptrBlockMatrix->block[i][j].nNumber == 0));
			if (j>0)
			{
				SC_RETURN_V_IF((ptrBlockMatrix->block[i][j].nNumber == nPreNumber));
			}
			nPreNumber = ptrBlockMatrix->block[i][j].nNumber;
		}
	}

	for (int i = 0; i<4; ++i)
	{
		int nPreNumber = 0;
		for (int j = 0; j<4; ++j)
		{
			if (j>0)
			{
				SC_RETURN_V_IF((ptrBlockMatrix->block[j][i].nNumber == nPreNumber));
			}
			nPreNumber = ptrBlockMatrix->block[j][i].nNumber;
		}
	}
	
    this->delayExecute(1, [this]()
                       {
                           SCMsgDisp()->postMessage(kTWOKMsgGameOver);
                       });
}


void PlayController::onBvrDirectorSwipe(const SpeedCC::SCDictionary& par)
{
    auto ptrStage = par.getValue(SC_KEY_STAGE).getObject<SCStage::Ptr>();
    auto ptrMsg = par.getValue(SC_KEY_MESSAGE).getObject<SCMessage::Ptr>();
    
	auto ptrEntity = ptrStage->getEntityByID(kEntityIDDirector);
    auto ptrBlockMatrix = ptrEntity->getComponent<SBlockMatrix>();
    
    std::map<int,int> moveMap;
    std::list<int> removeList;
    std::map<int,int> mergeMap;
    
    std::list<SBlockUpdate> mergeList;
    std::list<SBlockUpdate> moveList;
    
    bool bHorizontal = false;
    int sequenceArray[4];
    
    switch(ptrMsg->nMsgID)
    {
        case kTWOKMsgSwipeLeft:
            bHorizontal = true;
            sequenceArray[0] = 0;
            sequenceArray[1] = 1;
            sequenceArray[2] = 2;
            sequenceArray[3] = 3;
            break;
            
        case kTWOKMsgSwipeRight:
            bHorizontal = true;
            sequenceArray[0] = 3;
            sequenceArray[1] = 2;
            sequenceArray[2] = 1;
            sequenceArray[3] = 0;
            break;
            
        case kTWOKMsgSwipeUp:
            bHorizontal = false;
            sequenceArray[0] = 3;
            sequenceArray[1] = 2;
            sequenceArray[2] = 1;
            sequenceArray[3] = 0;
            break;
            
        case kTWOKMsgSwipeDown:
            bHorizontal = false;
            sequenceArray[0] = 0;
            sequenceArray[1] = 1;
            sequenceArray[2] = 2;
            sequenceArray[3] = 3;
            break;
    }
    
    
    SBlock blockArray[4];
    for(int i=0;i<4;++i)
    {
        mergeMap.clear();
        moveMap.clear();
        for(int k=0;k<4;++k)
        {
            int y = bHorizontal ? i : sequenceArray[k];
            int x = bHorizontal ? sequenceArray[k] : i;
            blockArray[k] = ptrBlockMatrix->block[y][x];
        }
        
        this->calculateBlockSituation(blockArray,SC_ARRAY_LENGTH(blockArray),moveMap,removeList,mergeMap);
        
        for(auto it : mergeMap)
        {
            SBlockUpdate bu;
            
            if(sequenceArray[3]>sequenceArray[0])
            {
                bu.nPosX = (bHorizontal) ? it.first : i;
                bu.nPosY = (bHorizontal) ? i : it.first;
            }
            else
            {
                bu.nPosX = (bHorizontal) ? (3-it.first) : i;
                bu.nPosY = (bHorizontal) ? i : (3-it.first);
            }
            
            bu.nNumber = it.second;
            bu.nID = ++ptrBlockMatrix->nBlockIDCounter;
            blockArray[it.first].nID = bu.nID;
            mergeList.push_back(bu);
        }
        
        for(auto it : moveMap)
        {
            SBlockUpdate bu;
            bu.update = SBlockUpdate::kMove;
            bu.nID = it.first;
            
            if(bHorizontal)
            {
                bu.nPosY = i;
                
                for(int j=0;j<4;++j)
                {
                    if(ptrBlockMatrix->block[i][j].nID==bu.nID)
                    {
                        bu.nPosX = (sequenceArray[3]>sequenceArray[0] ? (j - it.second) : (it.second + j));
                        break;
                    }
                }
            }
            else
            {
                bu.nPosX = i;
                
                for(int j=0;j<4;++j)
                {
                    if(ptrBlockMatrix->block[j][i].nID==bu.nID)
                    {
                        bu.nPosY = (sequenceArray[3]>sequenceArray[0] ? (j - it.second) : (it.second + j));
                        break;
                    }
                }
            }
            
            moveList.push_back(bu);
        }
        
        for(int k=0;k<4;++k)
        {
            int y = bHorizontal ? i : sequenceArray[k];
            int x = bHorizontal ? sequenceArray[k] : i;
            ptrBlockMatrix->block[y][x] = blockArray[k];
        }
    }
    
    if(!moveList.empty())
    {
        SCDictionary::SKVPair pair = {TWOK_ARG_BLOCK_UPDATE_LIST,moveList};
        SCMessage::Ptr ptrMsg = SCMessage::create(kTWOKMsgUpdateBlocks,pair);
        SCMsgDisp()->postMessage(ptrMsg);
        
        this->delayExecute(0.25, [this,ptrBlockMatrix,removeList,mergeList]()
                           {
                               if(!removeList.empty())
                               {
                                   std::list<SBlockUpdate> blockList;
                                   for(auto it : removeList)
                                   {
                                       SBlockUpdate bu;
                                       bu.update = SBlockUpdate::kRemove;
                                       bu.nID = it;
                                       blockList.push_back(bu);
                                   }
                                   
                                   SCDictionary::SKVPair pair= {TWOK_ARG_BLOCK_UPDATE_LIST,blockList};
                                   SCMessage::Ptr ptrMsg = SCMessage::create(kTWOKMsgUpdateBlocks,pair);
                                   SCMsgDisp()->postMessage(ptrMsg);
                                   
                                   // calculating score in step
                                   for(auto it : mergeList)
                                   {
                                       (*_ptrScoreWatchNumber) += it.nNumber;
                                   }
                                   
                                   if((*BestScoreSetting)<(*_ptrScoreWatchNumber))
                                   {
                                       (*BestScoreSetting) = (*_ptrScoreWatchNumber);
                                   }
                                   
                                   this->addBlock(mergeList);
                               }
                               this->spawnBlock(ptrBlockMatrix);
                           });
    }
    
}

SBlockUIInfo PlayController::getColorByNumber(const int nNumber) const
{
    SBlockUIInfo ret;
    
    ret.fontColor = Color3B(255 , 250 , 244);
    ret.fFontSize = 60;
    switch (nNumber)
    {
        case 2:
            ret.blockColor = Color3B(238 , 228, 218);
            ret.fontColor = Color3B(120 , 110, 100);
            break;
            
        case 4:
            ret.blockColor = Color3B(236, 224, 220);
            ret.fontColor = Color3B(120 , 110, 100);
            break;
            
        case 8:
            ret.blockColor = Color3B(242, 177, 123);
            break;
            
        case 16:
            ret.blockColor = Color3B(236, 141, 83);
            break;
            
        case 32:
            ret.blockColor = Color3B(245, 124, 95);
            break;
            
        case 64:
            ret.blockColor = Color3B(233, 89, 55);
            break;
            
        case 128:
            ret.blockColor =  Color3B(243, 217, 107);
            ret.fFontSize = 50;
            break;
        case 256:
            ret.blockColor = Color3B(214, 208, 75);
            ret.fFontSize = 50;
            break;
            
        case 512:
            ret.blockColor = Color3B(228, 192, 44);
            ret.fFontSize = 50;
            break;
            
        case 1024:
            ret.blockColor = Color3B(236, 196, 0);
            ret.fFontSize = 40;
            break;
            
        case 2048:
            ret.blockColor = Color3B(236, 196, 0);
            ret.fFontSize = 40;
            break;
            
        case 4096:
            ret.blockColor = Color3B(236, 196, 0);
            ret.fFontSize = 40;
            break;
            
        default: break;
    }
    
    return ret;
}

