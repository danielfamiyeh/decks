#include "Mixer.h"
#include <algorithm>

#include "../Shared/Shared.h"
#include "../SystemState/SystemState.h"

void Mixer::init() {
    xTaskCreatePinnedToCore(taskEntry, "MixingTask", 10000, this, 1, &MixingTask, 0);
}

long Mixer::potPercent(int val, int minVal, int maxVal) {
    int percent = map(val, 0, 4095, 0, 100);
    percent = constrain(percent, 0, 100);

    return percent;
}

void Mixer::run() {
    for (;;) {
        int lPotNow = pots[0].read();
        int rPotNow = pots[1].read();

        int leftPercentNow = Mixer::potPercent(lPotNow, Mixer::LEFT_MIN, Mixer::LEFT_MAX);
        int rightPercentNow = Mixer::potPercent(rPotNow, Mixer::RIGHT_MIN, Mixer::RIGHT_MAX);
        
        int eqIdx = static_cast<int>(systemState.mixer.level);
        
        MixerEQ *systemMixerEq = &systemState.mixer.eq[eqIdx];

        if (xSemaphoreTake(systemStateMutex, portMAX_DELAY)) {
            bool leftChanged = abs(systemState.mixer.eq[eqIdx].leftPercent - leftPercentNow) > Pot::POT_THRESHOLD;
            bool rightChanged = abs(systemState.mixer.eq[eqIdx].rightPercent - rightPercentNow) > Pot::POT_THRESHOLD;

            int leftPotDrift = leftPercentNow - systemMixerEq->leftPercent;
            int rightPotDrift = rightPercentNow - systemMixerEq->rightPercent;

            if(abs(leftPotDrift) <= 20) {
                systemMixerEq->leftIsCaught = true;
                systemMixerEq->leftDriftDir = 0;
            } else {
                systemMixerEq->leftDriftDir = leftPotDrift < 0 ? -1 : 1;
                systemMixerEq->leftIsCaught = false;
                
            }

            if(abs(rightPotDrift) <= 20) {
                systemMixerEq->rightIsCaught = true;
                systemMixerEq->rightDriftDir = 0;
            } else {
                systemMixerEq->leftDriftDir = rightPotDrift < 0 ? -1 : 1;
                systemMixerEq->rightIsCaught = false;
            }

            if (leftChanged && systemMixerEq->leftIsCaught) {
                systemMixerEq->leftPercent = leftPercentNow;
            }

            if (rightChanged && systemMixerEq->rightIsCaught) {
                systemMixerEq->rightPercent = rightPercentNow;
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

            xSemaphoreGive(systemStateMutex);
        }

        vTaskDelay(Mixer::MIXER_TASK_DELAY / portTICK_PERIOD_MS);
    }
}

void Mixer::taskEntry(void *param) {
    Mixer *self = static_cast<Mixer *>(param);
    self->run();
}
