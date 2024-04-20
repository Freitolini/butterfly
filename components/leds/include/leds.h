

#ifndef LEDS_H
#define LEDS_H

#include <stddef.h>
#include <pins_arduino.h>
#include <Bluepad32.h>
#include <Adafruit_NeoPixel.h>

#define LED_NUMBER 3
#define POWER_LED 0
#define SPEED_MODE_LED 1
#define BLUETOOTH_LED 2
#define SPEED_MODE_STEP 20
#define VBAT_STEP 17


class LedManager {
    Adafruit_NeoPixel *pixels;
    uint8_t _ledPin;
    uint8_t _ledNumber;
   public:
    LedManager(uint16_t ledPin, uint16_t ledNumber);
    void init(void);
    void setSpeedMode(uint8_t speedMode, bool right);
    void updateLeds(void);
    void setVBat(uint16_t);
    void setSleep(void);
    void setBluetooth(bool);
   
};

#endif 