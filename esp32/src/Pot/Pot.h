#pragma once
#include <Arduino.h>

class Pot {
  public:
    int pin;
    int last;

    Pot(int inputPin, int threshold);
    int read();
};
