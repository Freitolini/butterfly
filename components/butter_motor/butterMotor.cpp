#include "butterMotor.h"
#include "esp32-hal-gpio.h"
#include <Bluepad32.h>
#include <Arduino.h>
#include <uni_log.h>


ButterMotor::ButterMotor(LedManager *ledManager,uint8_t enablePin, uint8_t iN1Pin, uint8_t iN2Pin) : 
_ledManager(ledManager), _enablePin(enablePin), _iN1Pin(iN1Pin), _iN2Pin(iN2Pin) {}

void ButterMotor::init(void){
    digitalWrite(_enablePin,LOW);
    digitalWrite(_iN1Pin,LOW);
    digitalWrite(_iN2Pin,LOW);
    pinMode(_enablePin, OUTPUT);
    pinMode(_iN1Pin, OUTPUT);
    pinMode(_iN2Pin, OUTPUT);
    _maintain = false;
    _speedMode = A;
    _direction = true;
    _speed = 0;
}

void ButterMotor::brakeS(void) {
    analogWrite(_enablePin,0);
    digitalWrite(_iN1Pin,LOW);
    digitalWrite(_iN2Pin,LOW);
}

void ButterMotor::lock(void) {}

void ButterMotor::unlock(void) {}

uint32_t ButterMotor::calculateSpeed(uint32_t speed){
    switch (_speedMode){
        case A:
            return speed >> 2;        
        case B:
            return speed >> 1;
        case C:
            return speed;
        default:
            return 0;
    };
}


void ButterMotor::forward(uint32_t speed) {
    _speed = calculateSpeed(speed);
    _direction = true;
    logi("forward: %d\n", _speed);
    analogWrite(_enablePin,_speed);
    digitalWrite(_iN1Pin,LOW);
    digitalWrite(_iN2Pin,HIGH);
}

void ButterMotor::reverse(uint32_t speed) {
    _speed = calculateSpeed(speed);
    _direction = false;
    logi("reverse: %d\n", _speed);
    analogWrite(_enablePin,_speed);
    digitalWrite(_iN1Pin,HIGH);
    digitalWrite(_iN2Pin,LOW);
}

void ButterMotor::processSpeedMode(ControllerPtr cptr){
    if (cptr->a() && (cptr->a() != _previousControler->a()) ){        
        switch (_speedMode)
        {
        case A:
        _speedMode = B;
            break;
        case B:
        _speedMode = C;
            break;
        case C:
        _speedMode = A;
            break;
        default:
            break;
        }
    }

}

void ButterMotor::processSpeedDir(ControllerPtr cptr){
    if (cptr->l1()){
        _maintain = true;
    }
    else if (cptr->r1()){
        _maintain = false;
    }

    if (_maintain == true){
        return;
    }
    int32_t throttle = cptr->throttle() >> 1;
    if (throttle > 60){
        forward(throttle);
        return;
    }  

    int32_t brake = cptr->brake() >> 1;
    if (brake > 60){
        reverse(brake);
        return;
    }  
    brakeS();
}



void ButterMotor::process(ControllerPtr cptr) {
    if (_previousControler != NULL){
        processSpeedMode(cptr);
    }
    _ledManager->setSpeedMode(static_cast<uint8_t>(_speedMode),_direction);
    processSpeedDir(cptr);
    ControllerSimplePtr simplePtr= new ControllerSimple(cptr->getData());
    _previousControler =  simplePtr;
}
