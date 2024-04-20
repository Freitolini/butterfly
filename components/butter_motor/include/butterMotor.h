

#ifndef BUTTER_MOTOR_H
#define BUTTER_MOTOR_H

#include <stddef.h>
#include <pins_arduino.h>
#include <Bluepad32.h>
#include <leds.h>


class ButterMotor {
   public:
    ButterMotor(LedManager *ledManager,uint8_t enablePin, uint8_t iN1Pin, uint8_t iN2Pin);
    void init(void);
    void process(ControllerPtr cptr);
    void brakeS(void);
   private:
    void lock(void);
    void unlock(void);
    void forward(uint32_t);
    void reverse(uint32_t);
    void processSpeedMode(ControllerPtr cptr);
    void processSpeedDir(ControllerPtr cptr);
    uint32_t calculateSpeed(uint32_t);
    LedManager *_ledManager;
    ControllerSimplePtr _previousControler;
    uint8_t _enablePin;
    uint8_t _iN1Pin;
    uint8_t _iN2Pin;
    bool _direction;
    uint32_t _speed;
    bool _maintain;
    enum SpeedMode{
        A=1,B=2,C
    };
    SpeedMode _speedMode;

};

#endif 