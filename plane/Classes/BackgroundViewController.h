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

#ifndef BackgroundViewController_hpp
#define BackgroundViewController_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "SpeedCC.h"

///---------- BackgroundViewController
class BackgroundViewController : public SpeedCC::SCViewController
{
public:
    SC_DEFINE_CLASS_PTR(BackgroundViewController)
    
    virtual void onCreate(SpeedCC::SCDictionary parameters) override;
    
    void run();
    void pause();
    void reset();
    void setMapIndex(int nIndex);
    
    void setSpeed(float fSpeed);
    
    inline bool isRunning() const { return _bRunning; }
    
private:
    void onFrame(float fDelta);
    
private:
    cocos2d::Sprite*    _moveSpriteArray[3];
    bool                _bRunning;
    int                 _nCurIndex;
    float               _fSpeed;
    int                 _nMapHeight;
    int                 _nMapIndex;
};



#endif /* BackgroundViewController_hpp */
