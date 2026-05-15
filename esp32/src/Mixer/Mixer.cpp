#include "Mixer.h"
#include <algorithm>

#include "../Shared/Shared.h"
#include "../SystemState/SystemState.h"

void Mixer::init() {
    xTaskCreatePinnedToCore(taskEntry, "MixingTask", 10000, this, 1, &MixingTask, 0);
}

long Mixer::potPercent(int val) {
    return map(val, 0, 4095, 0, 100);
}

void Mixer::run() {
    for (;;) {
        int lPotNow = pots[0].read();
        int rPotNow = pots[1].read();

        int leftPercentNow = Mixer::potPercent(lPotNow);
        int rightPercentNow = Mixer::potPercent(rPotNow);

        if (xSemaphoreTake(systemStateMutex, portMAX_DELAY)) {
            bool leftChanged = systemState.mixer.leftPercent != leftPercentNow;
            bool rightChanged = systemState.mixer.rightPercent != rightPercentNow;

            if (leftChanged || rightChanged) {
                systemState.mixer.leftPercent = leftPercentNow;
                systemState.mixer.rightPercent = rightPercentNow;
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
