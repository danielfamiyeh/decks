#include "Mixer.h"
#include <algorithm>

#include "../Shared/Shared.h"
#include "../Screen/Screen.h"
#include "../SystemState/SystemState.h"

void Mixer::init() {
    xTaskCreatePinnedToCore(
        taskEntry,
        "MixingTask",
        10000,
        this,
        1,
        &MixingTask,
        0
    );
}

long Mixer::potPercent(int val) {
    return map(val, 0, 4095, 0, 100);
}

void Mixer::run() {
    for (;;) {
        int lPotLast = pots[0].last;
        int rPotLast = pots[1].last;

        int lPotNow = pots[0].read();
        int rPotNow = pots[1].read();

        int leftDelta = abs(lPotNow - lPotLast);
        int rightDelta = abs(rPotNow - rPotLast);
        int maxDelta = std::max(leftDelta, rightDelta);

        if (maxDelta > 100 && xSemaphoreTake(systemStateMutex, portMAX_DELAY)) {
            systemState.mixer.leftPercent = Mixer::potPercent(lPotNow);
            systemState.mixer.rightPercent = Mixer::potPercent(rPotNow);
            systemState.screenDirty = true;

            xSemaphoreGive(systemStateMutex);
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void Mixer::taskEntry(void *param) {
    Mixer *self = static_cast<Mixer *>(param);
    self->run();
}