#include <PixelMaestro.h>
#include <core/maestro.h>
#include <colorpresets.h>
 
#include <vector>
using namespace std;

#include <FastLED.h>
#include <Chrono.h>

#define NUM_LEDS 50
#define DATA_PIN 10

CRGB leds[NUM_LEDS];
int MAXBRIGHT = 255;
int MINBRIGHT = 30;
int FADEBRIGHT;
int DELAYFACTOR = 5;

unsigned long time;
int delayed;

Section sections[6] = {
  Section(4, 3),
  Section(4, 3),
  Section(4, 3),
  Section(4, 3),
  Section(4, 3),
  Section(4, 3),
};

Maestro maestro(sections, 6);

int screenOffsets[6][2] = { {8, 0}, {8, 3}, {4, 3}, { 0,3 },  {0,0}, { 4,0} };

int physicalLayout[6][12] = {
{49,48,47,46,46,45,44,43,43,42,41,40},
{30,31,32,33,33,34,35,36,36,37,38,39},
{29,28,27,26,26,25,24,23,23,22,21,20},
{29,28,27,26,26,25,24,23,23,22,21,20},
{10,11,12,13,13,14,15,16,16,17,18,19},
{9,8,7,6,6,5,4,3,3,2,1,0}};

Colors::RGB blackout[1] = { ColorPresets::Black };
Palette blackoutPalette =  Palette(blackout, 1);

class WTFButton {
    private:
        bool state;
        int *screenOffset;
        int buttonActivated;
        Animation* animation;

    public:
        int maestroSection;
        long senseThreshold;
        long senseCurrent;
        int sensePin;
        unsigned long buttonPressed;
        Chrono buttonTimer;
        
        WTFButton(int pin, long threshold, int offset[2], int section) {
            sensePin = pin;
            maestroSection = section;
            senseThreshold = threshold;
            screenOffset = offset;
            animation = maestro.get_section(maestroSection)->set_animation(AnimationType::Wave);
            animation->set_palette(&blackoutPalette);
            animation->set_timer(200);
        }

        bool operator< (const WTFButton &other) const {
            return buttonPressed < other.buttonPressed;
        }
        
        void checkState() {
            senseCurrent = touchRead(sensePin);
            if (senseCurrent > senseThreshold ) {
                if (state == false) {
                     buttonPressed = time;
                }
                state = true;
                animation->set_palette(&ColorPresets::Colorwheel_Palette);
                buttonTimer.restart();
                Serial.println(sensePin);
            } else if (buttonTimer.hasPassed(1000) ) {
                if (state) {
                    state = false;
                    buttonPressed = 0;
                    animation->set_palette(&blackoutPalette);
                }
            }

        }
        
};

class buttonSet {
    private:

    public:
        vector<WTFButton> buttons;
        void updateButtonSet(int numberOfButtons, int pins[], long thresholds[], int screenOffets[6][2]) {
              for (int i = 0; i < numberOfButtons; i++) {
                  WTFButton b = WTFButton(pins[i], thresholds[i], screenOffsets[i], i );
                  buttons.push_back(b);
              }
        }

        WTFButton getButton(int i) {
            return buttons[i];
        }

        void checkStates() {
            SortButtonsByPress();
            for ( auto &i : buttons ) {
                i.checkState();
            }    
        }

        void SortButtonsByPress() {
            int i;
            bool swapped = true;
            while (swapped != false) {
                swapped = false;
                for (i = 0; i < buttons.size()-1; i++) {
                    if (buttons[i].buttonPressed > buttons[i+1].buttonPressed) {
                        iter_swap(&buttons[i], &buttons[i+1]);
                        swapped = true;
                    }
                }
            }
        }
};


struct pixel {
  int red;
  int green;
  int blue;
};

buttonSet buttonSets;

pixel virtualScreen[10][5];

long touchThreshold[6] = { 1525, 1330, 1250, 1250, 1800, 1150 };
int touchPin[6] = { 16, 17, 22, 19, 18, 15 };


int offset = 0;
bool cursorDirection = 1;



void setup()
{   
    maestro.set_brightness(150);
    buttonSets.updateButtonSet(6, touchPin, touchThreshold, screenOffsets);
    pinMode(DATA_PIN, OUTPUT);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    Serial.begin(38400);
}

void loop()
{
    time = millis();
    Colors::RGB pixelColor; 

    buttonSets.checkStates();
    if (maestro.update(time)) {
        Serial.println("updating");
        for ( auto &button : buttonSets.buttons ) {
            for (unsigned int x = 0; x < 4; x++) {
               for (unsigned int y = 0; y < 3; y++) {
                   pixelColor = maestro.get_pixel_color(button.maestroSection, x, y);
                   leds[physicalLayout[y+screenOffsets[button.maestroSection][1]][x+screenOffsets[button.maestroSection][0]]] = CRGB(pixelColor.r, pixelColor.g, pixelColor.b);
               }
            }
        }
    }
    
    FastLED.show();
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





