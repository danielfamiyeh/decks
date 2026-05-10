#pragma once

#include <Arduino.h>

enum MixerLevel { LOWS, MIDS, HIGHS };
enum ScreenMode { MIXER_VIEW, SETTINGS_VIEW };
enum JoystickState { NONE, LEFT, RIGHT, UP, DOWN, SWITCH };
struct MixerState {
    int leftPercent = 0;
    int rightPercent = 0;
    MixerLevel level = LOWS;
};

struct SystemState {
    MixerState mixer;
    ScreenMode screenMode = MIXER_VIEW;
    JoystickState joystickState;
    bool screenDirty = true;
};

extern SystemState systemState;
extern SemaphoreHandle_t systemStateMutex;