#pragma once

#include <Arduino.h>
#include "../SystemState/SystemState.h"

class Buttons {
  public:
    static const int BUTTON_MODE_PIN = 0;
    static const int BUTTON_TASK_DELAY = 20;

    void init();

    private:
      int lastButtonState = HIGH;
      void run();
      static void taskEntry(void *param);
};
