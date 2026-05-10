#include "./Joystick.h"
#include "../SystemState/SystemState.h"

void Joystick::init() {
  pinMode(Joystick::SW_PIN, INPUT);
};

void Joystick::run() {
  for(;;) {
    int switchBtnState = digitalRead(Joystick::SW_PIN);

    if(xSemaphoreTake(systemStateMutex, portMAX_DELAY)) {
      if(switchBtnState == HIGH) {
        systemState.joystick = SWITCH;
      }
      
      else {
        systemState.joystick = NONE;
      }
    } 
  }
};

void Joystick::taskEntry(void *param) {
  Joystick *self = static_cast<Joystick *>(param);
  self->run();
};