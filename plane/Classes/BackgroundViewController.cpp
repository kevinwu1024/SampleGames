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

#include "BackgroundViewController.h"
#include "TypeDef.h"

using namespace cocos2d;
using namespace SpeedCC;

void BackgroundViewController::onCreate(SpeedCC::SCDictionary parameters)
{
    SCViewController::onCreate(parameters);
    
	auto imageArray = parameters["image"].getArray();
	auto fSpeed = parameters["speed"].getFloat();

    SC_BEGIN_PARENT_ROOT(nullptr,nullptr,SCWinSize())
        for(int i=0; i<SC_ARRAY_LENGTH(_moveSpriteArray); ++i)
        {
			auto strImage = imageArray[i].getString();
            SC_INSERT_SPRITE(&_moveSpriteArray[i], "x=0.5; y=0.5;", strImage)
        }
    SC_END_PARENT
    
    _nMapHeight = _moveSpriteArray[0]->getContentSize().height;
    
    this->reset();
    
    _fSpeed = fSpeed;
    _bRunning = false;
}

void BackgroundViewController::setMapIndex(int nIndex)
{
    _nMapIndex = nIndex;
    
    auto sprite = SCNodeUtils::createSprite(SCString("bg_level%d.jpg",_nMapIndex+1));
    
    for(int i=0; i<SC_ARRAY_LENGTH(_moveSpriteArray); ++i)
    {
        _moveSpriteArray[i]->setSpriteFrame(sprite->getSpriteFrame());
    }
}

void BackgroundViewController::setSpeed(float fSpeed)
{
    _fSpeed = fSpeed;
}

void BackgroundViewController::onFrame(float fDelta)
{
    SC_RETURN_V_IF(!_bRunning);
    
    if(_moveSpriteArray[_nCurIndex]->getPositionY() <= -_nMapHeight/2)
    {
        int nNextNextBgIndex = (_nCurIndex + 2) % 3;
        _moveSpriteArray[_nCurIndex]->setPositionY(_moveSpriteArray[nNextNextBgIndex]->getPositionY()+_nMapHeight);
        
        if(++_nCurIndex == SC_ARRAY_LENGTH(_moveSpriteArray))
        {
            _nCurIndex = 0;
        }
    }
    
    for(int i=0; i<SC_ARRAY_LENGTH(_moveSpriteArray); ++i)
    {
        _moveSpriteArray[i]->setPositionY(_moveSpriteArray[i]->getPositionY() - _fSpeed);
    }
}

void BackgroundViewController::run()
{
    SC_RETURN_V_IF(_bRunning);
    this->schedule(SC_FUNC(BackgroundViewController::onFrame));
    _bRunning = true;
}

void BackgroundViewController::pause()
{
    SC_RETURN_V_IF(!_bRunning);
    this->unschedule(SC_FUNC(BackgroundViewController::onFrame));
    _bRunning = false;
}

void BackgroundViewController::reset()
{
    _nCurIndex = 0;
    _moveSpriteArray[_nCurIndex]->setPositionY(0);
    _moveSpriteArray[_nCurIndex+1]->setPositionY(_nMapHeight);
    _moveSpriteArray[_nCurIndex+2]->setPositionY(_nMapHeight*2);
}
