#pragma once

#include <Arduino.h>
#include "../Mixer/Mixer.h"
#include "../Screen/Screen.h"
#include "../Joystick/Joystick.h"

class DJDeck {
private:
    Mixer mixer;
    Screen screen;
    Joystick joystick;

public:
    void init();
};