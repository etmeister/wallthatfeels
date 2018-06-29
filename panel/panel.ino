#include <FastLED.h>
#include <Chrono.h> 
#include <vector>
using namespace std;

#define NUM_LEDS 50
#define DATA_PIN 10

CRGB leds[NUM_LEDS];
int MAXBRIGHT = 255;
int MINBRIGHT = 30;
int FADEBRIGHT;
int DELAYFACTOR = 5;

unsigned long time;
int delayed;

class WTFButton {
    private:
        vector<int> buttonColors;
        vector<int> buttonLights;
        bool state;
        int position = 0;

    public:
        long senseThreshold;
        long senseCurrent;
        int sensePin;
        Chrono timer;
        
        WTFButton(int pin, long threshold, int lights[], int numberLights, int colors[], int numberColors) {
            sensePin = pin;
            senseThreshold = threshold;
            setCHSVColors(colors, numberColors);
            for (int i = 0; i < numberLights; i++) {
                if (lights[i] > 0) {
                    buttonLights.push_back(lights[i]-1);
                }
            }
        }


        void checkState() {
            senseCurrent = touchRead(sensePin);
            if (senseCurrent > senseThreshold ) {
                Serial.print(",");
                Serial.print(sensePin);
                state = true;
                timer.restart();
            } else if (timer.hasPassed(1000) ) {
                state = false;
            } else {
                Serial.print(",");
                Serial.print(sensePin);

            }

        }
        
        void setCHSVColors(int colors[], int numberColors) {
            buttonColors.clear();
            for (int i = 0; i < numberColors; i++) {
                buttonColors.push_back(colors[i]);
            }
        }

        void updateLeds(CRGB *leds, char *brightMode) {
          if (state) {
              for (unsigned int i = 0; i < buttonLights.size(); i++) {
                  int BRIGHT;
                  if (strcmp(brightMode, "SLOFADE") == 0) {
                      BRIGHT = FADEBRIGHT;
                  } else if (strcmp(brightMode, "TWINKLE") == 0) {
                      BRIGHT = scale8(random8(), MAXBRIGHT);
                  } else if (strcmp(brightMode, "DELAYED") == 0) {
                      BRIGHT = map(timer.elapsed(),0,1000,255,0);
                  } else {
                      BRIGHT=MAXBRIGHT;
                  }
                  int color = (delayed / 51);
                  color = (color + i) % 6;
                  leds[buttonLights[i]] = CHSV(buttonColors[color], 255, BRIGHT);
              }
          }
        }
};

class buttonSet {
    private:

    public:
        vector<WTFButton> buttons;
        buttonSet(int numberOfButtons, int pins[], long thresholds[], int buttonLights[][13], int buttonColors[]) {
              for (int i = 0; i < numberOfButtons; i++) {
                  WTFButton b = WTFButton(pins[i], thresholds[i], buttonLights[i], 13, buttonColors, 6 );
                  buttons.push_back(b);
              }
        }

        WTFButton getButton(int i) {
            return buttons[i];
        }
};


long touchThreshold[6] = { 1525, 1330, 1250, 1250, 1800, 1150 };
int touchPin[6] = { 16, 17, 22, 19, 18, 15 };
int buttonLights[6][13] = {
  { 44, 43, 42, 41, 40, 39, 38, 37, 23, 22, 21 },
  { 20, 19, 18, 6, 5, 4, 3, 2, 1 },
  { 25, 17, 16, 15, 8, 7, 6, 5 },
  { 15, 14, 13, 12, 11, 10, 9, 8 },
  { 50, 49, 48, 47, 34, 33, 32, 31, 30, 29, 28, 27, 26},
  { 47, 46, 45, 37, 36, 35,34, 26, 24 }
};
int buttonColors[][6] = { { HUE_RED, HUE_ORANGE, HUE_YELLOW, HUE_GREEN, HUE_BLUE, HUE_PURPLE }, { HUE_AQUA, HUE_BLUE, HUE_PURPLE, HUE_AQUA, HUE_BLUE, HUE_PURPLE }, { HUE_RED, HUE_ORANGE, HUE_YELLOW, HUE_RED, HUE_ORANGE, HUE_YELLOW } };

buttonSet buttonSets(6, touchPin, touchThreshold, buttonLights, buttonColors[0]);

int offset = 0;
bool cursorDirection = 1;







void setFadeBright() {
    FADEBRIGHT= delayed % MAXBRIGHT+1;
    cursorDirection = delayed / MAXBRIGHT+1 % 2;
    if (cursorDirection) {
        FADEBRIGHT = map(FADEBRIGHT,MAXBRIGHT,0,0,MAXBRIGHT);
    }
    if (FADEBRIGHT < MINBRIGHT) FADEBRIGHT = MINBRIGHT;
}


void setup()
{
    pinMode(DATA_PIN, OUTPUT);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    Serial.begin(38400);
}

void loop()
{
    time = millis();
    delayed  = (int) time / DELAYFACTOR;
    setFadeBright();
    Serial.print("[0");

    for (int i = 0; i < NUM_LEDS; i++) {
         leds[i] = CHSV(0,255,0);
    }

    
    for (int i = 0; i < 6; i++) {
        char brightMode[] = "DELAYED";
        buttonSets.buttons[i].checkState();
        //strcpy(brightMode, (i % 2 > 0) ? "SLOFADE" : "TWINKLE" );
        buttonSets.buttons[i].setCHSVColors(buttonColors[((delayed / ((MAXBRIGHT-MINBRIGHT+1)*4) ) + i) % 3], 6);
        buttonSets.buttons[i].updateLeds(leds, brightMode);
    }

    FastLED.show();

    Serial.println("]");
    Serial.flush();
    
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





