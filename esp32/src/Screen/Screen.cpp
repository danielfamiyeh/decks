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

    if (strncmp(lastRendered, text, Screen::LINE_LENGTH) == 0) {
        return;
    }

    clearLine(row);
    lcd.print(text);
    strncpy(lastRendered, text, Screen::LINE_LENGTH);
    lastRendered[Screen::LINE_LENGTH] = '\0';
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
    char row0[Screen::LINE_LENGTH + 1];
    char row1[Screen::LINE_LENGTH + 1];

    int eqIdx = static_cast<int>(state.mixer.level);

    snprintf(row0, sizeof(row0), "Level: %-8s", Screen::levelTitles[eqIdx]);
    snprintf(row1, sizeof(row1), "L:%03d%%    R:%03d%%", state.mixer.eq[eqIdx].leftPercent, state.mixer.eq[eqIdx].rightPercent);

    renderLine(0, row0);
    renderLine(1, row1);
}

void Screen::renderJoystickDebug(const SystemState& state) {
    char row0[Screen::LINE_LENGTH + 1];
    char row1[Screen::LINE_LENGTH + 1];

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
