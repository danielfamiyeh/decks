#pragma once

#include <Arduino.h>
#include <cstring>
#include "../SystemState/SystemState.h"

class Screen {
public:
    static const int LINE_LENGTH = 16;

    const char* levelTitles[3] = {"Lows", "Mids", "Highs"};

    void init();
    void run();

    static void clearLine(int row);

private:
    char lastRenderedRow0[17] = "";
    char lastRenderedRow1[17] = "";

    static void taskEntry(void *param);
    void renderLine(int row, const char* text);
    void render(const SystemState& state);
    void renderMixer(const SystemState& state);
    void renderJoystickDebug(const SystemState& state);
};
