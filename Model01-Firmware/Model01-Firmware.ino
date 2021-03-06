/// \file  Model01-Firmware.ino
///
/// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
/// See "LICENSE" for license details
///
/// Modified by Tim Holt
///
/// Hardware Info:
/// Processor: ATMEGA32U4
///
/// | Board Name          | FQBN                            | Core           |
/// |---------------------|---------------------------------|----------------|
/// | Keyboardio Imago    | keyboardio:avr:keyboardio_imago | keyboardio:avr |
/// | Keyboardio Model 01 | keyboardio:avr:model01          | keyboardio:avr |
///
/// (above info derived from the command "`arduino-cli board list`". I'm not
/// sure of the (non)significance of the imago, so I'm leaving it here for now.
///
/// (in arduino-cli.yaml)
/// ```yaml
/// board_manager:
///    additional_urls:
///     - https://raw.githubusercontent.com/keyboardio/boardsmanager/master/package_keyboardio_index.json
/// ```

// Includes
//----------

#include <stdint.h>

#include <Kaleidoscope.h>

//   Plugins
//  ---------

//     Core Functionality
//    --------------------

#include <Kaleidoscope-HardwareTestMode.h>
#include <Kaleidoscope-Macros.h>

//       Regarding lighting
//      --------------------

#include <Kaleidoscope-LEDControl.h>

#include <Kaleidoscope-HostPowerManagement.h> // Turn off LEDs during sleep.
#include <Kaleidoscope-LEDEffect-BootGreeting.h> // "it's working" pulsing light.

//       Helper(s)
//      -----------

#include <Kaleidoscope-MagicCombo.h>
#include <Kaleidoscope-USB-Quirks.h>

//     Lighting
//    ----------

#include <Kaleidoscope-LED-Wavepool.h> // Now comes BUNDLED!! :D
#include <Kaleidoscope-LEDEffect-Breathe.h>
#include <Kaleidoscope-LEDEffect-Rainbow.h>
#include <Kaleidoscope-LEDEffect-SolidColor.h>

// I really really hate Arduino. I can't wait to de-Arduino this whole thing.
// (I couldn't make this plugin into a git submodule no matter what I tried.)
#include <Kaleidoscope-LEDEffect-FunctionalColor.h>

//     Augmented Functionality
//    -------------------------

#include <Kaleidoscope-TopsyTurvy.h> // Shifted keys, unshifted by shift.

// Defines
//---------

#if defined(__cplusplus)
  //199711L(until C++11), 201103L(C++11), 201402L(C++14), 201703L(C++17), 202002L(C++20)
  #if __cplusplus   == 199711
    #define CPLUSPLUS_VERSION "<C++11"
  #elif __cplusplus == 201103
    #define CPLUSPLUS_VERSION "C++11"
  #elif __cplusplus == 201402
    #define CPLUSPLUS_VERSION "C++14"
  #elif __cplusplus == 201703
    #define CPLUSPLUS_VERSION "C++17"
  #elif __cplusplus == 202002
    #define CPLUSPLUS_VERSION "C++20"
  #elif __cplusplus > 202002
    #define CPLUSPLUS_VERSION ">C++20"
  #else
    #define CPLUSPLUS_VERSION "bad value"
  #endif
#else
  #define CPLUSPLUS_VERSION "undefined"
#endif

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION                                                 "\n" \
  "locally built, modified by Tim Holt"                                   "\n" \
  "compiled " __DATE__ " " __TIME__                                       "\n" \
  "C++ version: (" CPLUSPLUS_VERSION ")"                                  "\n"
#endif

// Macros
//--------

/// These are used in two places:
///
/// 1. In your keymap definitions. There, you'll use the syntax `M(MACRO_NAME)`
///    to mark a specific keymap position as triggering `MACRO_NAME`
///
/// 2. In the 'switch' statement in the `macroAction` function. That switch
///    statement actually runs the code associated with a macro when a macro key
///    is pressed.
///
enum MyMacros
{
  /// Types out important firmware information.
  MACRO_VERSION_INFO,
  /// The fabled "any" key. Types a random alphanumeric character.
  MACRO_ANY
};


//   Per-Macro Definitions
//  -----------------------

/// Handles the 'firmware version info' macro.
///
/// When a key bound to the macro is pressed, this macro prints out the firmware
/// build information as virtual keystrokes.
///
static void versionInfoMacro(uint8_t keyState)
{
  if (keyToggledOn(keyState)) {
    Macros.type(PSTR("Keyboardio Model 01 - Kaleidoscope "));
    Macros.type(PSTR(BUILD_INFORMATION));
  }
}

/// Provides the functionality of the 'Any' key.
///
/// When the 'any key' macro is toggled on, a random alphanumeric key is
/// selected. While the key is held, the function generates a synthetic
/// keypress event repeating that randomly selected key.
///
static void anyKeyMacro(uint8_t keyState)
{
  static Key lastKey;
  bool toggledOn = false;
  if (keyToggledOn(keyState)) {
    lastKey.setKeyCode(Key_A.getKeyCode() + (uint8_t)(millis() % 36));
    toggledOn = true;
  }

  if (keyIsPressed(keyState)) {
    Kaleidoscope.hid().keyboard().pressKey(lastKey, toggledOn);
  }
}

//   Meta-Macro Definitions
//  ------------------------

/// Dispatches keymap events that are tied to a macro to that macro.
///
/// \param macroIndex
///   The macro being called (the entry in the 'enum' earlier in this file).
///
/// \param keyState
///   The state of the keyswitch. You can use the keyswitch state to figure out
///   if the key has just been toggled on, is currently pressed or if it's just
///   been released.
///
/// The 'switch' statement should have a 'case' for each entry of the macro
/// enum. Each 'case' statement should call out to a function to handle the
/// macro in question.
///
const macro_t*
macroAction(uint8_t macroIndex, uint8_t keyState)
{
  switch (macroIndex) {
    case MACRO_VERSION_INFO:  versionInfoMacro(keyState);                 break;
    case MACRO_ANY:           anyKeyMacro(keyState);                      break;
  }
  return MACRO_NONE;
}

// Key Layout
//------------

/// The Model 01's key layouts are defined as 'keymaps'. By default, there are three
/// keymaps: The standard QWERTY keymap, the "Function layer" keymap and the "Numpad"
/// keymap.
///
/// Each keymap is defined as a list using the 'KEYMAP_STACKED' macro, built
/// of first the left hand's layout, followed by the right hand's layout.
///
/// Keymaps typically consist mostly of `Key_` definitions. There are many, many keys
/// defined as part of the USB HID Keyboard specification. You can find the names
/// (if not yet the explanations) for all the standard `Key_` defintions offered by
/// Kaleidoscope in these files:
///    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_keyboard.h
///    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_consumerctl.h
///    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_sysctl.h
///    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs_keymaps.h
///
/// Additional things that should be documented here include
///  * using ___ to let keypresses fall through to the previously active layer
///  * using XXX to mark a keyswitch as 'blocked' on this layer
///  * using ShiftToLayer() and LockLayer() keys to change the active keymap.
///  * keeping NUM and FN consistent and accessible on all layers
///
/// The PROG key is special, since it is how you indicate to the board that you
/// want to flash the firmware. However, it can be remapped to a regular key.
/// When the keyboard boots, it first looks to see whether the PROG key is held
/// down; if it is, it simply awaits further flashing instructions. If it is
/// not, it continues loading the rest of the firmware and the keyboard
/// functions normally, with whatever binding you have set to PROG. More detail
/// here: https://community.keyboard.io/t/how-the-prog-key-gets-you-into-the-bootloader/506/8
///
/// The "keymaps" data structure is a list of the keymaps compiled into the firmware.
/// The order of keymaps in the list is important, as the ShiftToLayer(#) and LockLayer(#)
/// macros switch to key layers based on this list.
///
///
/// A key defined as 'ShiftToLayer(FUNCTION)' will switch to FUNCTION while held.
/// Similarly, a key defined as 'LockLayer(NUMPAD)' will switch to NUMPAD when tapped.
///

enum Layers
{
  DVORAK,
  QWERTY,
  NAVNUM,
  FUNCTION
};

#define Key_LeftAngleBracket LSHIFT(Key_Comma)
#define Key_RightAngleBracket LSHIFT(Key_Period)

KEYMAPS(
  // note: Inline commas are aligned with spaces in a given "paragraph".
  // Additionally, they're followed by a tab so you can change the tabstop width
  // in order to align columns between separate "paragraphs" with each-other.
  //
  // clang-format off
  //
  // This comment toggles astyle off
  // *INDENT-OFF*

  [DVORAK] = KEYMAP_STACKED(
    ___              ,	TOPSY(1)     ,	TOPSY(2) ,	TOPSY(3)  ,	TOPSY(4),	TOPSY(5),	Key_LEDEffectNext,
    Key_Backtick     ,	Key_Quote    ,	Key_Comma,	Key_Period,	Key_P   ,	Key_Y   ,	Key_Tab          ,
    Key_Backslash    ,	Key_A        ,	Key_O    ,	Key_E     ,	Key_U   ,	Key_I   ,
    Key_PcApplication,	Key_Semicolon,	Key_Q    ,	Key_J     ,	Key_K   ,	Key_X   ,	Key_Escape       ,

    Key_LeftGui,	Key_Backspace,	Key_LeftShift,	Key_LeftControl,
    ShiftToLayer(NAVNUM),

    M(MACRO_ANY),	TOPSY(6),	TOPSY(7),	TOPSY(8),	TOPSY(9),	TOPSY(0),	LockLayer(QWERTY),
    Key_Enter   ,	Key_F   ,	Key_G   ,	Key_C   ,	Key_R   ,	Key_L   ,	Key_Slash        ,
                 	Key_D   ,	Key_H   ,	Key_T   ,	Key_N   ,	Key_S   ,	Key_Minus        ,
    Key_RightAlt,	Key_B   ,	Key_M   ,	Key_W   ,	Key_V   ,	Key_Z   ,	Key_Equals       ,

    Key_RightControl,	Key_RightShift,	Key_Spacebar,	Key_LeftAlt,
    ShiftToLayer(NAVNUM)
  ),


  [QWERTY] = KEYMAP_STACKED(
    ___,	___  ,	___  ,	___  ,	___  ,	___  ,	___,
    ___,	Key_Q,	Key_W,	Key_E,	Key_R,	Key_T,	___,
    ___,	Key_A,	Key_S,	Key_D,	Key_F,	Key_G,
    ___,	Key_Z,	Key_X,	Key_C,	Key_V,	Key_B,	___,

    ___,	___,	___,	___,
    ___,

    ___,	___  ,	___  ,	___      ,	___       ,	___          ,	___       ,
    ___,	Key_Y,	Key_U,	Key_O    ,	Key_I     ,	Key_P        ,	Key_Equals,
        	Key_H,	Key_J,	Key_K    ,	Key_L     ,	Key_Semicolon,	Key_Quote ,
    ___,	Key_N,	Key_M,	Key_Comma,	Key_Period,	Key_Slash    ,	Key_Minus ,

    ___,	___,	___,	___,
    ___
  ),


  [NAVNUM] = KEYMAP_STACKED(
    XXX,	___                 ,	___          ,	___        ,	___           ,	___                  ,	Key_LEDEffectPrevious,
    ___,	___                 ,	Key_Home     ,	Key_UpArrow,	Key_PageUp    ,	___                  ,	___                  ,
    ___,	Key_LeftBracket     ,	Key_LeftArrow,	Key_Delete ,	Key_RightArrow,	Key_RightBracket     ,
    ___,	Key_LeftAngleBracket,	Key_End      ,	Key_DnArrow,	Key_PageDown  ,	Key_RightAngleBracket,	___                  ,

    ___,	___,	___,	___,
    ShiftToLayer(FUNCTION),

    Consumer_Mute  ,	Consumer_VolumeIncrement,	Key_KeypadNumLock,	Key_KeypadDivide,	Key_KeypadMultiply,	Key_KeypadSubtract   ,	___         ,
    Key_KeypadEnter,	Consumer_VolumeDecrement,	Key_Keypad7      ,	Key_Keypad8     ,	Key_Keypad9       ,	Key_KeypadAdd        ,	XXX         ,
                    	Key_LeftParen           ,	Key_Keypad4      ,	Key_Keypad5     ,	Key_Keypad6       ,	Key_RightParen       ,	XXX         ,
    ___            ,	Key_LeftCurlyBracket    ,	Key_Keypad1      ,	Key_Keypad2     ,	Key_Keypad3       ,	Key_RightCurlyBracket,	Key_CapsLock,

    ___,	Key_KeypadDot,	Key_Keypad0,	___,
    ShiftToLayer(FUNCTION)
  ),


  [FUNCTION] =  KEYMAP_STACKED(
    M(MACRO_VERSION_INFO),	Key_F1         ,	Key_F2    ,	Key_F3   ,	Key_F4 ,	Key_F5 ,	XXX,
    XXX                  ,	Key_F11        ,	Key_F12   ,	Key_F13  ,	Key_F14,	Key_F15,	XXX,
    XXX                  ,	Key_F21        ,	Key_F22   ,	Key_F23  ,	Key_F24,	XXX    ,
    XXX                  ,	Key_PrintScreen,	Key_Insert,	Key_Pause,	XXX    ,	XXX    ,	XXX,

    ___,	___,	___,	___,
    XXX,

    XXX,	Key_F6 ,	Key_F7 ,	Key_F8                    ,	Key_F9                     ,	Key_F10,	XXX,
    XXX,	Key_F16,	Key_F17,	Key_F18                   ,	Key_F19                    ,	Key_F20,	XXX,
        	XXX    ,	XXX    ,	Consumer_PlaySlashPause   ,	XXX                        ,	XXX    ,	XXX,
    XXX,	XXX    ,	XXX    ,	LGUI(LCTRL(Key_LeftArrow)),	LGUI(LCTRL(Key_RightArrow)),	XXX    ,	XXX,

    ___,	___,	Key_Space,	Key_RightAlt,
    XXX
  )

  // Re-enable astyle's indent enforcement.
  // *INDENT-ON*
  //
  // clang-format on
) // KEYMAPS

// Plugin Settings
//-----------------

//   Individual Plugins
//  --------------------

//     LEDSolidColor
//    ---------------

// These 'solid' color effect definitions define a rainbow of
// LED color modes calibrated to draw 500mA or less on the
// Keyboardio Model 01.
namespace SolidColor {
  using kaleidoscope::plugin::LEDSolidColor;

  static LEDSolidColor red(    160,   0,   0);
  static LEDSolidColor orange( 140,  70,   0);
  static LEDSolidColor yellow( 130, 100,   0);
  static LEDSolidColor green(    0, 160,   0);
  static LEDSolidColor blue(     0,  70, 130);
  static LEDSolidColor indigo(   0,   0, 170);
  static LEDSolidColor violet( 130,   0, 120);
} //namespace SolidColor

//     HostPowerManagement
//    ---------------------

/// Makes LEDs power state correspond to keyboard's wake state.
///
/// \TODO  Use a using declaration, maybe even a using enum (C++20).
///
void
toggleLedsOnSuspendResume(
  kaleidoscope::plugin::HostPowerManagement::Event event)
{
  //using namespace kaleidoscope::plugin::HostPowerManagement; // TODO
  switch (event) {
    case kaleidoscope::plugin::HostPowerManagement::Suspend:
    LEDControl.disable();
    break;

    case kaleidoscope::plugin::HostPowerManagement::Resume:
    LEDControl.enable();
    break;

    case kaleidoscope::plugin::HostPowerManagement::Sleep:
    // nop, apparently.
    break;
  }
}

/// \brief  Dispatches power management events (suspend, resume, and sleep) to
///   other functions that perform action based on these events.
///
/// \note  I don't think this does anything, nor that it ever has. If a keyboard
///   went to sleep when your computer did, you couldn't wake the computer with
///   the it!
///
inline
void
hostPowerManagementEventHandler(
  kaleidoscope::plugin::HostPowerManagement::Event event)
{
  toggleLedsOnSuspendResume(event);
}

//     Magic Combo
//    -------------

/// Just a list of all the magic combos used by the Model 01's firmware.
///
/// The names aren't particularly important. What is important is that
/// each is unique.
///
/// These are the names of your magic combos. They will be used by the
/// `USE_MAGIC_COMBOS` call below.
///
enum MagicCombos
{
  /// Toggle between Boot (6-key rollover) and NKRO mode.
  COMBO_TOGGLE_NKRO_MODE,
  /// Enter test mode.
  COMBO_ENTER_TEST_MODE
};

//       Wrappers
//      ----------

/// Toggles the keyboard protocol via USBQuirks, and wraps it within
/// a function with an unused argument, to match what MagicCombo expects.
///
static void
toggleKeyboardProtocol([[maybe_unused]] uint8_t combo_index)
{
  USBQuirks.toggleKeyboardProtocol();
}

static void
enterHardwareTestMode([[maybe_unused]] uint8_t combo_index)
{
  HardwareTestMode.runTests();
}

//      ----------
//       Wrappers

/// Magic combo list, a list of key combo and action pairs the firmware should
/// recognise.
///
USE_MAGIC_COMBOS(
  //                                         Left Fn + Esc  + Shift
  {.action = toggleKeyboardProtocol, .keys={ R3C6    , R2C6 , R3C7  }}
  , //                                       Left Fn + Prog + LED
  {.action = enterHardwareTestMode,  .keys={ R3C6    , R0C0 , R0C6  }}
);

//     FunctionalColor
//    -----------------

using namespace kaleidoscope::plugin::LEDFunctionalColor;

/// A completely custom color map
///
/// \note  see https://github.com/jdlien/Kaleidoscope-LEDEffect-FunctionalColor/blob/master/examples/fc_example/fc_example.ino
///     for guidance on bringing this up-to-date.
///
struct MyFunctionalColorMap : public colorMap
{
  // My own color constants, ensuring everything is in a consistent palette.
  static constexpr auto myDefaultColor     = dimgray;
  static constexpr auto myBaseColor        = lightgray;
  static constexpr auto subtleBaseColor1   = skyblue;
  static constexpr auto subtleBaseColor2   = coral;
  static constexpr auto attentionBaseColor = orangered;

  static constexpr auto secondaryColor          = teal;
  static constexpr auto attentionSecondaryColor = blue;

  static constexpr auto tertiaryColor          = orange;
  static constexpr auto attentionTertiaryColor = yellow;

  // shift, control, gui, and alt can all be colored by "modifier" if nocolor is
  // set here.
  FC_MAP_COLOR(defaultColor, myDefaultColor)
  FC_MAP_COLOR(baseColor   , myBaseColor)
  FC_MAP_COLOR(shift       , nocolor)
  FC_MAP_COLOR(control     , nocolor)
  FC_MAP_COLOR(gui         , attentionTertiaryColor)
  FC_MAP_COLOR(alt         , nocolor)
  FC_MAP_COLOR(modifier    , tertiaryColor)

  FC_MAP_COLOR(alpha      , myBaseColor)
  FC_MAP_COLOR(number     , secondaryColor)
  FC_MAP_COLOR(punctuation, tertiaryColor)

  // F1-F12 and F13-F24
  FC_MAP_COLOR(function, attentionSecondaryColor)

  // Page Up, Page Down, Home, End, Insert, and Delete (if del has nocolor)
  FC_MAP_COLOR(navigation, tertiaryColor)

  // Print Screen, Pause/Break, and Scroll Lock keys (brightness on Macs)
  FC_MAP_COLOR(system, tertiaryColor)

  FC_MAP_COLOR(arrow , secondaryColor)
  FC_MAP_COLOR(keypad, secondaryColor)

  // Includes play/pause, next/prev, volume control, mute, etc.
  FC_MAP_COLOR(media, tertiaryColor)

  FC_MAP_COLOR(mouseWheel , nocolor)
  FC_MAP_COLOR(mouseButton, tertiaryColor)
  FC_MAP_COLOR(mouseWarp  , secondaryColor)
  FC_MAP_COLOR(mouseMove  , nocolor)
  // mouse includes the four above groups if nocolor is set for those
  FC_MAP_COLOR(mouse    , myBaseColor)
  FC_MAP_COLOR(space    , myBaseColor)
  FC_MAP_COLOR(tab      , tertiaryColor)
  FC_MAP_COLOR(enter    , tertiaryColor)
  FC_MAP_COLOR(backspace, attentionTertiaryColor)
  FC_MAP_COLOR(escape   , attentionTertiaryColor)
  FC_MAP_COLOR(del      , attentionTertiaryColor)

  // fn will work properly if your FUNCTION layer is between layers 1-3
  FC_MAP_COLOR(fn, myBaseColor)

  // NumLock and other layer locks
  FC_MAP_COLOR(lock         , attentionTertiaryColor)
  FC_MAP_COLOR(LEDEffectNext, tertiaryColor)
};


FC_START_COLOR_LIST(MyColorMapOverrides)

  // Unusual Macros
  FC_KEYCOLOR(M(MACRO_VERSION_INFO), MyFunctionalColorMap::attentionBaseColor)

  // Macros
  //FC_GROUPKEY(M(MACRO_ANY))
  //FC_GROUPKEY(M(MACRO_NEXT_TAB))
  //FC_GROUPKEY(M(MACRO_PREV_TAB))
  //FC_GROUPKEY(M(MACRO_DESKTOP_LEFT))
  //FC_KEYCOLOR(M(MACRO_DESKTOP_RIGHT), MyFunctionalColorMap::attentionTertiaryColor)
  FC_KEYCOLOR(M(MACRO_ANY), MyFunctionalColorMap::attentionTertiaryColor)
  //
  //// Layers
  //FC_GROUPKEY(M(MACRO_MODE_DVORAK))
  //FC_GROUPKEY(M(MACRO_MODE_GAMING))
  //FC_GROUPKEY(M(MACRO_MODE_DF))
  //FC_GROUPKEY(M(MACRO_MODE_EPISTORY))
  //FC_KEYCOLOR(M(MACRO_MODE_MY_QWERTY), green)
  //
  //FC_KEYCOLOR(M(MACRO_MODE_QWERTY), red)

  // Function switchers
  //FC_GROUPKEY(ShiftToLayer(FUNCTION))
  //FC_GROUPKEY(ShiftToLayer(MY_FUNCTION_LR))
  //FC_GROUPKEY(M(MACRO_R_FUNCTION))
  //FC_GROUPKEY(ShiftToLayer(DF_FUNCTION))
  //FC_KEYCOLOR(ShiftToLayer(MY_FUNCTION_L), MyFunctionalColorMap::secondaryColor)
  FC_KEYCOLOR(ShiftToLayer(FUNCTION), MyFunctionalColorMap::secondaryColor)

  FC_GROUPKEY(Key_LeftAngleBracket)
  FC_KEYCOLOR(Key_RightAngleBracket, MyFunctionalColorMap::tertiaryColor)

  //FC_KEYCOLOR(LockLayer(NUMPAD), MyFunctionalColorMap::attentionBaseColor)
  FC_KEYCOLOR(LockLayer(QWERTY), MyFunctionalColorMap::attentionBaseColor)

  FC_KEYCOLOR(Key_CapsLock, MyFunctionalColorMap::attentionTertiaryColor)

  #ifdef DIFFERENTIATE_LETTERS_BY_COLOR
  // Vowels
  FC_GROUPKEY(Key_A)
  FC_GROUPKEY(Key_E)
  FC_GROUPKEY(Key_I)
  FC_GROUPKEY(Key_O)
  FC_KEYCOLOR(Key_U, MyFunctionalColorMap::subtleBaseColor1)

  // Vim directional keys
  FC_GROUPKEY(Key_H)
  FC_GROUPKEY(Key_J)
  FC_GROUPKEY(Key_K)
  FC_KEYCOLOR(Key_L, MyFunctionalColorMap::subtleBaseColor2)
  #endif

FC_END_COLOR_LIST

FunctionalColor my_functional_color_map(FC_COLOR_LIST(MyColorMapOverrides), 100, false);

//   Initialize Plugin System
//  --------------------------

/// Initialize Kaleidoscope plugins.
///
/// Tell Kaleidoscope which plugins you want to use. The order can be important.
/// For example, LED effects are added in the order they're listed here.
///
KALEIDOSCOPE_INIT_PLUGINS(
  HardwareTestMode,
  BootGreetingEffect,

  LEDControl,

  LEDOff, // Should be first!
  my_functional_color_map,
  WavepoolEffect, LEDRainbowWaveEffect, LEDRainbowEffect, LEDBreatheEffect,
  SolidColor::red, SolidColor::green, SolidColor::indigo,

  Macros,

  HostPowerManagement,
  MagicCombo,
  USBQuirks,

  TopsyTurvy
);

// Special Functions
//-------------------

//   Helper Functions
//  ------------------

namespace Hue {
  inline constexpr uint8_t toHue(float hue)
  {
    return UINT8_MAX * hue;
  }
  // Names are approximate to protect the innocent.
  // Primary
  static const auto red    = toHue(0.0 / 3.0);
  static const auto green  = toHue(1.0 / 3.0);
  static const auto blue   = toHue(2.0 / 3.0);

  // Secondary
  static const auto yellow = toHue(0.5 / 3.0);
  static const auto cyan   = toHue(1.5 / 3.0);
  static const auto purple = toHue(2.5 / 3.0);

  // Other
  static const auto orange    = toHue(1.0 / 12.0);
  static const auto redorange = toHue(0.5 / 12.0);
}

void inline
setupFunctionalColor()
{
  FC_SET_THEME(my_functional_color_map, MyFunctionalColorMap);
}

void inline
setupWavepoolEffect()
{
  // Default: 5'000 milliseconds (0 is off)
  WavepoolEffect.idle_timeout = 2'500 /* milliseconds */;

  WavepoolEffect.ripple_hue =
    WavepoolEffect.rainbow_hue;  // Default
    //Hue::cyan;
}

void inline
setupLEDRainbow()
{
  // Defaults were 150 (out of 255)
  LEDRainbowEffect.brightness(100);
  LEDRainbowWaveEffect.brightness(100);
}

void inline
setupLEDBreathe()
{
  LEDBreatheEffect.hue = Hue::redorange;
}

void
setupBoringCorePlugins()
{
  // Set the action key the test mode should listen for.
  HardwareTestMode.setActionKey(R3C6 /*Left Fn*/);

  // We want to make sure that the firmware starts with LED effects off.
  // (in case of low-powered devices!)
  LEDOff.activate();
}

//  ------------------
//   Helper Functions

void
setup()
{
  // Must be first!
  Kaleidoscope.setup();

  // Should be setup earlier than most (so it captures keypresses correctly)!
  setupWavepoolEffect();

  setupFunctionalColor();
  setupLEDRainbow();
  setupLEDBreathe();

  setupBoringCorePlugins();
}

/// \note For Kaleidoscope-based keyboard firmware, you usually just want to
///   call Kaleidoscope.loop(); and not do anything custom here.
///
void
loop()
{
  Kaleidoscope.loop();
}

//---
//

// vim: set ft=arduino.cpp et ts=2 sw=0 sts=0 :

