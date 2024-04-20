#ifndef BP32_ARDUINO_CONTROLLER_SIMPLE_H
#define BP32_ARDUINO_CONTROLLER_SIMPLE_H

#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif  // !CONFIG_BLUEPAD32_PLATFORM_ARDUINO

#include <inttypes.h>
#include <uni_platform_arduino.h>

#include <Arduino.h>

#include "ArduinoControllerData.h"
#include "ArduinoControllerProperties.h"
#include "ArduinoKeyboardConstants.h"

class ControllerSimple {
   
   public:
    ControllerSimple(ControllerData data);

    //
    // Gamepad Related
    //

    uint8_t dpad() const { return _data.gamepad.dpad; }

    // Axis
    int32_t axisX() const { return _data.gamepad.axis_x; }
    int32_t axisY() const { return _data.gamepad.axis_y; }
    int32_t axisRX() const { return _data.gamepad.axis_rx; }
    int32_t axisRY() const { return _data.gamepad.axis_ry; }

    // Brake & Throttle
    int32_t brake() const { return _data.gamepad.brake; }
    int32_t throttle() const { return _data.gamepad.throttle; }

    // Gyro / Accel
    int32_t gyroX() const { return _data.gamepad.gyro[0]; }
    int32_t gyroY() const { return _data.gamepad.gyro[1]; }
    int32_t gyroZ() const { return _data.gamepad.gyro[2]; }
    int32_t accelX() const { return _data.gamepad.accel[0]; }
    int32_t accelY() const { return _data.gamepad.accel[1]; }
    int32_t accelZ() const { return _data.gamepad.accel[2]; }

    //
    // Shared between Mouse & Gamepad
    //

    // Returns the state of all buttons.
    uint16_t buttons() const {
        if (_data.klass == UNI_CONTROLLER_CLASS_GAMEPAD)
            return _data.gamepad.buttons;
        if (_data.klass == UNI_CONTROLLER_CLASS_MOUSE)
            return _data.mouse.buttons;
        // Not supported in other controllers
        return 0;
    }

    // Returns the state of all misc buttons.
    uint16_t miscButtons() const {
        if (_data.klass == UNI_CONTROLLER_CLASS_GAMEPAD)
            return _data.gamepad.misc_buttons;
        if (_data.klass == UNI_CONTROLLER_CLASS_MOUSE)
            return _data.mouse.misc_buttons;
        // Not supported in other controllers
        return 0;
    }

    // To test one button at the time.
    bool a() const { return buttons() & BUTTON_A; }
    bool b() const { return buttons() & BUTTON_B; }
    bool x() const { return buttons() & BUTTON_X; }
    bool y() const { return buttons() & BUTTON_Y; }
    bool l1() const { return buttons() & BUTTON_SHOULDER_L; }
    bool l2() const { return buttons() & BUTTON_TRIGGER_L; }
    bool r1() const { return buttons() & BUTTON_SHOULDER_R; }
    bool r2() const { return buttons() & BUTTON_TRIGGER_R; }
    bool thumbL() const { return buttons() & BUTTON_THUMB_L; }
    bool thumbR() const { return buttons() & BUTTON_THUMB_R; }

    // Misc buttons
    bool miscSystem() const { return miscButtons() & MISC_BUTTON_SYSTEM; }
    bool miscSelect() const { return miscButtons() & MISC_BUTTON_SELECT; }
    bool miscStart() const { return miscButtons() & MISC_BUTTON_START; }
    bool miscCapture() const { return miscButtons() & MISC_BUTTON_CAPTURE; }

    // Deprecated
    bool miscBack() const { return miscSelect(); }
    bool miscHome() const { return miscStart(); }

    //
    // Mouse related
    //
    int32_t deltaX() const { return _data.mouse.delta_x; }
    int32_t deltaY() const { return _data.mouse.delta_y; }
    int8_t scrollWheel() const { return _data.mouse.scroll_wheel; }

    //
    // Wii Balance Board related
    //
    uint16_t topLeft() const { return _data.balance_board.tl; }
    uint16_t topRight() const { return _data.balance_board.tr; }
    uint16_t bottomLeft() const { return _data.balance_board.bl; }
    uint16_t bottomRight() const { return _data.balance_board.br; }
    int temperature() const { return _data.balance_board.temperature; }

    //
    // Keyboard related
    //
    bool isKeyPressed(KeyboardKey key) const;

    //
    // Shared among all
    //

    // 0 = Unknown Battery state
    // 1 = Battery Empty
    // 255 = Battery full
    uint8_t battery() const { return _data.battery; }

    bool isGamepad() const { return _data.klass == UNI_CONTROLLER_CLASS_GAMEPAD; }
    bool isMouse() const { return _data.klass == UNI_CONTROLLER_CLASS_MOUSE; }
    bool isBalanceBoard() const { return _data.klass == UNI_CONTROLLER_CLASS_BALANCE_BOARD; }
    bool isKeyboard() const { return _data.klass == UNI_CONTROLLER_CLASS_KEYBOARD; }
    uni_controller_class_t getClass() const { return _data.klass; }

   private:
        ControllerData _data;
        bool isModifierPressed(KeyboardKey key) const;


    // Friends
    friend class Bluepad32;
};

typedef ControllerSimple* ControllerSimplePtr;

#endif  // BP32_ARDUINO_CONTROLLER_H