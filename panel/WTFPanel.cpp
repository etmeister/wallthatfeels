#include <PixelMaestro.h>
#include <core/maestro.h>
#include <colorpresets.h>
#include "WTFButton.h"
#include "WTFButtonSet.h"
#include "WTFButtonSet.cpp"
#include <algorithm>
using namespace std;


#include <FastLED.h>
#include <Chrono.h>

#define NUM_LEDS 150
#define DATA_PIN 14

#define NUM_BUTTONS 20
#define SECTION_X 3
#define SECTION_Y 3
#define TOTAL_X 12
#define TOTAL_Y 15

// array of led colors used by FastLED.show() to drive LED strips
CRGB leds[NUM_LEDS];


/* 
 *  
 * size of the section of lights activated by a button. Used in buttonset to create a section for each button.
 *
 * (x,y) position of top left of button in the overall layout, used to map button sections to physical layout
 * 
 */

//int sectionOffsets[NUM_BUTTONS][2] = { {8, 0}, {8, 3}, {4, 3}, { 0,3 },  {0,0}, { 4,0} };

int sectionOffsets[NUM_BUTTONS][2] = { 
{0,0}, {0,3}, {0,6}, {0,9}, {0, 12},
{3,0}, {3,3}, {3,6}, {3,9}, {3, 12},
{6,0}, {6,3}, {6,6}, {6,9}, {6, 12},
{9,0}, {9,3}, {9,6}, {9,9}, {9, 12}
};


/* 
 This represents the overall mapping of sections to LEDs.  Each entry is a position in leds[] for a physical LED.
 The grid is accessed via [y][x], using the position of the pixel in the pixelMaestro section combined with the 
 offsets for each button.  
 
 Specifically, 6 4x3 sections are laid out in a 3x2 grid, with one overlapping row of pixels on any shared edge.
 
*/
int physicalLayout[TOTAL_Y][TOTAL_X] = {
{0,1,2,3,4,5,5,6,7,8,9,150},
{150,19,18,17,16,15,15,14,13,12,11,10},
{20,21,22,23,24,25,25,26,27,28,29,30},
{40,39,38,37,36,35,35,34,33,32,31,150},
{150,41,42,43,44,45,45,46,47,48,49,150},
{60,59,58,57,56,55,55,54,53,52,51,50},
{150,61,62,63,64,65,65,66,67,68,69,70},
{80,79,78,77,76,75,75,74,73,72,71,150},
{150,81,82,83,84,85,85,86,87,88,89,90},
{100,99,98,97,96,95,95,94,93,92,91,150},
{150,101,102,103,104,105,105,106,107,108,109,110},
{120,119,118,117,116,115,115,114,113,112,111,150},
{122,121,118,125,126,127,127,128,129,130,131,132},
{150,123,124,139,138,137,137,136,135,134,133,150},
{141,140,142,143,144,145,145,146,147,148,149,150}};

// Pins on the microcontroller that the capacitive sensors are connected to
int touchPin[NUM_BUTTONS][2] = { {16,0}, {17,0}, {22,0}, {15,0}, {0,0} , {1,0} , {23,0}, {0,1}, {1,1}, {2,1}, {3,1}, {4,1}, {5,1}, {6,1}, {7,1}, {8,1}, {9,1}, {10,1}, {11,1}, {11, 1} };

// Amount of variance in capacity required to activate a button
int SENSITIVITY=200;

// List of animations to apply to sections, one-to-one mapping.
AnimationType animations[NUM_BUTTONS] = { AnimationType::Plasma, AnimationType::Plasma, AnimationType::Plasma, AnimationType::Plasma, AnimationType::Plasma, AnimationType::Plasma };

// variables controlling animation 
const int MAXBRIGHT = 100;
int DELAY=200;

// Create the set of buttons and the maestro animator object
WTFButtonSet<NUM_BUTTONS,SECTION_X,SECTION_Y> buttonSet;

void setup()
{   
    // Configure global brightness, map the buttons, enable the LEDs, and calibrate the buttons.
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    pinMode(DATA_PIN, OUTPUT);
    buttonSet.updateButtonSet( touchPin, SENSITIVITY, sectionOffsets , animations, DELAY, MAXBRIGHT, leds);
    Serial.begin(38400);
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


