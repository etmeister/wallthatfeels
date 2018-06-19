#include <FastLED.h>
#include <Wire.h>
#include <vector>
using namespace std;

#define NUM_LEDS 50
#define DATA_PIN 10

CRGB leds[NUM_LEDS];
int MAXBRIGHT = 255;
int DELAY=50;
int BRIGHT;


class WTFButton {
    private:
        vector<int> buttonColors;
        vector<int> buttonLights;
        bool state;

    public:
        long senseThreshold;
        long senseCurrent;
        int sensePin;
        
        WTFButton(int pin, long threshold, int lights[], int colors[], int numberColors) {
            sensePin = pin;
            senseThreshold = threshold;
            setCHSVColors(colors, numberColors);
            for (int i = 0; i < 13; i++) {
                if (lights[i] > 0) {
                    buttonLights.push_back(lights[i]-1);
                }
            }
        }


        void checkState() {
            senseCurrent = touchRead(sensePin);
            if (senseCurrent > senseThreshold ) {
                state = true;
                Serial.print(",");
                Serial.print(sensePin);
            } else {
                state = false;
            }

        }
        
        void setCHSVColors(int colors[], int numberColors) {
            buttonColors.clear();
            for (int i = 0; i < numberColors; i++) {
                buttonColors.push_back(colors[i]);
            }
        }

        void updateLeds(CRGB *leds) {
          if (state) {
              for (unsigned int i = 0; i < buttonLights.size(); i++) {
                  leds[buttonLights[i]] = CHSV(buttonColors[0], 255, BRIGHT);
              }
          }
        }
};

class buttonSet {
    private:

    public:
        vector<WTFButton> buttons;
        buttonSet(int numberOfButtons, int pins[], long thresholds[], int buttonLights[][13], int buttonColors[][1]) {
              for (int i = 0; i < numberOfButtons; i++) {
                  WTFButton b = WTFButton(pins[i], thresholds[i], buttonLights[i], buttonColors[i], 1 );
                  buttons.push_back(b);
              }
        }

        WTFButton getButton(int i) {
            return buttons[i];
        }
};


long touchThreshold[6] = { 1250, 1350, 1200, 1200, 1200, 1200 };
int touchPin[6] = { 16, 17, 22, 19, 18, 15 };
int buttonLights[6][13] = {
  { 44, 43, 42, 41, 40, 39, 38, 37, 23, 22, 21 },
  { 20, 19, 18, 6, 5, 4, 3, 2, 1 },
  { 25, 17, 16, 15, 8, 7, 6, 5 },
  { 15, 14, 13, 12, 11, 10, 9, 8 },
  { 50, 49, 48, 47, 34, 33, 32, 31, 30, 29, 28, 27, 26},
  { 47, 46, 45, 37, 36, 35,34, 26, 24 }
};
int buttonColors[6][1] = { { HUE_RED }, { HUE_ORANGE }, { HUE_YELLOW }, { HUE_GREEN }, { HUE_BLUE }, {HUE_PURPLE} };
int buttonStates[6] = { 0, 0, 0, 0, 0, 0 };

int buttonState = 0;
int lastButtonState = 0;
int buttonCount = 0;
int buttonPin = 12;
int offset = 0;
bool cursorDirection = 1;






buttonSet buttonSets(6, touchPin, touchThreshold, buttonLights, buttonColors);

void setup()
{
    pinMode(DATA_PIN, OUTPUT);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    Serial.begin(9600);
    pinMode(buttonPin, INPUT_PULLUP);
}

int counter=400;
void loop()
{
    BRIGHT=counter/10;
    checkButton();
    Serial.print("[0");

    for (int i = 0; i < NUM_LEDS; i++) {
         leds[i] = CHSV(0,255,0);
    }

    
    for (int i = 0; i < 6; i++) {
        buttonSets.buttons[i].checkState();
        buttonSets.buttons[i].updateLeds(leds);
    }

    FastLED.show();

    Serial.println("]");
    
    if (cursorDirection) {
        if(counter < 2550) {
            counter++;
        } else {
            cursorDirection = 0;
        }
    } else {
       if(counter > 400) {
           counter--;
       } else {
           cursorDirection = 1;
       }
    }
}

bool checkButton() {
    bool returnVal = false;
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH && lastButtonState == LOW) {
      if (buttonCount < 4) {
          buttonCount++;
      } else {
          buttonCount = 0;
      }
      returnVal = true;
    }
    lastButtonState = buttonState;
    return returnVal;
}


namespace std {
  void __throw_bad_alloc()
  {
    Serial.println("Unable to allocate memory");
  }

  void __throw_length_error( char const*e )
  {
    Serial.print("Length Error :");
    Serial.println(e);
  }
}




