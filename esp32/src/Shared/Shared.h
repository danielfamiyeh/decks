#pragma once 

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

extern TaskHandle_t ScreenTask;
extern TaskHandle_t MixingTask;
extern TaskHandle_t JoystickTask;
extern TaskHandle_t ButtonTask;