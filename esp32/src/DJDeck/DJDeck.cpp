#include "DJDeck.h"
#include "../SystemState/SystemState.h"

void DJDeck::init() {
    Serial.begin(115200);

    systemStateMutex = xSemaphoreCreateMutex();
    joystick.init();
    screen.init();
    mixer.init();
}