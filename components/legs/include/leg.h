#ifndef LED_H
#define LED_H

#include <stddef.h>
#include <pins_arduino.h>
#include <Bluepad32.h>


class Leg {
   public:
    Leg(uint8_t enableAPin, uint8_t iNA1Pin, uint8_t iNA2Pin, String label);
    void init(void);
    void left(uint16_t speed);
    void right(uint16_t speed);
    void brakeS(void);
   private:
    uint8_t _enablePin;
    uint8_t _iN1Pin;
    uint8_t _iN2Pin;
    String _label;
};

#endif 