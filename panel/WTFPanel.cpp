#include <PixelMaestro.h>
#include <core/maestro.h>
#include <colorpresets.h>
#include "WTFButton.h"
#include "WTFPanel.h"
#include "WTFButtonSet.h"
#include "WTFButtonSet.cpp"
#include <algorithm>
using namespace std;


#include <i2c_t3.h>
#include <FastLED.h>
#include <Chrono.h>
#include "panelsetup.h"
#include "palettes.h"


// array of led colors used by FastLED.show() to drive LED strips
CRGB leds[NUM_LEDS];

Adafruit_MPR121 cap = Adafruit_MPR121();



//int sectionOffsets[NUM_BUTTONS][2] = { {8, 0}, {8, 3}, {4, 3}, { 0,3 },  {0,0}, { 4,0} };



// Pins on the microcontroller that the capacitive sensors are connected to




// Amount of variance in capacity required to activate a button
int SENSITIVITY=40;

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
    buttonSet.updateButtonSet( touchPin, SENSITIVITY, sectionOffsets , animations, DELAY, MAXBRIGHT, leds);
    colorCheck();
      Serial.println(ID);
    Serial.println("Ending setup");

}


void loop()
{
    unsigned long time = millis();
    //  Poll the current capacticance values and determines if buttons are activated
    buttonSet.checkStates(time);
    time = millis();
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


