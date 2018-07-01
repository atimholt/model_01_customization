// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details
// Modified by Tim Holt

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built, modified by Tim Holt"
#endif


#include "Kaleidoscope.h"

#include "Kaleidoscope-HostPowerManagement.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-Model01-TestMode.h"
#include "Kaleidoscope-MouseKeys.h"

#include "Kaleidoscope-LEDControl.h"

// Lighting plugins
#include "Kaleidoscope-LED-Stalker.h"
#include "Kaleidoscope-LEDEffect-BootGreeting.h"
#include "Kaleidoscope-LEDEffect-Breathe.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"
#include "Kaleidoscope-LEDEffect-SolidColor.h"
#include "Kaleidoscope-NumPad.h"
#include "LED-Off.h"

// Non-core plugins, not in this repo:
#include "Kaleidoscope-LED-Wavepool.h" //              https://github.com/ToyKeeper/Kaleidoscope-LED-Wavepool
#include "Kaleidoscope-LEDEffect-FunctionalColor.h" // https://github.com/jdlien/Kaleidoscope-LEDEffect-FunctionalColor

// Stuff plugins have asked for.
kaleidoscope::LEDFunctionalColor::FCPlugin funColor;

// clang-format off

// Macros (used in a switch statement & in mappings with M() macro.)
enum { MACRO_VERSION_INFO,
       MACRO_ANY,
       MACRO_NEXT_TAB, MACRO_PREV_TAB, MACRO_DESKTOP_LEFT, MACRO_DESKTOP_RIGHT,
       MACRO_MODE_DVORAK, MACRO_MODE_GAMING, MACRO_MODE_QWERTY,
       SWITCHER_BUTTON,
       MACRO_R_FUNCTION // For complementary function layers based on each other
     };

// Layers
enum { DVORAK, QWERTY,
       MY_FUNCTION_L, MY_FUNCTION_R, FUNCTION, NUMPAD,
       SWITCHER};

KEYMAPS(

  [DVORAK] = KEYMAP_STACKED
  (___,               Key_1,         Key_2,     Key_3,      Key_4, Key_5, Key_LEDEffectNext,
   Key_Backtick,      Key_Quote,     Key_Comma, Key_Period, Key_P, Key_Y, Key_Tab,
   M(MACRO_PREV_TAB), Key_A,         Key_O,     Key_E,      Key_U, Key_I,
   M(MACRO_NEXT_TAB), Key_Semicolon, Key_Q,     Key_J,      Key_K, Key_X, Key_Escape,
   Key_LeftGui, Key_Backspace, Key_LeftShift, Key_LeftControl,
   ShiftToLayer(MY_FUNCTION_L),

   Consumer_PlaySlashPause, Key_6, Key_7, Key_8, Key_9, Key_0, LockLayer(SWITCHER),
   Key_Enter,               Key_F, Key_G, Key_C, Key_R, Key_L, Key_Slash,
                            Key_D, Key_H, Key_T, Key_N, Key_S, Key_Minus,
   Key_CapsLock,            Key_B, Key_M, Key_W, Key_V, Key_Z, Key_Equals,
   Key_RightControl, Key_RightShift, Key_Spacebar, Key_LeftAlt,
   M(MACRO_R_FUNCTION)),

  [QWERTY] = KEYMAP_STACKED
  (___,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,
   Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
   ShiftToLayer(FUNCTION),

   M(MACRO_ANY),  Key_6, Key_7, Key_8,     Key_9,         Key_0,         LockLayer(SWITCHER),
   Key_Enter,     Key_Y, Key_U, Key_I,     Key_O,         Key_P,         Key_Equals,
                  Key_H, Key_J, Key_K,     Key_L,         Key_Semicolon, Key_Quote,
   Key_RightAlt,  Key_N, Key_M, Key_Comma, Key_Period,    Key_Slash,     Key_Minus,
   Key_RightShift, Key_LeftAlt, Key_Spacebar, Key_RightControl,
   ShiftToLayer(FUNCTION)),


  [MY_FUNCTION_L] = KEYMAP_STACKED
  (___,                   Key_F1,                 Key_F2,        Key_F3,        Key_F4,        Key_F5,           XXX,
   Key_Tab,               XXX,                    Key_mouseBtnR, Key_mouseUp,   Key_mouseBtnL, Key_mouseWarpEnd, Key_mouseWarpNE,
   M(MACRO_DESKTOP_LEFT), M(MACRO_DESKTOP_RIGHT), Key_mouseL,    Key_mouseBtnM, Key_mouseR,    Key_mouseWarpNW,
   XXX,                   Key_PrintScreen,        Key_Insert,    Key_mouseDn,   XXX,           Key_mouseWarpSW,  Key_mouseWarpSE,
   ___, ___, ___, ___,
   XXX,

   Key_KeypadNumLock,          Key_F6,               Key_F7,      Key_F8,      Key_F9,      Key_F10,               Key_F11,
   Consumer_ScanNextTrack,     Key_LeftBracket,      Key_Keypad7, Key_Keypad8, Key_Keypad9, Key_RightBracket,      Key_F12,
                               LSHIFT(Key_9),        Key_Keypad4, Key_Keypad5, Key_Keypad6, LSHIFT(Key_0),         Key_Backslash,
   Consumer_ScanPreviousTrack, Key_LeftCurlyBracket, Key_Keypad1, Key_Keypad2, Key_Keypad3, Key_RightCurlyBracket, Key_Pipe,
   ___, Key_Spacebar, Key_Keypad0, ___,
   XXX),

  // Only used with MY_FUNCTION_L
  [MY_FUNCTION_R] = KEYMAP_STACKED
  (___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   XXX,

   Consumer_Mute,            ___, ___,           ___,           ___,            ___, ___,
   Consumer_VolumeIncrement, ___, Key_Home,      Key_UpArrow,   Key_PageUp,     ___, ___,
                             ___, Key_LeftArrow, Key_Delete,    Key_RightArrow, ___, ___,
   Consumer_VolumeDecrement, ___, Key_End,       Key_DownArrow, Key_PageDown,   ___, ___,
   ___, ___, Key_Spacebar, ___,
   XXX),


  [FUNCTION] =  KEYMAP_STACKED
  (___,      Key_F1,           Key_F2,      Key_F3,     Key_F4,        Key_F5,           XXX,
   Key_Tab,  ___,              Key_mouseUp, ___,        Key_mouseBtnR, Key_mouseWarpEnd, Key_mouseWarpNE,
   Key_Home, Key_mouseL,       Key_mouseDn, Key_mouseR, Key_mouseBtnL, Key_mouseWarpNW,
   Key_End,  Key_PrintScreen,  Key_Insert,  ___,        Key_mouseBtnM, Key_mouseWarpSW,  Key_mouseWarpSE,
   ___, Key_Delete, ___, ___,
   ___,

   Consumer_ScanPreviousTrack, Key_F6,                 Key_F7,                   Key_F8,                   Key_F9,          Key_F10,          Key_F11,
   Consumer_PlaySlashPause,    Consumer_ScanNextTrack, Key_LeftCurlyBracket,     Key_RightCurlyBracket,    Key_LeftBracket, Key_RightBracket, Key_F12,
                               Key_LeftArrow,          Key_DownArrow,            Key_UpArrow,              Key_RightArrow,  ___,              ___,
   Key_PcApplication,          Consumer_Mute,          Consumer_VolumeDecrement, Consumer_VolumeIncrement, ___,             Key_Backslash,    Key_Pipe,
   ___, ___, Key_Enter, ___,
   ___),

  [NUMPAD] =  KEYMAP_STACKED
  (___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ___,

   M(MACRO_VERSION_INFO),  ___, Key_Keypad7, Key_Keypad8,   Key_Keypad9,        Key_KeypadSubtract, LockLayer(NUMPAD),
   ___,                    ___, Key_Keypad4, Key_Keypad5,   Key_Keypad6,        Key_KeypadAdd,      ___,
                           ___, Key_Keypad1, Key_Keypad2,   Key_Keypad3,        Key_Equals,         ___,
   ___,                    ___, Key_Keypad0, Key_KeypadDot, Key_KeypadMultiply, Key_KeypadDivide,   Key_Enter,
   ___, ___, ___, ___,
   ___),


  [SWITCHER] =  KEYMAP_STACKED
  (___, M(MACRO_MODE_DVORAK), M(MACRO_MODE_GAMING), XXX, XXX, XXX, XXX,
   XXX, XXX, XXX, XXX, XXX, XXX, XXX,
   XXX, XXX, XXX, XXX, XXX, XXX,
   XXX, XXX, XXX, XXX, XXX, XXX, XXX,
   XXX, XXX, XXX, XXX,
   XXX,

   XXX, XXX, XXX, XXX, XXX, M(MACRO_MODE_QWERTY), M(SWITCHER_BUTTON),
   XXX, XXX, XXX, XXX, XXX, XXX, XXX,
        XXX, XXX, XXX, XXX, XXX, XXX,
   XXX, XXX, XXX, XXX, XXX, XXX, XXX,
   XXX, XXX, XXX, XXX,
   XXX)

) // KEYMAPS(

// clang-format on

const uint8_t DefinedLayersCount PROGMEM = sizeof(keymaps) / sizeof(*keymaps);

/// When a key bound to the macro is pressed, this macro
/// prints out the firmware build information as virtual keystrokes
///
static void versionInfoMacro(uint8_t keyState)
{
  if (keyToggledOn(keyState)) {
    Macros.type(PSTR("Keyboardio Model 01 - Kaleidoscope "));
    Macros.type(PSTR(BUILD_INFORMATION));
  }
}

/// When the 'any key' macro is toggled on, a random alphanumeric key is
/// selected. While the key is held, the function generates a synthetic
/// keypress event repeating that randomly selected key.
///
static void anyKeyMacro(uint8_t keyState)
{
  static Key lastKey;
  if (keyToggledOn(keyState))
    lastKey.keyCode = Key_A.keyCode + (uint8_t)(millis() % 36);

  if (keyIsPressed(keyState)) kaleidoscope::hid::pressKey(lastKey);
}

/// Deactivates all but layer 0.
///
static void clearLayers()
{
  // Layer 0 is never turned off!
  for (uint8_t i = 1; i < DefinedLayersCount; ++i) {
    Layer.off(i);
  }
}

/// Activates related layers together. They work together as 'modes'.
///
static void modeSwitch(uint8_t macroIndex, uint8_t keyState)
{
  /*static const PROGMEM = {}*/
  if (!keyToggledOn(keyState)) return;

  clearLayers();
  switch (macroIndex) {
  case MACRO_MODE_DVORAK:
    Layer.on(DVORAK); // already/still on, but whatever.
    break;
  case MACRO_MODE_GAMING: break;
  case MACRO_MODE_QWERTY: Layer.on(QWERTY); break;
  }
}


/// Dispatches keymap events that are tied to a macro to that macro. It takes
/// two uint8_t parameters.
///
/// The first is the macro being called (the entry in the 'enum' earlier in this
/// file). The second is the state of the keyswitch. You can use the keyswitch
/// state to figure out if the key has just been toggled on, is currently
/// pressed or if it's just been released.
///
/// The 'switch' statement should have a 'case' for each entry of the macro
/// enum. Each 'case' statement should call out to a function to handle the
/// macro in question.
///
const macro_t* macroAction(uint8_t macroIndex, uint8_t keyState)
{
  switch (macroIndex) {
  case MACRO_VERSION_INFO: versionInfoMacro(keyState); break;

  case MACRO_ANY: anyKeyMacro(keyState); break;

  case MACRO_NEXT_TAB:
    if (keyToggledOn(keyState))
      return MACRO(D(LeftControl), T(Tab), U(LeftControl));
    break;

  case MACRO_PREV_TAB:
    if (keyToggledOn(keyState))
      return MACRO(
          D(LeftControl), D(LeftShift), T(Tab), U(LeftShift), U(LeftControl));
    break;

  case MACRO_DESKTOP_LEFT:
    if (keyToggledOn(keyState))
      return MACRO(
          D(LeftGui), D(LeftControl), T(LeftArrow), U(LeftControl), U(LeftGui));
    break;

  case MACRO_DESKTOP_RIGHT:
    if (keyToggledOn(keyState))
      return MACRO(D(LeftGui), D(LeftControl), T(RightArrow), U(LeftControl),
          U(LeftGui));
    break;

  // All the modes
  case MACRO_MODE_DVORAK:
  case MACRO_MODE_GAMING:
  case MACRO_MODE_QWERTY: modeSwitch(macroIndex, keyState); break;

  case SWITCHER_BUTTON:
    // TODO  make it more obvious that this is a mode thing, couple it together.
    if (keyToggledOn(keyState) && Layer.isOn(QWERTY)) {
      clearLayers();
      Layer.on(QWERTY);
      Layer.on(NUMPAD);
    }
    break;

  case MACRO_R_FUNCTION:
    if (keyIsPressed(keyState)) {
      return MACRO(
          Dr(ShiftToLayer(MY_FUNCTION_L)), Dr(ShiftToLayer(MY_FUNCTION_R)));
    }
    else {
      return MACRO(
          Ur(ShiftToLayer(MY_FUNCTION_L)), Ur(ShiftToLayer(MY_FUNCTION_R)));
    }
    break;
  }
  return MACRO_NONE;
}


// These 'solid' color effect definitions define a rainbow of
// LED color modes calibrated to draw 500mA or less on the
// Keyboardio Model 01.


static kaleidoscope::LEDSolidColor solidRed(160, 0, 0);
static kaleidoscope::LEDSolidColor solidOrange(140, 70, 0);
static kaleidoscope::LEDSolidColor solidBrightOrange(168, 84, 0);
static kaleidoscope::LEDSolidColor solidYellow(130, 100, 0);
static kaleidoscope::LEDSolidColor solidGreen(0, 160, 0);
static kaleidoscope::LEDSolidColor solidBlue(0, 70, 130);
static kaleidoscope::LEDSolidColor solidIndigo(0, 0, 170);
static kaleidoscope::LEDSolidColor solidViolet(130, 0, 120);
static kaleidoscope::LEDSolidColor solidWhite(100, 100, 100);

/// Toggles the LEDs off when the host goes to sleep, and turns them back on
/// when it wakes up.
///
/// TODO  Determine if this ever does anything or if it's obsolete.
///
void toggleLedsOnSuspendResume(kaleidoscope::HostPowerManagement::Event event)
{
  switch (event) {
  case kaleidoscope::HostPowerManagement::Suspend:
    LEDControl.paused = true;
    LEDControl.set_all_leds_to({0, 0, 0});
    LEDControl.syncLeds();
    break;
  case kaleidoscope::HostPowerManagement::Resume:
    LEDControl.paused = false;
    LEDControl.refreshAll();
    break;
  case kaleidoscope::HostPowerManagement::Sleep: break;
  }
}

/// Dispatches power management events (suspend, resume, and sleep) to other
/// functions that perform action based on these events.
///
void hostPowerManagementEventHandler(
    kaleidoscope::HostPowerManagement::Event event)
{
  toggleLedsOnSuspendResume(event);
}

KALEIDOSCOPE_INIT_PLUGINS(BootGreetingEffect,
    TestMode,
    LEDControl,
    LEDOff,
    funColor,
    WavepoolEffect,
    LEDRainbowEffect,
    LEDRainbowWaveEffect,
    solidWhite,
    solidRed,
    solidBrightOrange,
    solidGreen,
    solidBlue,
    LEDBreatheEffect,
    StalkerEffect,
    NumPad,
    Macros,
    MouseKeys,
    HostPowerManagement);

void setup()
{
  Kaleidoscope.setup();

  // While we hope to improve this in the future, the NumPad plugin
  // needs to be explicitly told which keymap layer is your numpad layer
  // TODO  Determine whether this has happened.
  NumPad.numPadLayer = NUMPAD;

  WavepoolEffect.idle_timeout = 2000;

  LEDRainbowEffect.brightness(150);
  LEDRainbowWaveEffect.brightness(150);

  StalkerEffect.variant = STALKER(BlazingTrail);

  // We want to make sure that the firmware starts with LED effects off
  // This avoids over-taxing devices that don't have a lot of power to share
  // with USB devices
  LEDOff.activate();
}

void loop()
{
  Kaleidoscope.loop();
}

