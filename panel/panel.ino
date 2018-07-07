#include <PixelMaestro.h>
#include <core/maestro.h>
#include <colorpresets.h>
 
#include <deque>
#include <algorithm>
using namespace std;


#include <FastLED.h>
#include <Chrono.h>

#define NUM_LEDS 50
#define DATA_PIN 10

CRGB leds[NUM_LEDS];
int MAXBRIGHT = 100;
int SECTIONS_X = 4;
int SECTIONS_Y = 3;
int DELAY=200;
unsigned long time;

Section sections[6] = {
  Section(SECTIONS_X, SECTIONS_Y),
  Section(SECTIONS_X, SECTIONS_Y),
  Section(SECTIONS_X, SECTIONS_Y),
  Section(SECTIONS_X, SECTIONS_Y),
  Section(SECTIONS_X, SECTIONS_Y),
  Section(SECTIONS_X, SECTIONS_Y),
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

long touchThreshold[6] = { 1525, 1330, 1250, 1250, 1800, 1150 };
int touchPin[6] = { 16, 17, 22, 19, 18, 15 };

int offset = 0;
bool cursorDirection = 1;

Colors::RGB blackout[1] = { ColorPresets::Black };
Palette blackoutPalette =  Palette(blackout, 1);

float derp(float v0, float v1, float t) { return (1 - t) * v0 + t * v1; };

class WTFButton {
    private:
        int *screenOffset;
        int buttonActivated;
        Animation* animation;

    public:
        bool state;
        int maestroSection;
        long senseThreshold;
        long senseCurrent;
        int sensePin;
        unsigned long buttonPressed;
        Chrono buttonTimer;
        
        WTFButton(){
        }
		void Setup(int pin, long threshold, int offset[2], AnimationType buttonAnimation, int section) {
            sensePin = pin;
            maestroSection = section;
            senseThreshold = threshold;
            screenOffset = offset;
            animation = maestro.get_section(maestroSection)->set_animation(buttonAnimation);
            animation->set_palette(&blackoutPalette);
            animation->set_timer(DELAY);
		}
        bool operator<= (const WTFButton &other) const {
            return buttonPressed <= other.buttonPressed;
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
                Serial.print(sensePin);
                Serial.print(": ");
                Serial.print(senseThreshold);
                Serial.print(",");
                state = true;
                animation->set_palette(&ColorPresets::Colorwheel_Palette);
                buttonTimer.restart();
            } else if (buttonTimer.hasPassed(1000) ) {
                if (state) {
                    state = false;
                    buttonPressed = 0;
//                    animation->set_timer(500);
                    animation->set_palette(&blackoutPalette);
                }
            }

        }
        
};
template <int numButtons>
class buttonSet {
    private:

    public:
		WTFButton buttons[numButtons];
        //deque<WTFButton*> buttons;
        void updateButtonSet(int pins[], long thresholds[], int screenOffets[][2], AnimationType animations[]) {
              for (int i = 0; i < numButtons; i++) {
				  buttons[i].Setup(pins[i], thresholds[i], screenOffsets[i], animations[i], i);
                  //WTFButton b ( );
                  //buttons.push_back(new WTFButton(pins[i], thresholds[i], screenOffsets[i], animations[i], i ));
              }
        }
        
        void checkStates() {
            for ( WTFButton i : buttons ) {
                i.checkState();
            }    
            bubbleSortButtonsByPress();
        }
        
        unsigned int partition(unsigned int lo, unsigned int hi) {
        int mid = ( lo + hi ) / 2;
        if (buttons[mid] < buttons[lo]) {
            swap(buttons[lo],buttons[mid]);
        }
        if (buttons[hi] < buttons[lo]) {
            swap(buttons[lo], buttons[hi]);
        }
        if (buttons[mid] < buttons[hi]) {
            swap(buttons[mid],buttons[hi]);
        }

            unsigned int i = lo;
            for (unsigned int j = lo; j < hi; j++) {
                if (buttons[j] <= buttons[hi]) {
                       swap(buttons[i],buttons[j]);
                   i++;
                }
            }
            swap(buttons[i], buttons[hi]);
            return i;
        }
       
        void quickSortButtonsByPress(unsigned int lo, unsigned int hi) {
            Serial.println("Sorting buttons...");
            if (lo < hi) {
                unsigned int part = partition(lo, hi);
                quickSortButtonsByPress(lo, part-1);
                quickSortButtonsByPress(part+1, hi);
            }
            Serial.println("Done sorting buttons...");
        }           


    
        void bubbleSortButtonsByPress() {
            Serial.println("Sorting buttons...");
            unsigned int i;
            bool swapped = true;
            while (swapped != false) {
                swapped = false;
                for (i = 0; i < buttons.size()-1; i++) {
                    if (buttons[i+1] < buttons[i]) {
                        swap(buttons[i], buttons[i+1]);
                        swapped = true;
                    }
                }
            }
            Serial.println("Done sorting buttons...");
        }
};

buttonSet<6> buttonSets;

AnimationType animations[6] = { AnimationType::Wave, AnimationType::Fire, AnimationType::Plasma, AnimationType::Wave, AnimationType::Cycle, AnimationType::Blink };

void setup()
{   
    maestro.set_brightness(MAXBRIGHT);
    //maestro.set_timer(MAESTRODELAY);
    buttonSets.updateButtonSet( touchPin, touchThreshold, screenOffsets, animations);
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
        for ( WTFButton button : buttonSets.buttons ) {
            for (int x = 0; x < SECTIONS_X; x++) {
               for (int y = 0; y < SECTIONS_Y; y++) {
                   pixelColor = maestro.get_pixel_color(button->maestroSection, x, y);
                   int ypos = screenOffsets[button->maestroSection][1] + y;
                   int xpos = screenOffsets[button->maestroSection][0] + x;
                   leds[physicalLayout[ypos][xpos]] = CRGB(pixelColor.r, pixelColor.g, pixelColor.b);
/*                   if (button.state == false) {
                       leds[physicalLayout[ypos][xpos]].n( 0);
                   }*/
               }
            }
        }
    }
    Serial.println("");
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





