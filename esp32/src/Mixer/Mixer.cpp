#include "Mixer.h"
#include <algorithm>

#include "../Shared/Shared.h"
#include "../Screen/Screen.h"

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

        if (maxDelta > 100) {
            Screen::clearLine(1);

            lcd.print("L:");
            lcd.print(Mixer::potPercent(leftDelta > 300 ? lPotNow : lPotLast));
            lcd.print("%   ");

            lcd.print("R:");
            lcd.print(Mixer::potPercent(rightDelta > 300 ? rPotNow : rPotLast));
            lcd.print("%");
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void Mixer::taskEntry(void *param) {
    Mixer *self = static_cast<Mixer *>(param);
    self->run();
}