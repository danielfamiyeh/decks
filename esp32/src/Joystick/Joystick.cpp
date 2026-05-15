#include "./Joystick.h"
#include "../Shared/Shared.h"
#include "../SystemState/SystemState.h"

void Joystick::init() {
  pinMode(Joystick::SW_PIN, INPUT_PULLUP);
  pinMode(Joystick::VRx_PIN, INPUT);
  pinMode(Joystick::VRy_PIN, INPUT);

  xTaskCreatePinnedToCore(taskEntry, "JoystickTask", 10000, this, 1, &JoystickTask, 1);
};

void Joystick::run() {
  for(;;) {
    int switchBtnState = digitalRead(Joystick::SW_PIN);
    int vrX = analogRead(Joystick::VRx_PIN);
    int vrY = analogRead(Joystick::VRy_PIN);
 
    
    if(xSemaphoreTake(systemStateMutex, portMAX_DELAY)) {
      if(switchBtnState != systemState.joystickState.btn ||
        abs(vrX - systemState.joystickState.x) > Joystick::JOYSTICK_THRESHOLD ||
        abs(vrY - systemState.joystickState.y) > Joystick::JOYSTICK_THRESHOLD
      ) {
        systemState.joystickState.x = vrX;
        systemState.screenDirty = true;
      }
      xSemaphoreGive(systemStateMutex);
  }
    vTaskDelay(Joystick::JOYSTICK_TASK_DELAY / portTICK_PERIOD_MS);
  }
};

void Joystick::taskEntry(void *param) {
  Joystick *self = static_cast<Joystick *>(param);
  self->run();
};