#include <PixelMaestro.h>
#include <core/maestro.h>
#include <colorpresets.h>
#ifndef Included_NameModel_H
#include "WTFButton.h"
#define Included_NameModel_H
#endif
#include "WTFButtonSet.h"
#include "WTFButtonSet.cpp"
#include <algorithm>
using namespace std;


#include <FastLED.h>
#include <Chrono.h>

#define NUM_LEDS 50
#define DATA_PIN 10

CRGB leds[NUM_LEDS];
const int MAXBRIGHT = 100;
int SECTIONS_X = 4;
int SECTIONS_Y = 3;
int SENSITIVITY=50;
int DELAY=200;

Section sections[6] = {
  Section(SECTIONS_X, SECTIONS_Y),
  Section(SECTIONS_X, SECTIONS_Y),
  Section(SECTIONS_X, SECTIONS_Y),
  Section(SECTIONS_X, SECTIONS_Y),
  Section(SECTIONS_X, SECTIONS_Y),
  Section(SECTIONS_X, SECTIONS_Y),
};


int screenOffsets[6][2] = { {8, 0}, {8, 3}, {4, 3}, { 0,3 },  {0,0}, { 4,0} };

int physicalLayout[6][12] = {
{49,48,47,46,46,45,44,43,43,42,41,40},
{30,31,32,33,33,34,35,36,36,37,38,39},
{29,28,27,26,26,25,24,23,23,22,21,20},
{29,28,27,26,26,25,24,23,23,22,21,20},
{10,11,12,13,13,14,15,16,16,17,18,19},
{9,8,7,6,6,5,4,3,3,2,1,0}};

int touchPin[6] = { 16, 17, 22, 19, 18, 15 };

int offset = 0;
bool cursorDirection = 1;

Colors::RGB blackout[1] = { ColorPresets::Black };
Palette blackoutPalette =  Palette(blackout, 1);

AnimationType animations[6] = { AnimationType::Plasma, AnimationType::Plasma, AnimationType::Plasma, AnimationType::Plasma, AnimationType::Plasma, AnimationType::Plasma };

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





WTFButtonSet<6> buttonSets;
Maestro maestro(sections, 6);

void setup()
{   
    maestro.set_brightness(MAXBRIGHT);
    buttonSets.updateButtonSet( touchPin, SENSITIVITY, screenOffsets, animations, DELAY, sections, &maestro);
    pinMode(DATA_PIN, OUTPUT);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    Serial.begin(38400);
    buttonSets.calibrateButtons(leds);
}

void loop()
{
    unsigned long time = millis();
    buttonSets.checkStates(time);
    if (maestro.update(time)) {
        buttonSets.updateLights(leds);
    }
    FastLED.show();
    Serial.println(millis()-time);
}


