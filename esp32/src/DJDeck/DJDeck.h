#pragma once

#include <Arduino.h>
#include "../Mixer/Mixer.h"
#include "../Screen/Screen.h"

class DJDeck {
private:
    Mixer mixer;
    Screen screen;

public:
    void init();
};