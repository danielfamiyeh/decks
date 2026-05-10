#include "DJDeck.h"
#include "../SystemState/SystemState.h"

void DJDeck::init() {
    Serial.begin(115200);

    systemStateMutex = xSemaphoreCreateMutex();

    screen.init();
    mixer.init();
}