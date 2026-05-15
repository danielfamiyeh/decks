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

        if (xSemaphoreTake(systemStateMutex, portMAX_DELAY)) {
            bool leftChanged = abs(systemState.mixer.leftPercent - leftPercentNow) > Pot::POT_THRESHOLD;
            bool rightChanged = abs(systemState.mixer.rightPercent - rightPercentNow) > Pot::POT_THRESHOLD;

            if (leftChanged) {
                systemState.mixer.leftPercent = leftPercentNow;
            }

            if (rightChanged) {
                systemState.mixer.rightPercent = rightPercentNow;
            }

            if (leftChanged || rightChanged) {
                systemState.screenDirty = true;
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
