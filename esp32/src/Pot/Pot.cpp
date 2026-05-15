#include "Pot.h"

Pot::Pot(int inputPin) {
    pin = inputPin;
    last = 0;
}

int Pot::read() {
    // ESP32 ADC channel switching can leak the previous channel into the next
    // sample, so discard the first read and average the following samples.
    analogRead(pin);
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
