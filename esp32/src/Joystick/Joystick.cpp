#include "./Joystick.h"
#include "../SystemState/SystemState.h"

void Joystick::init() {
  pinMode(Joystick::SW_PIN, INPUT);
};

void Joystick::run() {
  for(;;) {
    int switchBtnState = digitalRead(Joystick::SW_PIN);
 
    if(switchBtnState == HIGH) {
      if(xSemaphoreTake(systemStateMutex, portMAX_DELAY)) {
        systemState.joystickState = SWITCH;
      }
    }

    else {
      if(xSemaphoreTake(systemStateMutex, portMAX_DELAY)) {
        systemState.joystickState = NONE;
      }
    }

    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
};

void Joystick::taskEntry(void *param) {
  Joystick *self = static_cast<Joystick *>(param);
  self->run();
};