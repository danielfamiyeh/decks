#include <Arduino.h>

class UltrasonicSensor {
  public:
    static const int ULTRASONIC_SENSOR_TRIGGER_PIN = 27;
    static const int ULTRASONIC_SENSOR_TASK_DELAY = 0.01; //10us
    void init();

  private:
    void run();
    static void taskEntry(void *param);
};