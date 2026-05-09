#include <map>
#include <algorithm>
#include <LiquidCrystal_I2C.h>

TaskHandle_t ShiftTask;
TaskHandle_t ScreenTask;
TaskHandle_t JoystickTask;
TaskHandle_t MixingTask;

LiquidCrystal_I2C lcd(0x27,16,4);

enum MixerLevel { LOWS, MIDS, HIGHS };
enum ScreenRow { TOP, BOTTOM };

class Pot {
  public:
    int pin;
    int last;

    Pot(int inputPin, int threshold) {
      pin = inputPin;
      last = 0;
    }

    int read(){
      int total = 0;

      for (int i = 0; i < 10; i++) {
        total += analogRead(pin);
        delayMicroseconds(200);
      }

      last = total / 10;
      return last;
    }
};

class Mixer {
  public:
    static const int LEFT = 35;
    static const int  RIGHT = 33;
    static const int  POT_COUNT = 2;

    MixerLevel level = MixerLevel::LOWS;
    Pot pots[Mixer::POT_COUNT] = {
      Pot(Mixer::LEFT, 100),
      Pot(Mixer::RIGHT, 200)
    };

    void init(){
      xTaskCreatePinnedToCore(taskEntry, "MixingTask", 10000, this, 1, &MixingTask, 0);
    };

    void checkPots(){
      
    };

    static long potPercent(int val) {
      return map(val, 0, 4095, 0, 100);
    };

  private:
    void run() {
      for(;;) {
        int lPotLast = pots[0].last;
        int rPotLast = pots[1].last;

        int lPotNow = pots[0].read();
        int rPotNow = pots[1].read();
        int maxDelta = std::max(abs(lPotNow - lPotLast), abs(rPotNow - rPotLast));

        if(maxDelta > 100) {
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);

          lcd.print("L:");
          lcd.print(Mixer::potPercent(lPotNow));
          lcd.print("%");
          lcd.print("   ");

          lcd.print("R:");
          lcd.print(Mixer::potPercent(rPotNow));
          lcd.print("%");
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
      }
    }

    static void taskEntry(void *param) {
      Mixer *self = static_cast<Mixer *>(param);
      self->run();
    };
};

class Screen {
  public:
    std::map<MixerLevel, String> LevelText = {
      {MixerLevel::LOWS, "Lows"},
      {MixerLevel::MIDS, "Mids"},
      {MixerLevel::HIGHS, "Highs"}
    };

    void run(){
      for(;;) {
       
      }
    };

    void init(){
      lcd.init();
      lcd.backlight();

      xTaskCreatePinnedToCore(taskEntry, "ScreenTask", 10000, this, 1, &ScreenTask, 1);
    };

    static void taskEntry(void *param) {
      Screen *self = static_cast<Screen *>(param);
      self->run();
    };
};

class DJDeck {
  private:
    Mixer mixer;
    Screen screen;

  public:
    void init(){
      Serial.begin(115200);

      screen.init();
      mixer.init();
    };
};

DJDeck decks;
void setup() {
  decks.init(); 
};

void loop() {
  
}
