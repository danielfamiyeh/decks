#include "./Joystick.h"
#include "../Shared/Shared.h"
#include "../SystemState/SystemState.h"

void Joystick::init() {
  pinMode(Joystick::SW_PIN, INPUT_PULLUP);
  pinMode(Joystick::VRx_PIN, INPUT);

  xTaskCreatePinnedToCore(taskEntry, "JoystickTask", 10000, this, 0.9, &JoystickTask, 1);
};

void Joystick::run() {
  for(;;) {
    int switchBtnState = digitalRead(Joystick::SW_PIN);
    int vrX = analogRead(Joystick::VRx_PIN);
 
    
    if(xSemaphoreTake(systemStateMutex, portMAX_DELAY)) {
      if(switchBtnState != systemState.joystickState.btn) {
        systemState.joystickState.btn = switchBtnState;
        systemState.screenDirty = true;
      }
      xSemaphoreGive(systemStateMutex);
  }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
};

void Joystick::taskEntry(void *param) {
  Joystick *self = static_cast<Joystick *>(param);
  self->run();
};