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

int physicalLayout[5][10] = {
{49,48,47,46,45,44,43,42,41,40},
{30,31,32,33,34,35,36,37,38,39},
{29,28,27,26,25,24,23,22,21,20},
{10,11,12,13,14,15,16,17,18,19},
{9,8,7,6,5,4,3,2,1,0}};

class Screen
{
public:
  //Pixel[10][5] ThePixels;
  void Update(CRGB *leds, double zoom, double moveX, double moveY)
  {
      //each iteration, it calculates: new = old*old + c, where c is a constant and old starts at current pixel
  double cRe, cIm;           //real and imaginary part of the constant c, determinate shape of the Julia Set
  double newRe, newIm, oldRe, oldIm;   //real and imaginary parts of new and old
  CRGB color; //the RGB color value for the pixel
  int maxIterations = 255; //after how much iterations the function should stop
  int h =5;
  int w =10;
  int pixelIter = 0;
    for(int y = 0; y < h; y++)
      for(int x = 0; x < w; x++)
      {
      //calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
      newRe = 1.5 * (x - w / 2) / (0.5 * zoom * w) + moveX;
      newIm = (y - h / 2) / (0.5 * zoom * h) + moveY;
      //i will represent the number of iterations
      int i;
      //start the iteration process
      for(i = 0; i < maxIterations; i++)
      {
        //remember value of previous iteration
        oldRe = newRe;
        oldIm = newIm;
        //the actual iteration, the real and imaginary part are calculated
        newRe = oldRe * oldRe - oldIm * oldIm + cRe;
        newIm = 2 * oldRe * oldIm + cIm;
        //if the point is outside the circle with radius 2: stop

        if((newRe * newRe + newIm * newIm) > 4) break;
      }
      Serial.println(i);
      //use color model conversion to get rainbow palette, make brightness black if maxIterations reached
      leds[physicalLayout[y][x]].setHSV(i, 255, 255 * (i < maxIterations));
      }
  }
};

class WTFButton {
    private:
        vector<int> buttonColors;
        vector<int> buttonLights;
        bool state;
        int *sensePosition;

    public:
        long senseThreshold;
        long senseCurrent;
        int sensePin;
        Chrono timer;
        
        WTFButton(int pin, long threshold, int position[2],  int colors[], int numberColors) {
            sensePin = pin;
            senseThreshold = threshold;
            sensePosition = position;
            setCHSVColors(colors, numberColors);
        }


        void checkState() {
            senseCurrent = touchRead(sensePin);
            if (senseCurrent > senseThreshold ) {
                Serial.print(",");
                Serial.print(sensePin);
                Serial.print(",[");
                Serial.println(sensePosition[0]);
                Serial.print(",");
                Serial.print(sensePosition[1]);
                Serial.print("]");
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
              
              for (int x = sensePosition[0]-2; x <= sensePosition[0]+2; x++) {
                  for (int y = sensePosition[1]-1; y <= sensePosition[1]+1; y++) {
                  if ( x < 0 || y < 0 || x > 9 || y > 4) { continue; }
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
                  color = (color) % 6;
                  leds[physicalLayout[y][x]] = CHSV(buttonColors[color], 255, BRIGHT);
              }
              }
          }
        }
};

class buttonSet {
    private:

    public:
        vector<WTFButton> buttons;
        buttonSet(int numberOfButtons, int pins[], long thresholds[], int buttonPositions[6][2], int buttonColors[]) {
              for (int i = 0; i < numberOfButtons; i++) {
                  WTFButton b = WTFButton(pins[i], thresholds[i], buttonPositions[i], buttonColors, 6 );
                  buttons.push_back(b);
              }
        }

        WTFButton getButton(int i) {
            return buttons[i];
        }
};

int buttonPositions[6][2] = { {7, 1}, {7, 3}, {5, 3}, { 1,3 },  {2,1}, { 4,1} };


struct pixel {
  int red;
  int green;
  int blue;
};

pixel virtualScreen[10][5];

long touchThreshold[6] = { 1525, 1330, 1250, 1250, 1800, 1150 };
int touchPin[6] = { 16, 17, 22, 19, 18, 15 };

int buttonColors[][6] = { { HUE_RED, HUE_ORANGE, HUE_YELLOW, HUE_GREEN, HUE_BLUE, HUE_PURPLE }, { HUE_AQUA, HUE_BLUE, HUE_PURPLE, HUE_AQUA, HUE_BLUE, HUE_PURPLE }, { HUE_RED, HUE_ORANGE, HUE_YELLOW, HUE_RED, HUE_ORANGE, HUE_YELLOW } };

buttonSet buttonSets(6, touchPin, touchThreshold, buttonPositions, buttonColors[0]);

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

Screen potzer; 

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

    /*for (int i = 0; i < NUM_LEDS; i++) {
         leds[i] = CHSV(0,255,0);
    }

    
    for (int i = 0; i < 6; i++) {
        char brightMode[] = "DELAYED";
        buttonSets.buttons[i].checkState();
        //strcpy(brightMode, (i % 2 > 0) ? "SLOFADE" : "TWINKLE" );
        buttonSets.buttons[i].setCHSVColors(buttonColors[((delayed / ((MAXBRIGHT-MINBRIGHT+1)*4) ) + i) % 3], 6);
        buttonSets.buttons[i].updateLeds(leds, brightMode);
    }*/
        FastLED.show();
        FastLED.delay(1000);
        for(int x = 0; x < 10; x++) {
          for (int y = 0; y < 5; y++) {
             potzer.Update(leds, 1, x, y);        
            FastLED.show();
            FastLED.delay(1000);            
          }
        }
        for (int z = 0; z < 100; z++) {
             potzer.Update(leds, z, 5, 3);        
            FastLED.show();
            FastLED.delay(1000);                      
        }
        FastLED.show();
        FastLED.delay(1000);



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





