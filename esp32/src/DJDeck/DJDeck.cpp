#include "DJDeck.h"

void DJDeck::init() {
    Serial.begin(115200);

    screen.init();
    mixer.init();
}