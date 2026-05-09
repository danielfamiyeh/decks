#include <LiquidCrystal_I2C.h>

const int STcp = 27;
const int SHcp = 26;
const int DS = 25;

const int potPins[] = {34, 35, 32};

const int VRx = 34;

TaskHandle_t ShiftTask;
TaskHandle_t ScreenTask;
TaskHandle_t JoystickTask;

LiquidCrystal_I2C lcd(0x27,16,4);

int levelState = 0;
auto levelText = {"High", "Mid", "Low"};

int yVal = 0;

void setup() {
  Serial.begin(9600);

  // LED shift register
  pinMode(STcp, OUTPUT);
  pinMode(SHcp, OUTPUT);
  pinMode(DS, OUTPUT);

  // Screen
  lcd.init();

  lcd.backlight();

  // xTaskCreatePinnedToCore(shiftRegisterLoop, "ShiftTask", 10000, NULL, 1, &ShiftTask, 0);
  xTaskCreatePinnedToCore(screenLoop, "ScreenTask", 10000, NULL, 1, &ScreenTask, 1);
  xTaskCreatePinnedToCore(joystickLoop, "JoystickTask", 10000, NULL, 1, &JoystickTask, 0);
}

void shiftRegisterLoop(void * pvParameters) {
    int led = 1; 
    for(;;){
      for (int i=0; i<7; i++){
        digitalWrite(STcp, LOW);
        shiftOut(DS, SHcp, MSBFIRST, led);
        digitalWrite(STcp, HIGH);
        led <<= 1;
        led %= B00100000;
      }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }

void screenLoop(void *pvParameters) {
  int lastDisplayedState = -1;
  for(;;) {
    if (lastDisplayedState != levelState) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(*(levelText.begin() + levelState));

      lastDisplayedState = levelState;
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void joystickLoop(void *pvParameters) {
  bool moved = false;

  for(;;) {
    yVal = analogRead(VRx);
    Serial.write(yVal);
    if (yVal < 1000 && !moved) {
      levelState = (levelState == 0) ? 2 : levelState - 1;
      moved = true;
    } else if (yVal > 3000 && !moved) {
      levelState = (levelState + 1) % 3;
      moved = true;
    } else if (yVal > 1500 && yVal < 2500) {
      moved = false;
    }
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}

void loop() {
  
}
