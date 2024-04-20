#include "leg.h"
#include "esp32-hal-gpio.h"
#include <Bluepad32.h>
#include <Arduino.h>
#include <uni_log.h>


Leg::Leg(uint8_t enablePin, uint8_t iN1Pin, uint8_t iN2Pin, String label): 
    _enablePin(enablePin), _iN1Pin(iN1Pin), _iN2Pin(iN2Pin), _label(label) {}

void Leg::init(void){
    digitalWrite(_enablePin,LOW);
    digitalWrite(_iN1Pin,LOW);
    digitalWrite(_iN2Pin,LOW);
    pinMode(_enablePin, OUTPUT);
    pinMode(_iN1Pin, OUTPUT);
    pinMode(_iN2Pin, OUTPUT);
}

void Leg::brakeS(void) {
    analogWrite(_enablePin,0);
    digitalWrite(_iN1Pin,LOW);
    digitalWrite(_iN2Pin,LOW);
}

void Leg::left(uint16_t speed){
    logi("%s left: %d\n", _label, speed);
    analogWrite(_enablePin,speed);
    digitalWrite(_iN1Pin,HIGH);
    digitalWrite(_iN2Pin,LOW);
}
void Leg::right(uint16_t speed){
    logi("%s right: %d\n", _label, speed);
    analogWrite(_enablePin,speed);
    digitalWrite(_iN1Pin,LOW);
    digitalWrite(_iN2Pin,HIGH);
}
