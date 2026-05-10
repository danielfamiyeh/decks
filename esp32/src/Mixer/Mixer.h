#pragma once

#include <Arduino.h>
#include <map>
#include "../Pot/Pot.h"
#include "../SystemState/SystemState.h"


class Mixer {
public:
    static const int LEFT = 35;
    static const int RIGHT = 33;
    static const int POT_COUNT = 2;
    
    int level = 0;

    Pot pots[POT_COUNT] = {
        Pot(LEFT, 100),
        Pot(RIGHT, 200)
    };

    void init();

    static long potPercent(int val);

private:
    void run();
    static void taskEntry(void *param);
};