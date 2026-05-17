#pragma once

#include <Arduino.h>
#include "../Mixer/Mixer.h"
#include "../Screen/Screen.h"
#include "../Joystick/Joystick.h"
#include "../Buttons/Buttons.h"
#include "../UltrasonicSensor/UltrasonicSensor.h"

class DJDeck {
private:
    Mixer mixer;
    Screen screen;
    Joystick joystick;
    Buttons buttons;
    UltrasonicSensor ultrasonicSensor;

public:
    void init();
};
