#include "Mixer.h"
#include <algorithm>

#include "../Shared/Shared.h"
#include "../SystemState/SystemState.h"

void Mixer::init() {
    xTaskCreatePinnedToCore(taskEntry, "MixingTask", 10000, this, 1, &MixingTask, 0);
}

long Mixer::potPercent(int val, int minVal, int maxVal) {
    int constrainedVal = constrain(val, minVal, maxVal);
    return map(constrainedVal, minVal, maxVal, 0, 100);
}

void Mixer::run() {
    for (;;) {
        int lPotNow = pots[0].read();
        int rPotNow = pots[1].read();

        int leftPercentNow = Mixer::potPercent(lPotNow, Mixer::LEFT_MIN, Mixer::LEFT_MAX);
        int rightPercentNow = Mixer::potPercent(rPotNow, Mixer::RIGHT_MIN, Mixer::RIGHT_MAX);
        int eqIdx = static_cast<int>(systemState.mixer.level);

        if (xSemaphoreTake(systemStateMutex, portMAX_DELAY)) {
            bool leftChanged = abs(systemState.mixer.eq[eqIdx].leftPercent - leftPercentNow) > Pot::POT_THRESHOLD;
            bool rightChanged = abs(systemState.mixer.eq[eqIdx].rightPercent - rightPercentNow) > Pot::POT_THRESHOLD;

            if (leftChanged) {
                systemState.mixer.eq[eqIdx].leftPercent = leftPercentNow;
            }

            if (rightChanged) {
                systemState.mixer.eq[eqIdx].rightPercent = rightPercentNow;
            }

            if (leftChanged || rightChanged) {
                systemState.screenDirty = true;
            }

            if (systemState.joystickState.direction == JOYSTICK_UP) {
                systemState.joystickState.direction = JOYSTICK_NULL;
                systemState.mixer.level = static_cast<MixerLevel>((eqIdx + 1) % Mixer::NUM_MIXER_LEVELS);
            }

            if (systemState.joystickState.direction == JOYSTICK_DOWN) {
                systemState.joystickState.direction = JOYSTICK_NULL;
                systemState.mixer.level = static_cast<MixerLevel>(((eqIdx - 1) % Mixer::NUM_MIXER_LEVELS + 3) % Mixer::NUM_MIXER_LEVELS);
            }

            // else if (systemState.joystickState.direction == JOYSTICK_DOWN) {
            //     systemState.mixer.viewState = static_cast<MixerViewState>(static_cast<int>(systemState.mixer.viewState - 1 + Mixer::NUM_MIXER_LEVELS) % Mixer::NUM_MIXER_LEVELS);
            // }

            // else if (systemState.joystickState == JOYSTICK_DOWN) {
            //     systemState.mixer.viewState -= 1;
            //     systemState.mixer.viewState %= Mixer::NUM_MIXER_LEVELS;
            // }

            xSemaphoreGive(systemStateMutex);
        }

        vTaskDelay(Mixer::MIXER_TASK_DELAY / portTICK_PERIOD_MS);
    }
}

void Mixer::taskEntry(void *param) {
    Mixer *self = static_cast<Mixer *>(param);
    self->run();
}
