#include "Screen.h"
#include <cstdio>
#include "../Shared/Shared.h"
#include "../SystemState/SystemState.h"
#include "../Mixer/Mixer.h"
#include "../Joystick/Joystick.h"

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

void Screen::renderLine(int row, const char* text) {
    char* lastRendered = (row == 0) ? lastRenderedRow0 : lastRenderedRow1;

    if (strncmp(lastRendered, text, 16) == 0) {
        return;
    }

    clearLine(row);
    lcd.print(text);
    strncpy(lastRendered, text, 16);
    lastRendered[16] = '\0';
}

void Screen::render(const SystemState& state) {
    if (state.screenMode == MIXER_MODE) {
        renderMixer(state);
    }
    if (state.screenMode == SETTINGS_MODE) {
        renderJoystickDebug(state);
    }
}

void Screen::renderMixer(const SystemState& state) {
    char row0[17];
    char row1[17];

    snprintf(row0, sizeof(row0), "Level: %-8s", Screen::levelTitles[state.mixer.viewState]);
    snprintf(row1, sizeof(row1), "L:%3d%% R:%3d%%", state.mixer.leftPercent, state.mixer.rightPercent);

    renderLine(0, row0);
    renderLine(1, row1);
}

void Screen::renderJoystickDebug(const SystemState& state) {
    char row0[17];
    char row1[17];

    snprintf(row0, sizeof(row0), "Debug: Joystick");
    snprintf(row1, sizeof(row1), "x:%4d y:%4d", state.joystickState.x, state.joystickState.y);

    renderLine(0, row0);
    renderLine(1, row1);
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

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void Screen::taskEntry(void *param) {
    Screen *self = static_cast<Screen *>(param);
    self->run();
}
