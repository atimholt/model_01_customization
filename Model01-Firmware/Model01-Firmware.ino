// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details
// Modified by Tim Holt

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION                                                      \
  "\nlocally built, modified by Tim Holt\ncompiled " __DATE__ " " __TIME__ "\n"
#endif

// Options behind preprocessor guards. For quick, impermanent changes.

// For when I run into other Model01 users who touch type:
// #define CONFORM_QWERTY_THUMBS_TO_FACTORY_DEFAULT

// To better tell, at a glance, the current active layout.
// Made quickly toggleable in case it's too clashy.
// TODO figure out how to properly make it a separate color scheme (run-time
// optional)
#define DIFFERENTIATE_LETTERS_BY_COLOR

// uncomment to show off :)
// #define DO_MORE_LIGHTING


#include "Kaleidoscope.h"

#include "Kaleidoscope-HostPowerManagement.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-Model01-TestMode.h"
#include "Kaleidoscope-MouseKeys.h"

#include "Kaleidoscope-LEDControl.h"

// Lighting plugins
#include "Kaleidoscope-LEDEffect-BootGreeting.h"
#include "Kaleidoscope-LEDEffect-SolidColor.h"
#include "LED-Off.h"

#ifdef DO_MORE_LIGHTING
#include "Kaleidoscope-LED-Stalker.h"
#include "Kaleidoscope-LEDEffect-Breathe.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"
#endif

// Non-core plugins, not in this repo:
#include "Kaleidoscope-LEDEffect-FunctionalColor.h" // https://github.com/jdlien/Kaleidoscope-LEDEffect-FunctionalColor

#ifdef DO_MORE_LIGHTING
#include "Kaleidoscope-LED-Wavepool.h" //              https://github.com/ToyKeeper/Kaleidoscope-LED-Wavepool
#endif

// clang-format off

// Macros (used in a switch statement & in mappings with M() macro.)
enum { MACRO_VERSION_INFO,
       MACRO_ANY,
       MACRO_NEXT_TAB, MACRO_PREV_TAB, MACRO_DESKTOP_LEFT, MACRO_DESKTOP_RIGHT,
       MACRO_MODE_DVORAK, MACRO_MODE_GAMING, MACRO_MODE_MY_QWERTY, MACRO_MODE_QWERTY,
       MACRO_MODE_DF, MACRO_MODE_EPISTORY,
       MACRO_R_FUNCTION // For complementary function layers based on each other
     };

// Layers
enum { DVORAK, MY_QWERTY, QWERTY,
       DWARF_FORTRESS, DF_FUNCTION,
       EPISTORY, EPISTORY_FUNCTION,
       FUNCTION, NUMPAD, MY_FUNCTION_L, MY_FUNCTION_R, MY_FUNCTION_LR };

#define Key_LeftAngleBracket LSHIFT(Key_Comma)
#define Key_RightAngleBracket LSHIFT(Key_Period)

KEYMAPS(

  [DVORAK] = KEYMAP_STACKED
  (___,               Key_1,         Key_2,     Key_3,      Key_4, Key_5, Key_LEDEffectNext,
   Key_Backtick,      Key_Quote,     Key_Comma, Key_Period, Key_P, Key_Y, Key_Tab,
   M(MACRO_PREV_TAB), Key_A,         Key_O,     Key_E,      Key_U, Key_I,
   M(MACRO_NEXT_TAB), Key_Semicolon, Key_Q,     Key_J,      Key_K, Key_X, Key_Escape,
   Key_LeftGui, Key_Backspace, Key_LeftShift, Key_LeftControl,
   ShiftToLayer(MY_FUNCTION_L),

   Consumer_PlaySlashPause, Key_6, Key_7, Key_8, Key_9, Key_0, XXX,
   Key_Enter,               Key_F, Key_G, Key_C, Key_R, Key_L, Key_Slash,
                            Key_D, Key_H, Key_T, Key_N, Key_S, Key_Minus,
   Key_CapsLock,            Key_B, Key_M, Key_W, Key_V, Key_Z, Key_Equals,
   Key_RightControl, Key_RightShift, Key_Spacebar, Key_LeftAlt,
   M(MACRO_R_FUNCTION)),

  [MY_QWERTY] = KEYMAP_STACKED
  (___, ___,   ___,   ___,   ___,   ___,   ___,
   ___, Key_Q, Key_W, Key_E, Key_R, Key_T, ___,
   ___, Key_A, Key_S, Key_D, Key_F, Key_G,
   ___, Key_Z, Key_X, Key_C, Key_V, Key_B, ___,
   ___, ___, ___, ___,
   ___,

   ___, ___,   ___,   ___,       ___,        ___,           XXX,
   ___, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
        Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   ___, Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,
   ___, ___, ___, ___,
   ___),

  [QWERTY] = KEYMAP_STACKED
  (___,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,
#ifdef CONFORM_QWERTY_THUMBS_TO_FACTORY_DEFAULT
   Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
#else
   // Where I've moved the physical buttons to. The idea is, since I don’t have
   // custom keycaps, it makes sense to have a mode that actually matches the labels.
   Key_LeftGui, Key_Backspace, Key_LeftShift, Key_LeftControl,
#endif
   ShiftToLayer(FUNCTION),

   M(MACRO_ANY),  Key_6, Key_7, Key_8,     Key_9,         Key_0,         LockLayer(NUMPAD),
   Key_Enter,     Key_Y, Key_U, Key_I,     Key_O,         Key_P,         Key_Equals,
                  Key_H, Key_J, Key_K,     Key_L,         Key_Semicolon, Key_Quote,
   Key_RightAlt,  Key_N, Key_M, Key_Comma, Key_Period,    Key_Slash,     Key_Minus,
#ifdef CONFORM_QWERTY_THUMBS_TO_FACTORY_DEFAULT
   Key_RightShift, Key_LeftAlt, Key_Spacebar, Key_RightControl,
#else
   Key_RightControl, Key_RightShift, Key_Spacebar, Key_LeftAlt,
#endif
   ShiftToLayer(FUNCTION)),


   // Based on MY_QWERTY.
  [DWARF_FORTRESS] =  KEYMAP_STACKED
  (___,                   ___, ___, ___, ___, ___, ___,
   ___,                   ___, ___, ___, ___, ___, ___,
   Key_LeftAngleBracket,  ___, ___, ___, ___, ___,
   Key_RightAngleBracket, ___, ___, ___, ___, ___, ___,
   Key_LeftAlt, ___, ___, ___,
   ShiftToLayer(DF_FUNCTION),

   XXX, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, Key_RightAlt,
   ShiftToLayer(DF_FUNCTION)),

  [DF_FUNCTION] =  KEYMAP_STACKED
  (XXX,               XXX,                  XXX,           XXX,           XXX,            XXX,                   XXX,
   Key_F10,           Key_LeftCurlyBracket, Key_Home,      Key_UpArrow,   Key_PageUp,     Key_RightCurlyBracket, XXX,
   Key_mouseScrollUp, Key_LeftParen,        Key_LeftArrow, Key_Delete,    Key_RightArrow, Key_RightParen,
   Key_mouseScrollDn, Key_LeftBracket,      Key_End,       Key_DownArrow, Key_PageDown,   Key_RightBracket,      XXX,
   XXX, XXX, XXX, XXX,
   ShiftToLayer(MY_FUNCTION_LR),

   Key_KeypadNumLock, XXX, XXX,         Key_KeypadDivide, Key_KeypadMultiply, Key_KeypadSubtract, XXX,
   XXX,               XXX, Key_Keypad7, Key_Keypad8,      Key_Keypad9,        Key_KeypadAdd,      XXX,
                      XXX, Key_Keypad4, Key_Keypad5,      Key_Keypad6,        Key_KeypadEnter,    XXX,
   XXX,               XXX, Key_Keypad1, Key_Keypad2,      Key_Keypad3,        XXX,                XXX,
   ___, Key_Spacebar, Key_Keypad0, Key_KeypadDot,
   ShiftToLayer(MY_FUNCTION_LR)),

  // Epistory needs its own mode because its movement keys are weird. Also,
  // it's nice to block out the Windows key with any game, and the browser tab
  // switching macro serves less than no purpose anywhere but a browser.
  [EPISTORY] =  KEYMAP_STACKED
  (___,          ___, ___, ___, ___, ___, ___,
   ___,          ___, ___, ___, ___, ___, ___,
   Key_PageUp,   ___, ___, ___, ___, ___,
   Key_PageDown, ___, ___, ___, ___, ___, ___,
   Key_LeftAlt, ___, ___, ___,
   ShiftToLayer(EPISTORY_FUNCTION),

   XXX, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, Key_RightAlt,
   ShiftToLayer(EPISTORY_FUNCTION)),

  [EPISTORY_FUNCTION] =  KEYMAP_STACKED
  (XXX,               XXX,                  XXX,           XXX,           XXX,            XXX,                   XXX,
   Key_F10,           Key_LeftCurlyBracket, Key_Period,    Key_UpArrow,   Key_C,          Key_RightCurlyBracket, XXX,
   Key_mouseScrollUp, Key_LeftParen,        Key_LeftArrow, Key_Spacebar,  Key_RightArrow, Key_RightParen,
   Key_mouseScrollDn, Key_LeftBracket,      Key_U,         Key_DownArrow, Key_H,          Key_RightBracket,      XXX,
   XXX, XXX, XXX, XXX,
   ShiftToLayer(MY_FUNCTION_LR),

   Key_KeypadNumLock, XXX, XXX,         Key_KeypadDivide, Key_KeypadMultiply, Key_KeypadSubtract, XXX,
   XXX,               XXX, Key_Keypad7, Key_Keypad8,      Key_Keypad9,        Key_KeypadAdd,      XXX,
                      XXX, Key_Keypad4, Key_Keypad5,      Key_Keypad6,        Key_KeypadEnter,    XXX,
   XXX,               XXX, Key_Keypad1, Key_Keypad2,      Key_Keypad3,        XXX,                XXX,
   ___, Key_Spacebar, Key_Keypad0, Key_KeypadDot,
   ShiftToLayer(MY_FUNCTION_LR)),


  [FUNCTION] =  KEYMAP_STACKED
  (___,      Key_F1,           Key_F2,      Key_F3,     Key_F4,        Key_F5,           XXX,
   Key_Tab,  ___,              Key_mouseUp, ___,        Key_mouseBtnR, Key_mouseWarpEnd, Key_mouseWarpNE,
   Key_Home, Key_mouseL,       Key_mouseDn, Key_mouseR, Key_mouseBtnL, Key_mouseWarpNW,
   Key_End,  Key_PrintScreen,  Key_Insert,  ___,        Key_mouseBtnM, Key_mouseWarpSW,  Key_mouseWarpSE,
   ___, Key_Delete, ___, ___,
   ShiftToLayer(MY_FUNCTION_LR),

   Consumer_ScanPreviousTrack, Key_F6,                 Key_F7,                   Key_F8,                   Key_F9,          Key_F10,          Key_F11,
   Consumer_PlaySlashPause,    Consumer_ScanNextTrack, Key_LeftCurlyBracket,     Key_RightCurlyBracket,    Key_LeftBracket, Key_RightBracket, Key_F12,
                               Key_LeftArrow,          Key_DownArrow,            Key_UpArrow,              Key_RightArrow,  ___,              ___,
   Key_PcApplication,          Consumer_Mute,          Consumer_VolumeDecrement, Consumer_VolumeIncrement, ___,             Key_Backslash,    Key_Pipe,
   ___, ___, Key_Enter, ___,
   ShiftToLayer(MY_FUNCTION_LR)),

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

  [MY_FUNCTION_L] = KEYMAP_STACKED
  (___,                   Key_F1,                 Key_F2,        Key_F3,        Key_F4,        Key_F5,           XXX,
   Key_mouseScrollUp,     XXX,                    Key_mouseBtnR, Key_mouseUp,   Key_mouseBtnL, Key_mouseWarpEnd, Key_mouseWarpNE,
   M(MACRO_DESKTOP_LEFT), M(MACRO_DESKTOP_RIGHT), Key_mouseL,    Key_mouseBtnM, Key_mouseR,    Key_mouseWarpNW,
   Key_mouseScrollDn,     Key_PrintScreen,        Key_Insert,    Key_mouseDn,   XXX,           Key_mouseWarpSW,  Key_mouseWarpSE,
   ___, ___, ___, ___,
   XXX,

   Key_KeypadNumLock,          Key_F6,               Key_F7,      Key_F8,      Key_F9,      Key_F10,               Key_F11,
   Consumer_ScanNextTrack,     Key_LeftCurlyBracket, Key_Keypad7, Key_Keypad8, Key_Keypad9, Key_RightCurlyBracket, Key_F12,
                               Key_LeftParen,        Key_Keypad4, Key_Keypad5, Key_Keypad6, Key_RightParen,        Key_Backslash,
   Consumer_ScanPreviousTrack, Key_LeftBracket,      Key_Keypad1, Key_Keypad2, Key_Keypad3, Key_RightBracket,      Key_Pipe,
   ___, Key_Spacebar, Key_Keypad0, ___,
   ShiftToLayer(MY_FUNCTION_LR)),

  // Only used with MY_FUNCTION_L
  [MY_FUNCTION_R] = KEYMAP_STACKED
  (___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ShiftToLayer(MY_FUNCTION_LR),

   Consumer_Mute,            ___, ___,           ___,           ___,            ___, ___,
   Consumer_VolumeIncrement, ___, Key_Home,      Key_UpArrow,   Key_PageUp,     ___, ___,
                             ___, Key_LeftArrow, Key_Delete,    Key_RightArrow, ___, ___,
   Consumer_VolumeDecrement, ___, Key_End,       Key_DownArrow, Key_PageDown,   ___, ___,
   ___, Key_RightShift, Key_Spacebar, ___,
   XXX),

  [MY_FUNCTION_LR] = KEYMAP_STACKED
  (M(MACRO_VERSION_INFO), Key_F1,  Key_F2,  Key_F3,  Key_F4,  Key_F5,  XXX,
   XXX,                   Key_F11, Key_F12, Key_F13, Key_F14, Key_F15, XXX,
   XXX,                   Key_F21, Key_F22, Key_F23, Key_F24, XXX,
   XXX,                   XXX,     XXX,     XXX,     XXX,     XXX,     XXX,
   Key_LeftAlt, Key_Backspace, Key_LeftShift, Key_LeftControl,
   XXX,

   M(MACRO_ANY), Key_F6,           Key_F7,                 Key_F8,  Key_F9,  Key_F10,                 XXX,
   XXX,          Key_F16,          Key_F17,                Key_F18, Key_F19, Key_F20,                 XXX,
                 XXX,              M(MACRO_MODE_DVORAK),   XXX,     XXX,     M(MACRO_MODE_MY_QWERTY), XXX,
   XXX,          M(MACRO_MODE_DF), M(MACRO_MODE_EPISTORY), XXX,     XXX,     M(MACRO_MODE_QWERTY),    XXX,
   Key_RightControl, Key_RightShift, Key_PcApplication, Key_RightAlt,
   XXX)

) // KEYMAPS(

// clang-format on

const uint8_t DefinedLayersCount PROGMEM = sizeof(keymaps) / sizeof(*keymaps);

// Stuff plugins have asked for.
using namespace kaleidoscope::LEDFunctionalColor;

struct MyColorMap : public colorMap
{
  // My own color constants, ensuring everything is in a consistent palette.
  static constexpr cRGB defaultColor = dimgray;

  static constexpr cRGB baseColor          = lightgray;
  static constexpr cRGB subtleBaseColor1   = skyblue;
  static constexpr cRGB subtleBaseColor2   = coral;
  static constexpr cRGB attentionBaseColor = orangered;

  static constexpr cRGB secondaryColor          = teal;
  static constexpr cRGB attentionSecondaryColor = blue;

  static constexpr cRGB tertiaryColor          = orange;
  static constexpr cRGB attentionTertiaryColor = yellow;

  // shift, control, gui, and alt can all be colored by "modifier" if nocolor is
  // set here.
  static constexpr cRGB shift    = nocolor;
  static constexpr cRGB control  = nocolor;
  static constexpr cRGB gui      = attentionTertiaryColor;
  static constexpr cRGB alt      = nocolor;
  static constexpr cRGB modifier = tertiaryColor;

  static constexpr cRGB alpha       = baseColor;
  static constexpr cRGB number      = secondaryColor;
  static constexpr cRGB punctuation = tertiaryColor;

  // F1-F12 and F13-F24
  static constexpr cRGB function = attentionSecondaryColor;

  // Page Up, Page Down, Home, End, Insert, and Delete (if del has nocolor)
  static constexpr cRGB navigation = tertiaryColor;

  // Print Screen, Pause/Break, and Scroll Lock keys (brightness on Macs)
  static constexpr cRGB system = tertiaryColor;

  static constexpr cRGB arrow  = secondaryColor;
  static constexpr cRGB keypad = secondaryColor;

  // Includes play/pause, next/prev, volume control, mute, etc.
  static constexpr cRGB media = tertiaryColor;

  static constexpr cRGB mouseWheel  = nocolor;
  static constexpr cRGB mouseButton = tertiaryColor;
  static constexpr cRGB mouseWarp   = secondaryColor;
  static constexpr cRGB mouseMove   = nocolor;
  // mouse includes the four above groups if nocolor is set for those
  static constexpr cRGB mouse     = baseColor;
  static constexpr cRGB space     = baseColor;
  static constexpr cRGB tab       = tertiaryColor;
  static constexpr cRGB enter     = tertiaryColor;
  static constexpr cRGB backspace = attentionTertiaryColor;
  static constexpr cRGB escape    = attentionTertiaryColor;
  static constexpr cRGB del       = attentionTertiaryColor;

  // fn will work properly if your FUNCTION layer is between layers 1-3
  static constexpr cRGB fn = baseColor;

  // NumLock and other layer locks
  static constexpr cRGB lock          = attentionTertiaryColor;
  static constexpr cRGB LEDEffectNext = tertiaryColor;
};


FC_START_COLOR_LIST(MyColorMapOverrides)

// Unusual Macros
FC_KEYCOLOR(M(MACRO_VERSION_INFO), MyColorMap::attentionBaseColor)

// Macros
FC_GROUPKEY(M(MACRO_ANY))
FC_GROUPKEY(M(MACRO_NEXT_TAB))
FC_GROUPKEY(M(MACRO_PREV_TAB))
FC_GROUPKEY(M(MACRO_DESKTOP_LEFT))
FC_KEYCOLOR(M(MACRO_DESKTOP_RIGHT), MyColorMap::attentionTertiaryColor)

// Layers
FC_GROUPKEY(M(MACRO_MODE_DVORAK))
FC_GROUPKEY(M(MACRO_MODE_GAMING))
FC_GROUPKEY(M(MACRO_MODE_DF))
FC_GROUPKEY(M(MACRO_MODE_EPISTORY))
FC_KEYCOLOR(M(MACRO_MODE_MY_QWERTY), green)

FC_KEYCOLOR(M(MACRO_MODE_QWERTY), red)

// Function switchers
FC_GROUPKEY(ShiftToLayer(FUNCTION))
FC_GROUPKEY(ShiftToLayer(MY_FUNCTION_LR))
FC_GROUPKEY(M(MACRO_R_FUNCTION))
FC_GROUPKEY(ShiftToLayer(DF_FUNCTION))
FC_KEYCOLOR(ShiftToLayer(MY_FUNCTION_L), MyColorMap::secondaryColor)

FC_GROUPKEY(Key_LeftAngleBracket)
FC_KEYCOLOR(Key_RightAngleBracket, MyColorMap::tertiaryColor)

FC_KEYCOLOR(LockLayer(NUMPAD), MyColorMap::attentionBaseColor)

FC_KEYCOLOR(Key_CapsLock, MyColorMap::attentionTertiaryColor)

#ifdef DIFFERENTIATE_LETTERS_BY_COLOR
// Vowels
FC_GROUPKEY(Key_A)
FC_GROUPKEY(Key_E)
FC_GROUPKEY(Key_I)
FC_GROUPKEY(Key_O)
FC_KEYCOLOR(Key_U, MyColorMap::subtleBaseColor1)

// Vim directional keys
FC_GROUPKEY(Key_H)
FC_GROUPKEY(Key_J)
FC_GROUPKEY(Key_K)
FC_KEYCOLOR(Key_L, MyColorMap::subtleBaseColor2)
#endif

FC_END_COLOR_LIST


FCPlugin myColorMap(FC_COLOR_LIST(MyColorMapOverrides), 255);


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
static void clearLayers(bool skipFunctionLR = false)
{
  // Layer 0 is never turned off!
  for (uint8_t i = 1; i < DefinedLayersCount; ++i) {
    Layer.off(i);
  }
  if (!skipFunctionLR) Layer.on(MY_FUNCTION_LR);
}

/// Activates related layers together. They work together as 'modes'.
///
static void modeSwitch(uint8_t macroIndex, uint8_t keyState)
{
  /*static const PROGMEM = {}*/
  if (!keyToggledOn(keyState)) return;

  clearLayers(true);
  switch (macroIndex) {
  case MACRO_MODE_DVORAK:
    Layer.on(DVORAK); // already/still on, but whatever.
    break;
  case MACRO_MODE_GAMING:
    // TODO implement something here.
    break;
  case MACRO_MODE_MY_QWERTY:
    Layer.on(MY_QWERTY);
    break;
  case MACRO_MODE_QWERTY:
    Layer.on(QWERTY);
    break;
  case MACRO_MODE_DF:
    Layer.on(MY_QWERTY);
    Layer.on(DWARF_FORTRESS);
    break;
  case MACRO_MODE_EPISTORY:
    Layer.on(DVORAK);
    Layer.on(EPISTORY);
    break;
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
  case MACRO_VERSION_INFO:
    versionInfoMacro(keyState);
    break;

  case MACRO_ANY:
    anyKeyMacro(keyState);
    break;

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
  case MACRO_MODE_MY_QWERTY:
  case MACRO_MODE_QWERTY:
  case MACRO_MODE_DF:
  case MACRO_MODE_EPISTORY:
    modeSwitch(macroIndex, keyState);
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
static kaleidoscope::LEDSolidColor solidVeryDim(30, 30, 30);

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
  case kaleidoscope::HostPowerManagement::Sleep:
    break;
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
#ifdef DO_MORE_LIGHTING
    myColorMap,
    WavepoolEffect,
    LEDRainbowWaveEffect,
    StalkerEffect,
    solidWhite,
    solidRed,
    solidBrightOrange,
    solidYellow,
    solidGreen,
    solidBlue,
    solidIndigo,
    solidViolet,
    LEDBreatheEffect,
#else
    solidVeryDim,
    myColorMap,
#endif
    Macros,
    MouseKeys,
    HostPowerManagement);

void setup()
{
  Kaleidoscope.setup();

#ifdef DO_MORE_LIGHTING
  WavepoolEffect.idle_timeout = 2000;
  LEDRainbowWaveEffect.brightness(150);

  StalkerEffect.variant     = STALKER(BlazingTrail);
  StalkerEffect.step_length = 150;

  LEDBreatheEffect.hue = 85; // green
#endif

  FC_SET_THEME(myColorMap, MyColorMap);

  BootGreetingEffect.timeout = 5000;

  // Start with LED off in case there’s not much power.
  LEDOff.activate();
}

void loop()
{
  Kaleidoscope.loop();
}

