#include "legManager.h"
#include "leds.h"
#include "esp32-hal-gpio.h"
#include <Bluepad32.h>
#include <Arduino.h>
#include <uni_log.h>


LegManager::LegManager(LedManager *ledManager,
    uint8_t enableAPin, uint8_t iNA1Pin, uint8_t iNA2Pin,
    uint8_t enableBPin, uint8_t iNB1Pin, uint8_t iNB2Pin,
    uint8_t enableCPin, uint8_t iNC1Pin, uint8_t iNC2Pin,
    uint8_t enableDPin, uint8_t iND1Pin, uint8_t iND2Pin): 
    _ledManager(ledManager) {
    _legA = new Leg(enableAPin, iNA1Pin, iNA2Pin, "MOTOR-A");
    _legB = new Leg(enableBPin, iNB1Pin, iNB2Pin, "MOTOR-B");
    _legC = new Leg(enableCPin, iNC1Pin, iNC2Pin, "MOTOR-C");
    _legD = new Leg(enableDPin, iND1Pin, iND2Pin, "MOTOR-D");
    }

void LegManager::init(void){
    _legA->init();
    _legB->init();
    _legC->init();
    _legD->init();
}

void LegManager::brakeS(void) {
    _legA->brakeS();
    _legB->brakeS();
    _legC->brakeS();
    _legD->brakeS();
}

void LegManager::process(ControllerPtr cptr) {

    int32_t value = cptr->axisRX();
    if (value > 100){
        _legA->left(value >> 1);
    } else if (value < -100){
        _legA->right(abs(value)  >> 1);
    }else { _legA->brakeS();
    }
    value = cptr->axisRY();
    if (value > 100){
        _legB->left(value  >> 1);
    } else if (value < -100){
        _legB->right(abs(value)  >> 1);
    }else { _legB->brakeS();
    }
    value = cptr->axisX();
    if (value > 100){
        _legC->left(value  >> 1);
    } else if (value < -100){
        _legC->right(abs(value)  >> 1);
    }else { _legC->brakeS();
    }
    value = cptr->axisY();
    if (value > 100){
        _legD->left(value  >> 1);
    } else if (value < -100){
        _legD->right(abs(value)  >> 1);
    }else { _legD->brakeS();
    }


    ControllerSimplePtr simplePtr= new ControllerSimple(cptr->getData());
   // delete _previousControler;
    _previousControler =  simplePtr;
}
