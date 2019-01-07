#include <PixelMaestro.h>
#include <core/maestro.h>
#include <colorpresets.h>
#include "WTFButton.h"
#include "WTFButtonSet.h"
#include "WTFButtonSet.cpp"
#include <algorithm>
using namespace std;


#include <i2c_t3.h>
#include "Adafruit_MPR121-Wire1.h"
#include <FastLED.h>
#include <Chrono.h>

#define NUM_LEDS 150
#define DATA_PIN 13

#define NUM_BUTTONS 24      
#define SECTION_X 4
#define SECTION_Y 4
#define TOTAL_X 12
#define TOTAL_Y 32  

// array of led colors used by FastLED.show() to drive LED strips
CRGB leds[NUM_LEDS];


Adafruit_MPR121 cap = Adafruit_MPR121();


/* 
 *  
 * size of the section of lights activated by a button. Used in buttonset to create a section for each button.
 *
 * (x,y) position of top left of button in the overall layout, used to map button sections to physical layout
 * 
 */

//int sectionOffsets[NUM_BUTTONS][2] = { {8, 0}, {8, 3}, {4, 3}, { 0,3 },  {0,0}, { 4,0} };

int sectionOffsets[NUM_BUTTONS][2] = { 
  {0,0}, {4,0}, {8,0},
  {0,4}, {4,4}, {8,4},
  {0,8}, {4,8}, {8,8},
  {0,12}, {4,12}, {8,12},
  {0,16}, {4,16}, {8,16},
  {0,20}, {4,20}, {8,20},
  {0,24}, {4,24}, {8,24},
  {0,28}, {4,28}, {8,28}
};

// Pins on the microcontroller that the capacitive sensors are connected to
int touchPin[NUM_BUTTONS][2] = { 
  {4,1},  {5,1}, {7,1},
  {2,1},  {6,1},  {8,1},
  {1,1}, {3,1} , {10, 1},
  {0,1}, {11,1}, {9,1},
  {32,0}, {15,0}, {16,0}, 
  {33,0}, {25,0}, {17,0},
  {1,0}, {0,0}, {18,0}, 
  {23,0}, {22,0}, {19,0}
};

/* 
 This represents the overall mapping of sections to LEDs.  Each entry is a position in leds[] for a physical LED.
 The grid is accessed via [y][x], using the position of the pixel in the pixelMaestro section combined with the 
 offsets for each button.  
 
 Specifically, 6 4x3 sections are laid out in a 3x2 grid, with one overlapping row of pixels on any shared edge.
 
*/

// SQUARE BOARD

int physicalLayout[TOTAL_Y][TOTAL_X] = {
  {150,150,150,150,64,150,66,150,150,150,114,150},
  {150,24,150,150,150,65,150,67,150,113,150,115},
  {23,150,25,150,63,150,68,150,150,112,116,150},
  {22,150,26,150,62,150,150,150,150,150,118,117},
  {150,21,150,150,150,69,150,150,111,150,150,150},
  {20,150,27,150,61,150,70,110,150,119,150,150},
  {150,19,28,150,60,71,150,150,109,150,120,121},
  {150,150,150,150,150,150,150,150,150,150,150,150},
  {18,150,29,59,150,150,72,108,150,150,150,122},
  {150,150,150,150,150,73,150,150,150,124,123,150},
  {150,17,30,150,58,150,74,107,150,125,126,150},
  {16,150,31,150,57,75,150,106,150,150,150,150},
  {150,150,150,150,150,150,150,150,105,150,150,127},
  {150,32,150,15,56,150,76,104,150,129,128,150}, 
  {150,14,150,33,55,77,150,150,103,130,131,150},
  {150,13,150,34,150,150,150,150,150,150,150,132},
  {12,150,150,150,54,150,78,102,150,133,150,150},
  {150,150,35,53,150,79,150,150,101,150,135,150},
  {150,11,36,150,52,80,150,100,150,134,136,150},
  {10,150,37,150,51,81,150,150,99,150,150,137},
  {150,9,150,150,150,150,150,98,150,150,138,150},
  {8,150,150,38,50,150,82,150,97,139,150,150},
  {150,7,150,39,150,49,83,150,96,150,140,141},
  {150,150,150,150,150,150,150,150,150,150,142,150},
  {6,150,150,40,48,150,84,150,95,150,150,150},
  {150,150,5,150,150,47,150,85,150,143,150,144},
  {150,4,150,41,150,46,86,150,94,150,145,150},
  {150,3,150,42,48,150,150,87,93,150,146,150},
  {2,150,150,150,150,45,88,150,150,92,150,147},
  {150,1,43,150,44,150,89,150,91,150,150,150},
  {0,150,150,150,150,150,150,90,150,149,148,150},
  {150,150,150,150,150,150,150,150,150,150,150,150},
};

  


// Amount of variance in capacity required to activate a button
int SENSITIVITY=20;

// List of animations to apply to sections, one-to-one mapping.
AnimationType animations[2] = { 
  AnimationType::Plasma, AnimationType::Wave
};
// variables controlling animation 
const int MAXBRIGHT = 255;
int DELAY=200;

// Create the set of buttons and the maestro animator object
WTFButtonSet<NUM_BUTTONS,SECTION_X,SECTION_Y> buttonSet;
int lerpout(float v0, float t) { return (int)max(0.0,(1 - t) * v0); };


void colorCheck() {
      for(int i = 0; i <= 100; i++) {
        float lerp = lerpout(100, i*.01);
        for (int led = 0; led < NUM_LEDS; led++) {
            leds[led] = CRGB(lerp,0,0);
        }
        FastLED.show();
        delay(1);
    }
      for(int i = 0; i <= 100; i++) {
        float lerp = lerpout(100, i*.01);
        for (int led = 0; led < NUM_LEDS; led++) {
            leds[led] = CRGB(0,lerp,0);
        }
        FastLED.show();
        delay(1);
    }
      for(int i = 0; i <= 100; i++) {
        float lerp = lerpout(100, i*.01);
        for (int led = 0; led < NUM_LEDS; led++) {
            leds[led] = CRGB(0,0,lerp);
        }
        FastLED.show();
        delay(1);
    }    
        
}

void setup()
{   
    Serial.begin(38400);
    
  char ID[32];
  sprintf(ID, "%08lX %08lX %08lX %08lX", SIM_UIDH, SIM_UIDMH, SIM_UIDML, SIM_UIDL);
    Serial.println("Beginning setup");
    // Configure global brightness, map the buttons, enable the LEDs, and calibrate the buttons.
    pinMode(DATA_PIN, OUTPUT);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    buttonSet.updateButtonSet( touchPin, SENSITIVITY, sectionOffsets , animations, DELAY, MAXBRIGHT);
    colorCheck();
      Serial.println(ID);

    Serial.println("Ending setup");

}


void loop()
{
    unsigned long time = millis();
    //  Poll the current capacticance values and determines if buttons are activated
    buttonSet.checkStates(time);
    // maestro.update returns true if the animations require redrawing of pixels for the current time marker.
    if (buttonSet.maestro.update(time)) {
        buttonSet.updateLights(leds);
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


