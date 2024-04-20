#include "leds.h"
#include "esp32-hal-gpio.h"
#include <Bluepad32.h>
#include <Arduino.h>
#include <uni_log.h>



LedManager::LedManager(uint16_t ledPin, uint16_t ledNumber): _ledPin(ledPin),  _ledNumber(ledNumber) {}


void LedManager::init(void){
    this->pixels = new Adafruit_NeoPixel(_ledNumber, _ledPin, NEO_GRB + NEO_KHZ800);
    this->pixels->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    this->pixels->clear(); // Set all pixel colors to 'off'}
    pixels->setBrightness(100);
    pixels->setPixelColor(POWER_LED, pixels->Color(255, 0, 0));
    pixels->setPixelColor(SPEED_MODE_LED, pixels->Color(0, 0, 0));
    pixels->setPixelColor(BLUETOOTH_LED, pixels->Color(0, 0, 0));
    pixels->show(); 
}

void LedManager::updateLeds(void){
    pixels->show(); 
}

void LedManager::setSpeedMode(uint8_t speedMode, bool direction){
    if (direction){
        pixels->setPixelColor(SPEED_MODE_LED, pixels->Color(0, SPEED_MODE_STEP*speedMode, 0));
    }
    else {
        pixels->setPixelColor(SPEED_MODE_LED, pixels->Color(0, 0, SPEED_MODE_STEP*speedMode));
    }
}

void LedManager::setVBat(uint16_t vbat ){
    pixels->setPixelColor(POWER_LED, pixels->Color((vbat>>8)*VBAT_STEP,0 , 0));
}

void LedManager::setSleep(void){
    pixels->setPixelColor(POWER_LED, pixels->Color(76, 0, 173));
    pixels->setPixelColor(SPEED_MODE_LED, pixels->Color(76, 0, 173));
    pixels->setPixelColor(BLUETOOTH_LED, pixels->Color(76, 0, 173));
}

void LedManager::setBluetooth(bool connected){
    if (connected){
        pixels->setPixelColor(BLUETOOTH_LED, pixels->Color(0,0,255));
    }
    else{
        pixels->setPixelColor(BLUETOOTH_LED, pixels->Color(50, 50, 255));
    }

}
