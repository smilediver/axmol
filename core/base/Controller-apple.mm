/****************************************************************************
 Copyright (c) 2014 cocos2d-x.org
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

#include "base/Controller.h"
#include "platform/PlatformConfig.h"
#if (AX_TARGET_PLATFORM == AX_PLATFORM_IOS || AX_TARGET_PLATFORM == AX_PLATFORM_MAC)

#    include "base/Macros.h"
#    include "base/EventDispatcher.h"
#    include "base/EventController.h"
#    include "base/EventListenerController.h"
#    include "base/Director.h"
#    include "2d/Label.h"

#    import <GameController/GameController.h>

typedef void (^GCControllerConnectionBlock)(GCController* controller);
typedef void (^GCControllerDisconnectionBlock)(GCController* controller);

@interface GCControllerConnectionEventHandler : NSObject {
}

@property(copy) GCControllerConnectionBlock _connectionBlock;
@property(copy) GCControllerDisconnectionBlock _disconnectionBlock;

+ (GCControllerConnectionEventHandler*)getInstance;
+ (void)destroyInstance;
@end

@implementation GCControllerConnectionEventHandler

static GCControllerConnectionEventHandler* __instance = nil;

+ (GCControllerConnectionEventHandler*)getInstance
{

    if (__instance == nil)
    {
        __instance = [[GCControllerConnectionEventHandler alloc] init];
    }
    return __instance;
}

+ (void)destroyInstance
{
    if (__instance)
    {
        [__instance release];
        __instance = nil;
    }
}

- (void)observerConnection:(GCControllerConnectionBlock)connectBlock
             disconnection:(GCControllerDisconnectionBlock)disconnectBlock
{
    self._connectionBlock    = connectBlock;
    self._disconnectionBlock = disconnectBlock;

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(onControllerConnected:)
                                                 name:GCControllerDidConnectNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(onControllerDisconnected:)
                                                 name:GCControllerDidDisconnectNotification
                                               object:nil];
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];

    // Have to reset 'copy' property to nil value to avoid memory leak.
    self._connectionBlock    = nil;
    self._disconnectionBlock = nil;

    [super dealloc];
}

- (void)onControllerConnected:(NSNotification*)connectedNotification
{
    GCController* controller = (GCController*)[connectedNotification object];

    self._connectionBlock(controller);
}

- (void)onControllerDisconnected:(NSNotification*)connectedNotification
{

    GCController* controller = (GCController*)[connectedNotification object];
    self._disconnectionBlock(controller);
}

@end

NS_AX_BEGIN

class ControllerImpl
{
public:
    ControllerImpl(Controller* controller) : _controller(controller), _gcController(nil) {}

    Controller* _controller;
    GCController* _gcController;
};

void Controller::startDiscoveryController()
{
    if (NSClassFromString(@"GCController") == nil)
    {
        return;
    }
    [GCController startWirelessControllerDiscoveryWithCompletionHandler:nil];

    [[GCControllerConnectionEventHandler getInstance]
        observerConnection:^(GCController* gcController) {
          auto controller                  = new Controller();
          controller->_impl->_gcController = gcController;

          // get the player index
          GCControllerPlayerIndex playerIndex = gcController.playerIndex;
          if (playerIndex == GCControllerPlayerIndexUnset) {
              // apple support up to 4 players
              // ref: https://developer.apple.com/documentation/gamecontroller/gccontrollerplayerindex
              for (int i = 0; i < 4; ++i) {
                  bool indexInUse = false;

                  for (GCController *existingController in [GCController controllers]) {
                      if (existingController.playerIndex == i) {
                          indexInUse = true;
                          break;
                      }
                  }

                  if (!indexInUse) {
                      gcController.playerIndex = static_cast<GCControllerPlayerIndex>(i);
                      playerIndex = static_cast<GCControllerPlayerIndex>(i);
                      break;
                  }
              }
          }

          controller->_deviceId = static_cast<int>(playerIndex);
          controller->_deviceName = [gcController.vendorName UTF8String];

          s_allController.push_back(controller);

          controller->registerListeners();
          controller->getDeviceName();

          controller->onConnected();
        }
        disconnection:^(GCController* gcController) {
          auto iter = std::find_if(s_allController.begin(), s_allController.end(),
                                   [gcController](Controller* c) { return c->_impl->_gcController == gcController; });

          if (iter == s_allController.end())
          {
              AXLOGW("disconnect:Could not find the controller");
              return;
          }

          (*iter)->onDisconnected();
          s_allController.erase(iter);
        }];
}

void Controller::stopDiscoveryController()
{
    if (NSClassFromString(@"GCController") == nil)
    {
        return;
    }
    [GCController stopWirelessControllerDiscovery];
}

Controller::Controller()
    : _deviceId(0)
    , _controllerTag(TAG_UNSET)
    , _impl(new ControllerImpl(this))
    , _connectEvent(nullptr)
    , _keyEvent(nullptr)
    , _axisEvent(nullptr)
{
    init();
}

Controller::~Controller()
{
    delete _impl;

    delete _connectEvent;
    delete _keyEvent;
    delete _axisEvent;
}

void Controller::registerListeners()
{
    if (_impl->_gcController.extendedGamepad != nil)
    {
        _impl->_gcController.extendedGamepad.dpad.up.valueChangedHandler =
            ^(GCControllerButtonInput* button, float value, BOOL pressed) {
              onButtonEvent(Key::BUTTON_DPAD_UP, pressed, value, button.isAnalog);
            };

        _impl->_gcController.extendedGamepad.dpad.down.valueChangedHandler =
            ^(GCControllerButtonInput* button, float value, BOOL pressed) {
              onButtonEvent(Key::BUTTON_DPAD_DOWN, pressed, value, button.isAnalog);
            };

        _impl->_gcController.extendedGamepad.dpad.left.valueChangedHandler =
            ^(GCControllerButtonInput* button, float value, BOOL pressed) {
              onButtonEvent(Key::BUTTON_DPAD_LEFT, pressed, value, button.isAnalog);
            };

        _impl->_gcController.extendedGamepad.dpad.right.valueChangedHandler =
            ^(GCControllerButtonInput* button, float value, BOOL pressed) {
              onButtonEvent(Key::BUTTON_DPAD_RIGHT, pressed, value, button.isAnalog);
            };

        _impl->_gcController.extendedGamepad.leftThumbstick.xAxis.valueChangedHandler =
            ^(GCControllerAxisInput* axis, float value) {
              onAxisEvent(Key::JOYSTICK_LEFT_X, value, axis.isAnalog);
            };

        _impl->_gcController.extendedGamepad.leftThumbstick.yAxis.valueChangedHandler =
            ^(GCControllerAxisInput* axis, float value) {
              onAxisEvent(Key::JOYSTICK_LEFT_Y, -value, axis.isAnalog);
            };

        _impl->_gcController.extendedGamepad.rightThumbstick.xAxis.valueChangedHandler =
            ^(GCControllerAxisInput* axis, float value) {
              onAxisEvent(Key::JOYSTICK_RIGHT_X, value, axis.isAnalog);
            };

        _impl->_gcController.extendedGamepad.rightThumbstick.yAxis.valueChangedHandler =
            ^(GCControllerAxisInput* axis, float value) {
              onAxisEvent(Key::JOYSTICK_RIGHT_Y, -value, axis.isAnalog);
            };

        _impl->_gcController.extendedGamepad.valueChangedHandler = ^(GCExtendedGamepad* gamepad,
                                                                     GCControllerElement* element) {
          if (element == gamepad.buttonA)
          {
              onButtonEvent(Key::BUTTON_A, gamepad.buttonA.isPressed, gamepad.buttonA.value, gamepad.buttonA.isAnalog);
          }
          else if (element == gamepad.buttonB)
          {
              onButtonEvent(Key::BUTTON_B, gamepad.buttonB.isPressed, gamepad.buttonB.value, gamepad.buttonB.isAnalog);
          }
          else if (element == gamepad.buttonX)
          {
              onButtonEvent(Key::BUTTON_X, gamepad.buttonX.isPressed, gamepad.buttonX.value, gamepad.buttonX.isAnalog);
          }
          else if (element == gamepad.buttonY)
          {
              onButtonEvent(Key::BUTTON_Y, gamepad.buttonY.isPressed, gamepad.buttonY.value, gamepad.buttonY.isAnalog);
          }
          else if (element == gamepad.leftShoulder)
          {
              onButtonEvent(Key::BUTTON_LEFT_SHOULDER, gamepad.leftShoulder.isPressed, gamepad.leftShoulder.value,
                            gamepad.leftShoulder.isAnalog);
          }
          else if (element == gamepad.rightShoulder)
          {
              onButtonEvent(Key::BUTTON_RIGHT_SHOULDER, gamepad.rightShoulder.isPressed, gamepad.rightShoulder.value,
                            gamepad.rightShoulder.isAnalog);
          }
          else if (element == gamepad.leftTrigger)
          {
              onAxisEvent(Key::AXIS_LEFT_TRIGGER, gamepad.leftTrigger.value, gamepad.leftTrigger.isAnalog);
          }
          else if (element == gamepad.rightTrigger)
          {
              onAxisEvent(Key::AXIS_RIGHT_TRIGGER, gamepad.rightTrigger.value, gamepad.rightTrigger.isAnalog);
          }
        };
    }
#    if defined(AX_TARGET_OS_TVOS)
    else if (_impl->_gcController.microGamepad != nil)
    {
        _impl->_gcController.microGamepad.dpad.up.valueChangedHandler =
            ^(GCControllerButtonInput* button, float value, BOOL pressed) {
              onButtonEvent(Key::BUTTON_DPAD_UP, pressed, value, button.isAnalog);
            };

        _impl->_gcController.microGamepad.dpad.down.valueChangedHandler =
            ^(GCControllerButtonInput* button, float value, BOOL pressed) {
              onButtonEvent(Key::BUTTON_DPAD_DOWN, pressed, value, button.isAnalog);
            };

        _impl->_gcController.microGamepad.dpad.left.valueChangedHandler =
            ^(GCControllerButtonInput* button, float value, BOOL pressed) {
              onButtonEvent(Key::BUTTON_DPAD_LEFT, pressed, value, button.isAnalog);
            };

        _impl->_gcController.microGamepad.dpad.right.valueChangedHandler =
            ^(GCControllerButtonInput* button, float value, BOOL pressed) {
              onButtonEvent(Key::BUTTON_DPAD_RIGHT, pressed, value, button.isAnalog);
            };

        _impl->_gcController.microGamepad.valueChangedHandler = ^(GCMicroGamepad* gamepad,
                                                                  GCControllerElement* element) {
          if (element == gamepad.buttonA)
          {
              onButtonEvent(Key::BUTTON_A, gamepad.buttonA.isPressed, gamepad.buttonA.value, gamepad.buttonA.isAnalog);
          }
          else if (element == gamepad.buttonX)
          {
              onButtonEvent(Key::BUTTON_X, gamepad.buttonX.isPressed, gamepad.buttonX.value, gamepad.buttonX.isAnalog);
          }
        };
    }
#    endif

    _impl->_gcController.controllerPausedHandler = ^(GCController* gcCon) {
      auto iter = std::find_if(s_allController.begin(), s_allController.end(),
                               [gcCon](Controller* c) { return c->_impl->_gcController == gcCon; });

      if (iter == s_allController.end())
      {
          AXLOGW("Could not find the controller");
          return;
      }

      onButtonEvent(Key::BUTTON_PAUSE, true, 1.0f, false);
      onButtonEvent(Key::BUTTON_PAUSE, false, 0.0f, false);
    };
}

bool Controller::isConnected() const
{
    return _impl->_gcController.isAttachedToDevice == YES;
}

void Controller::receiveExternalKeyEvent(int externalKeyCode, bool receive) {}

NS_AX_END

#endif  // #if (AX_TARGET_PLATFORM == AX_PLATFORM_IOS)
