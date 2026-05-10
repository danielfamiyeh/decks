#pragma once

#include <Arduino.h>

enum MixerLevel { LOWS, MIDS, HIGHS };
enum ScreenMode { MIXER_VIEW, SETTINGS_VIEW, DEBUG_JOYSTICK_VIEW };

struct JoystickState {
    int x = -1;
    int y = -1;
    int btn = LOW;
};

struct MixerState {
    int leftPercent = 0;
    int rightPercent = 0;
    MixerLevel level = LOWS;
};

struct SystemState {
    MixerState mixer;
    ScreenMode screenMode = DEBUG_JOYSTICK_VIEW;
    JoystickState joystickState;
    bool screenDirty = true;
};

extern SystemState systemState;
extern SemaphoreHandle_t systemStateMutex;