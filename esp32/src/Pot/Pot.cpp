#include "Pot.h"

Pot::Pot(int inputPin) {
    pin = inputPin;
    last = 0;
}

int Pot::read() {
    delayMicroseconds(200);

    int total = 0;
    const int sampleCount = 4;

    for (int i = 0; i < sampleCount; i++) {
        total += analogRead(pin);
        delayMicroseconds(200);
    }

    last = total / sampleCount;
    return last;
}
