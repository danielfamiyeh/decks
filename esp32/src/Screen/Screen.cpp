#include "Screen.h"
#include "../Shared/Shared.h"

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

void Screen::run() {
    for (;;) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void Screen::taskEntry(void *param) {
    Screen *self = static_cast<Screen *>(param);
    self->run();
}