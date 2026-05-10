#include <Arduino.h>
#include "../SystemState/SystemState.h"

class Joystick {
  public:
    static const int SW_PIN = 26;
    static const int VRx_PIN = 25;
    static const int SCREEN_TOGGLE_PIN = 27;

    // char* joystickTitles[3] = {"None", "Left", 'Right', ""};

  void init();

  void read();

  private:
    void run();
    static void taskEntry(void *param);
};