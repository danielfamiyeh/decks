#include "./Buttons.h"
#include "../Shared/Shared.h"
#include "../SystemState/SystemState.h"

void Buttons::init() {
  pinMode(Buttons::BUTTON_MODE_PIN, INPUT_PULLUP);
  xTaskCreatePinnedToCore(taskEntry, "ButtonsTask", 10000, this, 1, &ButtonTask, 0);
}

void Buttons::run(){
  for(;;){
    int buttonState = digitalRead(Buttons::BUTTON_MODE_PIN);
    bool wasReleased = lastButtonState == HIGH;
    bool isPressed = buttonState == LOW;

    if(xSemaphoreTake(systemStateMutex, portMAX_DELAY)) {
      bool buttonChanged = systemState.modeButtonState != buttonState;
      systemState.modeButtonState = buttonState;

      if(wasReleased && isPressed) {
        systemState.screenMode =
          (systemState.screenMode == MIXER_MODE) ? SETTINGS_MODE : MIXER_MODE;
        systemState.screenDirty = true;
      }
      if(buttonChanged) {
        systemState.screenDirty = true;
      }
      xSemaphoreGive(systemStateMutex);
    }

    lastButtonState = buttonState;
    vTaskDelay(Buttons::BUTTON_TASK_DELAY / portTICK_PERIOD_MS);
  }
}

void Buttons::taskEntry(void *param) {
  Buttons *self = static_cast<Buttons *>(param);
  self->run();
}
