/****************************************************************************
http://retro.moe/unijoysticle2

Copyright 2021 Ricardo Quesada

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
****************************************************************************/

#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif  // !CONFIG_BLUEPAD32_PLATFORM_ARDUINO

#include <Arduino.h>
#include <Bluepad32.h>
#include <butterMotor.h>
#include "esp_sleep.h"
#include <leds.h>
#include <legManager.h>

#define LED_PIN 19
#define LED_COUNT 3
#define BUTTER_ENABLE_PIN 23
#define BUTTER_IN1_PIN 22
#define BUTTER_IN2_PIN 21
#define VBAT_PIN 34
 
#define LASER_PIN 18

#define LEG_A_ENABLE_PIN 4
#define LEG_A_IN1_PIN 2
#define LEG_A_IN2_PIN 15
#define LEG_B_ENABLE_PIN 16
#define LEG_B_IN1_PIN 32
#define LEG_B_IN2_PIN 33
#define LEG_C_ENABLE_PIN 25
#define LEG_C_IN1_PIN 26
#define LEG_C_IN2_PIN 27
#define LEG_D_ENABLE_PIN 14
#define LEG_D_IN1_PIN 12
#define LEG_D_IN2_PIN 13


#define DEFAULT_WAKEUP_LEVEL 1
#define DEFAULT_WAKEUP_PIN  GPIO_NUM_34

ButterMotor *butter;
LedManager *ledManager;
LegManager *legManager;
uint16_t vbat;
//
// README FIRST, README FIRST, README FIRST
//
// Bluepad32 has a built-in interactive console.
// By default it is enabled (hey, this is a great feature!).
// But it is incompatible with Arduino "Serial" class.
//
// Instead of using "Serial" you can use Bluepad32 "Console" class instead.
// It is somewhat similar to Serial but not exactly the same.
//
// Should you want to still use "Serial", you have to disable the Bluepad32's console
// from "sdkconfig.defaults" with:
//    CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE=n

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Console.printf("CALLBACK: Controller is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            ControllerProperties properties = ctl->getProperties();
            Console.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName(), properties.vendor_id,
                           properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Console.println("CALLBACK: Controller connected, but could not found empty slot");
    }
    BP32.enableNewBluetoothConnections(false);
    ledManager->setBluetooth(true);
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Console.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    if (!foundController) {
        Console.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
    ledManager->setBluetooth(false);
    BP32.enableNewBluetoothConnections(true);
    
}

void dumpGamepad(ControllerPtr ctl) {
    Console.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
        "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d battery:%3d\n",
        ctl->index(),        // Controller Index
        ctl->dpad(),         // DPAD
        ctl->buttons(),      // bitmask of pressed buttons
        ctl->axisX(),        // (-511 - 512) left X Axis
        ctl->axisY(),        // (-511 - 512) left Y axis
        ctl->axisRX(),       // (-511 - 512) right X axis
        ctl->axisRY(),       // (-511 - 512) right Y axis
        ctl->brake(),        // (0 - 1023): brake button
        ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
        ctl->miscButtons(),  // bitmak of pressed "misc" buttons
        ctl->gyroX(),        // Gyro X
        ctl->gyroY(),        // Gyro Y
        ctl->gyroZ(),        // Gyro Z
        ctl->accelX(),       // Accelerometer X
        ctl->accelY(),       // Accelerometer Y
        ctl->accelZ(),        // Accelerometer Z
        ctl->battery()
    );
}

void processGamepad(ControllerPtr ctl) {
    butter->process(ctl);
    legManager->process(ctl);
    dumpGamepad(ctl);
}
void registerDeepSleep(){
    esp_sleep_enable_ext0_wakeup(DEFAULT_WAKEUP_PIN, DEFAULT_WAKEUP_LEVEL);
}

// Arduino setup function. Runs in CPU 1
void setup() {
   // pinMode(VBAT_PIN, INPUT);
  //  vbat = analogRead(VBAT_PIN);
    ledManager = new LedManager(LED_PIN,LED_COUNT);
    ledManager->init();
  //  registerDeepSleep();
    BP32.forgetBluetoothKeys();

    Console.printf("Firmware: %s\n", BP32.firmwareVersion());
    const uint8_t* addr = BP32.localBdAddress();
    Console.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.enableVirtualDevice(false);

    butter = new ButterMotor(ledManager, BUTTER_ENABLE_PIN,BUTTER_IN1_PIN,BUTTER_IN2_PIN);
    butter->init();
    ledManager->setBluetooth(false);
    legManager = new LegManager(ledManager,
    LEG_A_ENABLE_PIN,LEG_A_IN1_PIN,LEG_A_IN2_PIN,
    LEG_B_ENABLE_PIN,LEG_B_IN1_PIN,LEG_B_IN2_PIN,
    LEG_C_ENABLE_PIN,LEG_C_IN1_PIN,LEG_C_IN2_PIN,
    LEG_D_ENABLE_PIN,LEG_D_IN1_PIN,LEG_D_IN2_PIN);
    legManager->init();
}

// Arduino loop function. Runs in CPU 1
void loop() {
    BP32.update();
    //vbat = analogRead(VBAT_PIN);
    ledManager->setVBat(vbat);
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        ControllerPtr myController = myControllers[i];

        if (myController && myController->isConnected()) {
            if (myController->isGamepad() && myController->a() && myController->b()) {
                butter->brakeS();
                ledManager->setSleep();
                Console.printf("Entering sleep mode!\n");
                delay(2000);                
                esp_deep_sleep_start();
                continue;
            }

            if (myController->isGamepad()) {
                processGamepad(myController);
            } else {
                Console.printf("Data not available yet\n");
                continue;
            }
            
        }
    }
    ledManager->updateLeds();

    delay(150);
}
