#pragma once

#include <Arduino.h>
#include <map>
#include "../Pot/Pot.h"
#include "../SystemState/SystemState.h"


class Mixer {
public:
    static const int LEFT = 36;
    static const int RIGHT = 34;
    static const int POT_COUNT = 2;
    static const int MIXER_TASK_DELAY = 20;

    Pot pots[POT_COUNT] = {
        Pot(LEFT),
        Pot(RIGHT)
    };

    void init();

    static long potPercent(int val);

private:
    void run();
    static void taskEntry(void *param);
};