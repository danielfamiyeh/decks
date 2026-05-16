#include <Arduino.h>
#include "../SystemState/SystemState.h"

class Joystick {
  public:
    static const int SW_PIN = 26;
    static const int VRx_PIN = 25;
    static const int VRy_PIN = 4;
    static const int JOYSTICK_THRESHOLD = 100;
    static const int JOYSTICK_TASK_DELAY = 150;


  void init();

  void read();

  private:
    void run();
    static void taskEntry(void *param);
};