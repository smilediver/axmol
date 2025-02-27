/****************************************************************************
 Copyright (c) 2014-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).
 
 https://axmol.dev/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __CCANIMATE3D_H__
#define __CCANIMATE3D_H__

#include <map>
#include <unordered_map>

#include "3d/Animation3D.h"
#include "base/Macros.h"
#include "base/Object.h"
#include "2d/ActionInterval.h"

NS_AX_BEGIN

class Bone3D;
class MeshRenderer;
class EventCustom;

enum class Animate3DQuality
{
    QUALITY_NONE =
        0,  // it'll be ignore the curve-evaluating(the animation looks like stop), just accumulate transition time.
    QUALITY_LOW,   // low animation quality, it'll be more efficient.
    QUALITY_HIGH,  // high animation quality.
};

/**
 * @addtogroup _3d
 * @{
 */

/**
 * @brief Animate3D, Animates a MeshRenderer given with an Animation3D
 */
class AX_DLL Animate3D : public ActionInterval
{
public:
    /**create Animate3D using Animation.*/
    static Animate3D* create(Animation3D* animation);

    /**
     * create Animate3D
     * @param animation used to generate animate3D
     * @param fromTime
     * @param duration Time the Animate3D lasts
     * @return Animate3D created using animate
     */
    static Animate3D* create(Animation3D* animation, float fromTime, float duration);

    /**
     * create Animate3D by frame section, [startFrame, endFrame)
     * @param animation used to generate animate3D
     * @param startFrame
     * @param endFrame
     * @param frameRate default is 30 per second
     * @return Animate3D created using animate
     */
    static Animate3D* createWithFrames(Animation3D* animation, int startFrame, int endFrame, float frameRate = 30.f);

    //
    // Overrides
    //
    virtual void stop() override;
    virtual void step(float dt) override;
    virtual void startWithTarget(Node* target) override;
    virtual Animate3D* reverse() const override;
    virtual Animate3D* clone() const override;

    virtual void update(float t) override;

    /**get & set speed, negative speed means playing reverse */
    float getSpeed() const;
    void setSpeed(float speed);

    /**get & set blend weight, weight must positive*/
    float getWeight() const { return _weight; }
    void setWeight(float weight);

    /**get & set origin interval*/
    void setOriginInterval(float interval);
    float getOriginInterval() const { return _originInterval; }

    /** get animate transition time between 3d animations */
    static float getTransitionTime() { return _transTime; }

    /** set animate transition time between 3d animations */
    static void setTransitionTime(float transTime)
    {
        if (transTime >= 0.f)
            _transTime = transTime;
    }

    /**set animate quality*/
    void setQuality(Animate3DQuality quality);

    /**get animate quality*/
    Animate3DQuality getQuality() const;

    struct Animate3DDisplayedEventInfo
    {
        int frame;
        Node* target;
        const ValueMap* userInfo;
    };
    void setKeyFrameUserInfo(int keyFrame, const ValueMap& userInfo);
    const ValueMap* getKeyFrameUserInfo(int keyFrame) const;
    ValueMap* getKeyFrameUserInfo(int keyFrame);

    Animate3D();
    virtual ~Animate3D();

    void removeFromMap();

    /** init method */
    bool init(Animation3D* animation);
    bool init(Animation3D* animation, float fromTime, float duration);
    bool initWithFrames(Animation3D* animation, int startFrame, int endFrame, float frameRate);

protected:
    enum class Animate3DState
    {
        FadeIn,
        FadeOut,
        Running,
    };
    Animate3DState _state;    // animation state
    Animation3D* _animation;  // animation data

    float _absSpeed;          // playing speed
    float _weight;            // blend weight
    float _start;             // start time 0 - 1, used to generate sub Animate3D
    float _last;              // last time 0 - 1, used to generate sub Animate3D
    bool _playReverse;        // is playing reverse
    static float _transTime;  // transition time from one animate3d to another
    float _accTransTime;      // accumulate transition time
    float _lastTime;          // last t (0 - 1)
    float _originInterval;    // save origin interval time
    float _frameRate;

    // animation quality
    EvaluateType _translateEvaluate;
    EvaluateType _roteEvaluate;
    EvaluateType _scaleEvaluate;
    Animate3DQuality _quality;

    std::unordered_map<Bone3D*, Animation3D::Curve*> _boneCurves;  // weak ref
    std::unordered_map<Node*, Animation3D::Curve*> _nodeCurves;

    std::unordered_map<int, ValueMap> _keyFrameUserInfos;
    std::unordered_map<int, EventCustom*> _keyFrameEvent;
    std::unordered_map<int, Animate3DDisplayedEventInfo> _displayedEventInfo;

    // mesh animations
    static std::unordered_map<Node*, Animate3D*> s_fadeInAnimates;
    static std::unordered_map<Node*, Animate3D*> s_fadeOutAnimates;
    static std::unordered_map<Node*, Animate3D*> s_runningAnimates;
};

// end of 3d group
/// @}

NS_AX_END

#endif  // __CCANIMATE3D_H__
