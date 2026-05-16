#pragma once

#include <Arduino.h>

enum ScreenMode { MIXER_MODE, SETTINGS_MODE };
enum MixerLevel { MIXER_LOWS, MIXER_MIDS, MIXER_HIGHS };
enum SettingsViewState { DEBUG_JOYSTICK, DEBUG_POTS };
enum JoystickDirection { JOYSTICK_NULL, JOYSTICK_UP, JOYSTICK_DOWN, JOYSTICK_LEFT, JOYSTICK_RIGHT };

struct JoystickState {
    int x = -1;
    int y = -1;
    int btn = LOW;
    bool isLocked = false;
    JoystickDirection direction = JOYSTICK_NULL;
};

struct MixerEQ {
    int leftPercent = 0;
    int rightPercent = 0;
};

struct MixerState {
    MixerEQ eq[3];
    MixerLevel level = MIXER_LOWS;
};

struct SystemState {
    MixerState mixer;
    SettingsViewState settingsViewState = DEBUG_JOYSTICK;
    ScreenMode screenMode = MIXER_MODE;
    JoystickState joystickState;
    int modeButtonState = HIGH;
    int screenToggle;
    bool screenDirty = true;
};
 
extern SystemState systemState;
extern SemaphoreHandle_t systemStateMutex;
