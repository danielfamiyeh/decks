#pragma once
#include <Arduino.h>

class Pot {
  public:
    int pin;
    int last;

    static const int POT_THRESHOLD = 10;

    Pot(int inputPin);
    int read();
};
