// Copyright 2021 - 2023, Ricardo Quesada, http://retro.moe
// SPDX-License-Identifier: Apache-2.0 or LGPL-2.1-or-later

#include "ArduinoGamepad.h"

#include <inttypes.h>
#include <uni_common.h>
#include <uni_log.h>
#include <uni_platform_arduino.h>

#include "include/ArduinoControllerSimple.h"
#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif  // !CONFIG_BLUEPAD32_PLATFORM_ARDUINO



ControllerSimple::ControllerSimple(ControllerData data): _data(data){}


//
// Keyboard functions
//
bool ControllerSimple::isKeyPressed(KeyboardKey key) const {
    // When querying for Modifiers, delegate to modifier function
    if (key >= Keyboard_LeftControl && key <= Keyboard_RightMeta) {
        return isModifierPressed(key);
    }

    for (int i = 0; i < UNI_KEYBOARD_PRESSED_KEYS_MAX; i++) {
        // Return early on error
        if (_data.keyboard.pressed_keys[i] <= HID_USAGE_KB_ERROR_UNDEFINED)
            return false;
        if (_data.keyboard.pressed_keys[i] == key)
            return true;
    }
    return false;
}

bool ControllerSimple::isModifierPressed(KeyboardKey key) const {
    static uint8_t convertion[] = {
        UNI_KEYBOARD_MODIFIER_LEFT_CONTROL,   //
        UNI_KEYBOARD_MODIFIER_LEFT_SHIFT,     //
        UNI_KEYBOARD_MODIFIER_LEFT_ALT,       //
        UNI_KEYBOARD_MODIFIER_LEFT_GUI,       //
        UNI_KEYBOARD_MODIFIER_RIGHT_CONTROL,  //
        UNI_KEYBOARD_MODIFIER_RIGHT_SHIFT,    //
        UNI_KEYBOARD_MODIFIER_RIGHT_ALT,      //
        UNI_KEYBOARD_MODIFIER_RIGHT_GUI,      //
    };

    // Safety check, out of range ?
    if (key < Keyboard_LeftControl || key > Keyboard_RightMeta)
        return false;

    int idx = key - Keyboard_LeftControl;
    uint8_t modifier = convertion[idx];

    // Safe to test for non-zero since we know that only one-bit is on in "modifier".
    return (_data.keyboard.modifiers & modifier);
}

