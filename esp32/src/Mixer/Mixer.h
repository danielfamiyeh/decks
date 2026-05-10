#pragma once

#include <Arduino.h>
#include <map>
#include "../Pot/Pot.h"

enum MixerLevel { LOWS, MIDS, HIGHS };

class Mixer {
public:
    static const int LEFT = 35;
    static const int RIGHT = 33;
    static const int POT_COUNT = 2;

    MixerLevel level = MixerLevel::LOWS;

    std::map<MixerLevel, String> LevelText = {
        {MixerLevel::LOWS, "Lows"},
        {MixerLevel::MIDS, "Mids"},
        {MixerLevel::HIGHS, "Highs"}
    };

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