#include <Arduino.h>
#include "../SystemState/SystemState.h"

class Joystick {
  public:
    static const int SW_PIN = 5;

    void init();

  private:
    void run();
    static void taskEntry(void *param);
};