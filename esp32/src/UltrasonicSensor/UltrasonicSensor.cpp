#include "UltrasonicSensor.h"
#include "../Shared/Shared.h"
#include "../SystemState/SystemState.h"

void UltrasonicSensor::init(){
  pinMode(UltrasonicSensor::ULTRASONIC_SENSOR_TRIGGER_PIN, OUTPUT);

  xTaskCreatePinnedToCore(taskEntry, "UltrasonicSensorTask", 10000, this, 1, &UltrasonicSensorTask, 1);
}

void UltrasonicSensor::taskEntry(void *param){
    UltrasonicSensor *self = static_cast<UltrasonicSensor *>(param);
    self->run();
}

void UltrasonicSensor::run(){
  for(;;) {
    if (xSemaphoreTake(systemStateMutex, portMAX_DELAY)) {
      digitalWrite(
        UltrasonicSensor::ULTRASONIC_SENSOR_TRIGGER_PIN,
        digitalRead(ULTRASONIC_SENSOR_TRIGGER_PIN) == HIGH ? LOW : HIGH
      );
      
      vTaskDelay(UltrasonicSensor::ULTRASONIC_SENSOR_TASK_DELAY / 2*  portTICK_PERIOD_MS);
    }
  }
}
