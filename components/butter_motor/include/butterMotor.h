

#ifndef BUTTER_MOTOR_H
#define BP32_CONSOLE_H

#include <stddef.h>
#include <pins_arduino.h>
#include <Bluepad32.h>


class ButterMotor {
   public:
    ButterMotor(uint8_t enablePin, uint8_t iN1Pin, uint8_t iN2Pin);
    void init(void);
    void process(ControllerPtr cptr);
    void brakeS(void);
   private:
   void lock(void);
   void unlock(void);
   void forward(uint32_t);
   void reverse(uint32_t);
    uint8_t _enablePin;
    uint8_t _iN1Pin;
    uint8_t _iN2Pin;
};

#endif 