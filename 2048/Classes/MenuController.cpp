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

#include "MenuController.h"
#include "TypeDef.h"

using namespace cocos2d;
using namespace SpeedCC;

void MenuController::onCreate(SpeedCC::SCDictionary parameters)
{
    SCViewController::onCreate(parameters);
    
    auto ptrBackSceneBvr = SCBehaviorViewBack::create();
    auto ptrNewGameBvr = SC_BVR_GROUP(ptrBackSceneBvr,
                                 SCBehaviorDeliverMessage::create(false,SCMessage::create(kTWOKMsgNewGame)));
    
    SC_BEGIN_PARENT_ROOT(nullptr,nullptr,SCWinSize())
        SC_BEGIN_PARENT_LAYER_COLOR(nullptr,"dock=center;",SCWinSize(),Color4B::WHITE)
    
            SC_INSERT_LABEL(nullptr, "x=0.5; y=0.9; text-color=(120,110,100);", "Menu", "", 50)
    
            SC_INSERT_SPRITE(nullptr, "x=0.5; y=0.75; scale-x=1.5; scale-y=0.5; color=(187,173,160);", "block.png")
            SC_INSERT_BUTTON_LABEL(nullptr, "x=0.5; y=0.75;", "KEEP GOING", "", 20, ptrBackSceneBvr)
    
            SC_INSERT_SPRITE(nullptr, "x=0.5; y=0.6; scale-x=1.5; scale-y=0.5; color=(187,173,160);", "block.png")
            SC_INSERT_BUTTON_LABEL(nullptr, "x=0.5; y=0.6;", "NEW GAME", "", 20, ptrNewGameBvr)
    
            SC_INSERT_OPTION_SOUND(nullptr,"x=0.5; y=0.45; scale=0.5;","btn-sound-on.png","btn-sound-off.png")
        SC_END_PARENT
    
    SC_END_PARENT
}

///------------- GameOverController
void GameOverController::onCreate(SpeedCC::SCDictionary parameters)
{
    SCViewController::onCreate(parameters);
    
    SCWatchInt::Ptr ptrScore = parameters.getValue("score").getObject<SCWatchInt::Ptr>();
    auto ptrBackSceneBvr = SCBehaviorViewBack::create();
    
    SC_BEGIN_PARENT_ROOT(nullptr,nullptr,SCWinSize())
        SC_BEGIN_PARENT_LAYER_COLOR(nullptr,"dock=center;",SCWinSize(),Color4B::WHITE)
    
            SC_INSERT_LABEL(nullptr,"x=0.5; y=0.9; text-color=(120,110,100);", "Game Over!", "", 50)
    
            SC_INSERT_SPRITE(nullptr,"x=0.5; y=0.75;  scale-x=1.5; scale-y=0.5; color=(187,173,160);", "block.png")
            SC_INSERT_LABEL(nullptr,"x=0.5; y=0.775;" , "score", "", 18)
            SC_INSERT_LABEL(nullptr,"x=0.5; y=0.726; ", ptrScore, "", 23)
    
            SC_INSERT_SPRITE(nullptr, "x=0.5; y=0.6; scale-x=1.5; scale-y=0.5; color=(187,173,160);", "block.png")
            SC_INSERT_BUTTON_LABEL(nullptr, "x=0.5; y=0.6;", "TRY AGAIN", "", 20, ptrBackSceneBvr)
    
        SC_END_PARENT
    SC_END_PARENT
}
