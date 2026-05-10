#pragma once

#include <Arduino.h>
#include "../SystemState/SystemState.h"

class Screen {
public:
    char* levelTitles[3] = {"Lows", "Mids", "Highs"};

    void init();
    void run();

    static void clearLine(int row);

private:
    static void taskEntry(void *param);
    void render(const SystemState& state);
    void renderMixer(const MixerState& mixer);
};