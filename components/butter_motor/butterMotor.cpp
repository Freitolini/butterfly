#include "butterMotor.h"
#include "esp32-hal-gpio.h"
#include <Bluepad32.h>
#include <Arduino.h>
#include <uni_log.h>


ButterMotor::ButterMotor(uint8_t enablePin, uint8_t iN1Pin, uint8_t iN2Pin) : _enablePin(enablePin), _iN1Pin(iN1Pin), _iN2Pin(iN2Pin) {}

void ButterMotor::init(void){
    digitalWrite(_enablePin,LOW);
    digitalWrite(_enablePin,LOW);
    digitalWrite(_enablePin,LOW);
    pinMode(_enablePin, OUTPUT);
    pinMode(_iN1Pin, OUTPUT);
    pinMode(_iN2Pin, OUTPUT);
}



void ButterMotor::brakeS(void) {
    analogWrite(_enablePin,0);
    digitalWrite(_iN1Pin,LOW);
    digitalWrite(_iN2Pin,LOW);
}

void ButterMotor::lock(void) {}

void ButterMotor::unlock(void) {}

void ButterMotor::forward(uint32_t speed) {
    logi("forward: %d\n", speed);
    //Console.printf("forward: %4d\n",speed);
    analogWrite(_enablePin,speed);
    digitalWrite(_iN1Pin,LOW);
    digitalWrite(_iN2Pin,HIGH);
}

void ButterMotor::reverse(uint32_t speed) {
    logi("reverse: %d\n", speed);
    //Console.printf("reverse: %4d\n",speed);
    analogWrite(_enablePin,speed);
    digitalWrite(_iN1Pin,HIGH);
    digitalWrite(_iN2Pin,LOW);
}

void ButterMotor::process(ControllerPtr cptr) {
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
