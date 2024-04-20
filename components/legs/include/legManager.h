

#ifndef LED_MANAGER_H
#define BP32_CONSOLE_H
#include "leg.h"
#include <stddef.h>
#include <pins_arduino.h>
#include <Bluepad32.h>
#include <leds.h>


class LegManager {
   public:
    LegManager(LedManager *ledManager,
    uint8_t enableAPin, uint8_t iNA1Pin, uint8_t iNA2Pin,
    uint8_t enableBPin, uint8_t iNB1Pin, uint8_t iNB2Pin,
    uint8_t enableCPin, uint8_t iNC1Pin, uint8_t iNC2Pin,
    uint8_t enableDPin, uint8_t iND1Pin, uint8_t iND2Pin);
    void init(void);
    void process(ControllerPtr cptr);
    void brakeS(void);
   private:
    LedManager *_ledManager;
    ControllerSimplePtr _previousControler;
    Leg *_legA;
    Leg *_legB;
    Leg *_legC;
    Leg *_legD;
};

#endif 