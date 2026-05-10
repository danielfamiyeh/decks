#include "Screen.h"
#include "../Shared/Shared.h"
#include "../SystemState/SystemState.h"

void Screen::clearLine(int row) {
    lcd.setCursor(0, row);
    lcd.print("                ");
    lcd.setCursor(0, row);
}

void Screen::init() {
    lcd.init();
    lcd.backlight();

    xTaskCreatePinnedToCore(
        taskEntry,
        "ScreenTask",
        10000,
        this,
        1,
        &ScreenTask,
        1
    );
}

void Screen::render(const SystemState& state) {
    if (state.screenMode == MIXER_VIEW) {
        renderMixer(state.mixer);
    }
}

void Screen::renderMixer(const MixerState& mixer) {
    clearLine(0);
    lcd.print("Mode: ");

    if (mixer.level == LOWS) lcd.print("Lows");
    if (mixer.level == MIDS) lcd.print("Mids");
    if (mixer.level == HIGHS) lcd.print("Highs");

    clearLine(1);
    lcd.print("L:");
    lcd.print(mixer.leftPercent);
    lcd.print("%   R:");
    lcd.print(mixer.rightPercent);
    lcd.print("%");
}

void Screen::run() {
    for (;;) {
        SystemState snapshot;
        bool shouldRender = false;

        if (xSemaphoreTake(systemStateMutex, portMAX_DELAY)) {
            shouldRender = systemState.screenDirty;

            if (shouldRender) {
                snapshot = systemState;
                systemState.screenDirty = false;
            }

            xSemaphoreGive(systemStateMutex);
        }

        if (shouldRender) {
            render(snapshot);
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void Screen::taskEntry(void *param) {
    Screen *self = static_cast<Screen *>(param);
    self->run();
}