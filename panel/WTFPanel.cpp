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

#define NUM_LEDS 50
#define DATA_PIN 10

#define NUM_LEDS 50
#define NUM_BUTTONS 6
#define SECTION_X 4
#define SECTION_Y 3

// array of led colors used by FastLED.show() to drive LED strips
CRGB leds[NUM_LEDS];

// size of the section of lights activated by a button. Used in buttonset to create a section for each button.

// (x,y) position of top left of button in the overall layout, used to map button sections to physical layout
int sectionOffsets[NUM_BUTTONS][2] = { {8, 0}, {8, 3}, {4, 3}, { 0,3 },  {0,0}, { 4,0} };

/* 
 This represents the overall mapping of sections to LEDs.  Each entry is a position in leds[] for a physical LED.
 The grid is accessed via [y][x], using the position of the pixel in the pixelMaestro section combined with the 
 offsets for each button.  
 
 Specifically, 6 4x3 sections are laid out in a 3x2 grid, with one overlapping row of pixels on any shared edge.
 
*/
int physicalLayout[NUM_BUTTONS][12] = {
{49,48,47,46,46,45,44,43,43,42,41,40},
{30,31,32,33,33,34,35,36,36,37,38,39},
{29,28,27,26,26,25,24,23,23,22,21,20},
{29,28,27,26,26,25,24,23,23,22,21,20},
{10,11,12,13,13,14,15,16,16,17,18,19},
{9,8,7,6,6,5,4,3,3,2,1,0}};

// Pins on the microcontroller that the capacitive sensors are connected to
int touchPin[NUM_BUTTONS] = { 16, 17, 22, 19, 18, 15 };

// Amount of variance in capacity required to activate a button
int SENSITIVITY=50;

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
    Serial.println(time);
    //  Poll the current capacticance values and determines if buttons are activated
    buttonSet.checkStates(time);
    // maestro.update returns true if the animations require redrawing of pixels for the current time marker.
    if (buttonSet.maestro.update(time)) {
        buttonSet.updateLights(leds);
    }
    FastLED.show();
    Serial.println(millis()-time);
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


