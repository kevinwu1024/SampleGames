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

#ifndef TypeDef_h
#define TypeDef_h

#include "cocos2d.h"
#include "SpeedCC.h"

struct SBlockUIInfo
{
    cocos2d::Color3B    blockColor;
    float               fFontSize;
    cocos2d::Color3B    fontColor;
};

struct SBlockUpdate
{
    enum UpdateType { kRemove,kMove };
    int         nID;
    UpdateType  update;
    int         nNumber;
    int         nPosX;
    int         nPosY;
};

struct SBlock
{
    int nID;
    int nNumber;
};

struct SBlockMatrix : public SpeedCC::SCComponent
{
    int     nBlockIDCounter;
    SBlock  block[4][4];
};

struct SSwipeRecogInfo : public SpeedCC::SCComponent
{
    cocos2d::Vec2    beginPos;
    bool    bDidMove;
};

struct SBlockProperty : public SpeedCC::SCComponent
{
    cocos2d::Node*   pNode;
    int     nPosX;
    int     nPosY;
    
    SBlockProperty():
    pNode(NULL),
    nPosX(0),
    nPosY(0)
    {}
};

struct SBlockRole : public SpeedCC::SCComponent
{
    int     nCounter;
};

enum ETWOKMessageType
{
    kTWOKMsgSwipeLeft = SpeedCC::SCID::kUserID + 1,
    kTWOKMsgSwipeRight,
    kTWOKMsgSwipeUp,
    kTWOKMsgSwipeDown,
    
    kTWOKMsgUpdateBlocks,
    kTWOKMsgBlocksReady,
    
    kTWOKMsgNewGame,
    kTWOKMsgGameOver,
};


enum
{
    // strategy id
    kStgyIDSwipeRecog = 65536,
    
    
    kEntityIDDirector,

	kEntityIDBlockBegin = 65536 + 1000,
};

#endif /* TypeDef_h */
