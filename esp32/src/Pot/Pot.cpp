#include "Pot.h"

Pot::Pot(int inputPin, int threshold) {
    pin = inputPin;
    last = 0;
}

int Pot::read() {
    int total = 0;

    for (int i = 0; i < 10; i++) {
        total += analogRead(pin);
        delayMicroseconds(200);
    }

    last = total / 10;
    return last;
}
