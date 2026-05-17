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
    static const int LEFT_MIN = 172;
    static const int LEFT_MAX = 4095;
    static const int RIGHT_MIN = 172;
    static const int RIGHT_MAX = 4095;
    static const int NUM_MIXER_LEVELS = 3;

    Pot pots[POT_COUNT] = {
        Pot(LEFT),
        Pot(RIGHT)
    };

    void init();

    static long potPercent(int val, int minVal, int maxVal);

private:
    void run();
    static void taskEntry(void *param);
};
