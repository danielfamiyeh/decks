#pragma once

#include <Arduino.h>

class Screen {
public:
    void init();
    void run();

    static void clearLine(int row);

private:
    static void taskEntry(void *param);
};