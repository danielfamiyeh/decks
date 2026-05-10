#include "Shared.h"

LiquidCrystal_I2C lcd(0x27, 16, 4);

TaskHandle_t JoystickTask = nullptr;
TaskHandle_t ScreenTask = nullptr;
TaskHandle_t MixingTask = nullptr;
