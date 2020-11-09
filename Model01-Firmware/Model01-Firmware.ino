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

// Defines
//---------

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION                                                 "\n" \
  "locally built, modified by Tim Holt"                                   "\n" \
  "compiled " __DATE__ " " __TIME__                                       "\n"
#endif

// Includes
//----------

//   Kaleidoscope core & plugins
//  -----------------------------

// The Kaleidoscope core
#include "Kaleidoscope.h"

// Support for storing the keymap in EEPROM
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"

// Support for communicating with the host via a simple Serial protocol
#include "Kaleidoscope-FocusSerial.h"

// Support for macros
#include "Kaleidoscope-Macros.h"

// Support for controlling the keyboard's LEDs
#include "Kaleidoscope-LEDControl.h"

// Support for the "Boot greeting" effect, which pulses the 'LED' button for 10s
// when the keyboard is connected to a computer (or that computer is powered on)
#include "Kaleidoscope-LEDEffect-BootGreeting.h"

// Support for LED modes that set all LEDs to a single color
#include "Kaleidoscope-LEDEffect-SolidColor.h"

// Support for LED modes that pulse the keyboard's LED in a rainbow pattern
#include "Kaleidoscope-LEDEffect-Rainbow.h"

// Support for shared palettes for other plugins, like Colormap below
#include "Kaleidoscope-LED-Palette-Theme.h"

// Support for an LED mode that lets one configure per-layer color maps
#include "Kaleidoscope-Colormap.h"

// Support for Keyboardio's internal keyboard testing mode
#include "Kaleidoscope-HardwareTestMode.h"

// Support for host power management (suspend & wakeup)
#include "Kaleidoscope-HostPowerManagement.h"

// Support for magic combos (key chords that trigger an action)
#include "Kaleidoscope-MagicCombo.h"

// Support for USB quirks, like changing the key state report protocol
#include "Kaleidoscope-USB-Quirks.h"

#include "Kaleidoscope-TopsyTurvy.h"

// Key Layout
//------------

/// Just a list of all the macros used by the Model 01's firmware.
///
/// The names aren't particularly important. What is important is that each
/// is unique.
///
/// These are the names of your macros. They'll be used in two places:
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
///   using ___ to let keypresses fall through to the previously active layer
///   using XXX to mark a keyswitch as 'blocked' on this layer
///   using ShiftToLayer() and LockLayer() keys to change the active keymap.
///   keeping NUM and FN consistent and accessible on all layers
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


// This comment toggles astyle off
// *INDENT-OFF*
// clang-format off

KEYMAPS(

  [DVORAK] = KEYMAP_STACKED
  (___, TOPSY(1), TOPSY(2), TOPSY(3), TOPSY(4), TOPSY(5), Key_LEDEffectPrevious,
   Key_Backtick,           Key_Quote,     Key_Comma, Key_Period, Key_P, Key_Y, Key_Tab,
   LSHIFT(LCTRL(Key_Tab)), Key_A,         Key_O,     Key_E,      Key_U, Key_I,
   LCTRL(Key_Tab),         Key_Semicolon, Key_Q,     Key_J,      Key_K, Key_X, Key_Escape,

   Key_LeftGui, Key_Backspace, Key_LeftShift, Key_LeftControl,
   ShiftToLayer(NAVNUM),

   M(MACRO_ANY), TOPSY(6), TOPSY(7), TOPSY(8), TOPSY(9), TOPSY(0), LockLayer(QWERTY),
   Key_Enter,    Key_F, Key_G, Key_C, Key_R, Key_L, Key_Slash,
                 Key_D, Key_H, Key_T, Key_N, Key_S, Key_Minus,
   Key_RightAlt, Key_B, Key_M, Key_W, Key_V, Key_Z, Key_Equals,

   Key_RightControl, Key_RightShift, Key_Spacebar, Key_LeftAlt,
   ShiftToLayer(NAVNUM)),


  [QWERTY] = KEYMAP_STACKED
  (___, ___  , ___  , ___  , ___  , ___  , ___,
   ___, Key_Q, Key_W, Key_E, Key_R, Key_T, ___,
   ___, Key_A, Key_S, Key_D, Key_F, Key_G,
   ___, Key_Z, Key_X, Key_C, Key_V, Key_B, ___,

   ___, ___, ___, ___,
   ___,

   ___, ___, ___, ___, ___, ___, ___,
   ___, Key_Y, Key_U, Key_O, Key_I, Key_P, Key_Equals,
        Key_H, Key_J, Key_K, Key_L, Key_Semicolon, Key_Quote,
   ___, Key_N, Key_M, Key_Comma, Key_Period, Key_Slash, Key_Minus,

   ___, ___, ___, ___,
   ___),


  [NAVNUM] =  KEYMAP_STACKED
  (XXX, ___              , ___          , ___        , ___           , ___,                ___,
   ___, ___              , Key_Home     , Key_UpArrow, Key_PageUp    , ___,                ___,
   ___, Key_LeftBracket  , Key_LeftArrow, Key_Delete , Key_RightArrow, Key_RightBracket,
   ___, LSHIFT(Key_Comma), Key_End      , Key_DnArrow, Key_PageDown  , LSHIFT(Key_Period), ___,

   ___, ___, ___, ___,
   ShiftToLayer(FUNCTION),

   Consumer_Mute, Consumer_VolumeIncrement, Key_KeypadNumLock, Key_KeypadDivide, Key_KeypadMultiply, Key_KeypadSubtract, ___,
   Key_KeypadEnter, Consumer_VolumeDecrement, Key_Keypad7, Key_Keypad8, Key_Keypad9, Key_KeypadAdd, Key_Backslash,
                  Key_LeftParen, Key_Keypad4, Key_Keypad5,      Key_Keypad6,              Key_RightParen, Key_Pipe,
   ___, Key_LeftCurlyBracket, Key_Keypad1, Key_Keypad2, Key_Keypad3, Key_RightCurlyBracket, Key_CapsLock,

   ___, Key_KeypadDot, Key_Keypad0, ___,
   ShiftToLayer(FUNCTION)),


  [FUNCTION] =  KEYMAP_STACKED
  (M(MACRO_VERSION_INFO),
        Key_F1 , Key_F2 , Key_F3 , Key_F4 , Key_F5 , XXX,
   XXX, Key_F11, Key_F12, Key_F13, Key_F14, Key_F15, XXX,
   XXX, Key_F21, Key_F22, Key_F23, Key_F24, XXX    ,
   XXX, Key_PrintScreen, Key_Insert, Key_Pause, XXX, XXX, XXX,
   ___, ___, ___, ___,
   XXX,

   XXX, Key_F6 , Key_F7 , Key_F8 , Key_F9 , Key_F10, XXX,
   XXX, Key_F16, Key_F17, Key_F18, Key_F19, Key_F20, XXX,
        Key_LeftArrow, Key_DownArrow, Key_UpArrow, Key_RightArrow, XXX, XXX,
   XXX, XXX, XXX, LGUI(LCTRL(Key_LeftArrow)), LGUI(LCTRL(Key_RightArrow)), XXX, XXX,

   ___, ___, Key_Space, Key_RightAlt,
   XXX)

) // KEYMAPS

// clang-format on
// Re-enable astyle's indent enforcement.
// *INDENT-ON*

//   Macros
//  --------

//     Per-Macro Definitions
//    -----------------------

/// Handles the 'firmware version info' macro.
///
/// When a key bound to the macro is pressed, this macro prints out the firmware
/// build information as virtual keystrokes.
///
static void
versionInfoMacro(uint8_t keyState)
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
static void
anyKeyMacro(uint8_t keyState)
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

//     Meta-Macro Definitions
//    ------------------------

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

// Plugin Settings
//-----------------

// These 'solid' color effect definitions define a rainbow of
// LED color modes calibrated to draw 500mA or less on the
// Keyboardio Model 01.


static kaleidoscope::plugin::LEDSolidColor solidRed(160, 0, 0);
static kaleidoscope::plugin::LEDSolidColor solidOrange(140, 70, 0);
//static kaleidoscope::plugin::LEDSolidColor solidYellow(130, 100, 0);
static kaleidoscope::plugin::LEDSolidColor solidGreen(0, 160, 0);
//static kaleidoscope::plugin::LEDSolidColor solidBlue(0, 70, 130);
//static kaleidoscope::plugin::LEDSolidColor solidIndigo(0, 0, 170);
//static kaleidoscope::plugin::LEDSolidColor solidViolet(130, 0, 120);

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

//   Magic Combo Stuff
//  -------------------

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

//     Wrappers
//    ----------

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

/// Magic combo list, a list of key combo and action pairs the firmware should
/// recognise.
///
USE_MAGIC_COMBOS(
  //                                         Left Fn + Esc  + Shift
  {.action = toggleKeyboardProtocol, .keys={ R3C6    , R2C6 , R3C7  }}
  , //                                       Left Fn + Prog + LED
  {.action = enterHardwareTestMode,  .keys={ R3C6    , R0C0 , R0C6  }}
);

//   Initialize Plugin System
//  --------------------------

/// Initialize Kaleidoscope plugins.
///
/// First, tell Kaleidoscope which plugins you want to use.
/// The order can be important. For example, LED effects are
/// added in the order they're listed here.
///
KALEIDOSCOPE_INIT_PLUGINS(
  // The EEPROMSettings & EEPROMKeymap plugins make it possible to have an
  // editable keymap in EEPROM.
  EEPROMSettings,
  EEPROMKeymap,

  // Focus allows bi-directional communication with the host, and is the
  // interface through which the keymap in EEPROM can be edited.
  Focus,

  // FocusSettingsCommand adds a few Focus commands, intended to aid in
  // changing some settings of the keyboard, such as the default layer (via the
  // `settings.defaultLayer` command)
  FocusSettingsCommand,

  // FocusEEPROMCommand adds a set of Focus commands, which are very helpful in
  // both debugging, and in backing up one's EEPROM contents.
  FocusEEPROMCommand,

  // The boot greeting effect pulses the LED button for 10 seconds after the
  // keyboard is first connected
  BootGreetingEffect,

  // The hardware test mode, which can be invoked by tapping Prog, LED and the
  // left Fn button at the same time.
  HardwareTestMode,

  // LEDControl provides support for other LED modes
  LEDControl,

  // We start with the LED effect that turns off all the LEDs.
  LEDOff,

  // The rainbow effect changes the color of all of the keyboard's keys at the same time
  // running through all the colors of the rainbow.
  LEDRainbowEffect,

  // The rainbow wave effect lights up your keyboard with all the colors of a rainbow
  // and slowly moves the rainbow across your keyboard
  LEDRainbowWaveEffect,

  // These static effects turn your keyboard's LEDs a variety of colors
  solidRed, solidOrange, solidGreen,

  // The LED Palette Theme plugin provides a shared palette for other plugins,
  // like Colormap below
  LEDPaletteTheme,

  // The Colormap effect makes it possible to set up per-layer colormaps
  ColormapEffect,

  // The macros plugin adds support for macros
  Macros,

  // The HostPowerManagement plugin allows us to turn LEDs off when then host
  // goes to sleep, and resume them when it wakes up.
  HostPowerManagement,

  // The MagicCombo plugin lets you use key combinations to trigger custom
  // actions - a bit like Macros, but triggered by pressing multiple keys at the
  // same time.
  MagicCombo,

  // The USBQuirks plugin lets you do some things with USB that we aren't
  // comfortable - or able - to do automatically, but can be useful
  // nevertheless. Such as toggling the key report protocol between Boot (used
  // by BIOSes) and Report (NKRO).
  USBQuirks,

  TopsyTurvy
);

// Special Functions
//-------------------

void
setup()
{
  // First, call Kaleidoscope's internal setup function
  Kaleidoscope.setup();

  // Defaults were 150 (out of 255)
  LEDRainbowEffect.brightness(100);
  LEDRainbowWaveEffect.brightness(100);

  // Set the action key the test mode should listen for.
  HardwareTestMode.setActionKey(R3C6 /*Left Fn*/);

  // We want to make sure that the firmware starts with LED effects off
  // This avoids over-taxing devices that don't have a lot of power to share
  // with USB devices
  LEDOff.activate();

  // To make the keymap editable without flashing new firmware, we store
  // additional layers in EEPROM. For now, we reserve space for five layers. If
  // one wants to use these layers, just set the default layer to one in EEPROM,
  // by using the `settings.defaultLayer` Focus command, or by using the
  // `keymap.onlyCustom` command to use EEPROM layers only.
  EEPROMKeymap.setup(5);

  // We need to tell the Colormap plugin how many layers we want to have custom
  // maps for. To make things simple, we set it to five layers, which is how
  // many editable layers we have (see above).
  ColormapEffect.max_layers(5);
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

// vim: set ft=arduino.cpp et ts=2 sw=2 sts=2 :

