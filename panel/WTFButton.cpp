#include "WTFButton.h"
#include <colorpresets.h>
#include <algorithm>

int derpout(float v0, float t) { return (int)max(0.0,(1 - t) * v0); };
CRGB BLACK (0,0,0);

void WTFButton::Setup(int pin[2], int sensitivity, int offset[2], AnimationType buttonAnimation, int delayed, Section* section, int sectionId, Maestro* m) {
        sensePin = pin[0];
        senseController = pin[1];
        maestroSection = section;
        maestroSectionId = sectionId;
        maestro = m;
        senseItivity = sensitivity;
        sectionOffset = offset;
        animation = maestroSection->set_animation(buttonAnimation);
        animation->set_palette(&ColorPresets::Colorwheel_Palette);
        animation->set_timer(delayed);
}

void WTFButton::calibrate(CRGB *leds) {
    if (senseController == 1) {
        calibrateMPR121(leds);
    } else {
        calibrateTeensy(leds);
    }
}

void WTFButton::calibrateMPR121(CRGB *leds) {
    blueLights(leds);
    FastLED.delay(100);  
    blackLights(leds);
}

void WTFButton::calibrateTeensy(CRGB *leds) {
    blueLights(leds);
    long threshold = 0;
    for (int i = 0; i < 10000; i++) {
        threshold = max(threshold, touchRead(sensePin));
    }
    senseThreshold = threshold + senseItivity;
    Serial.print(sensePin);
    Serial.print(" done calibrating: ");
    Serial.println(senseThreshold);
    blackLights(leds);
}

void WTFButton::checkState(unsigned long time, uint16_t *mprStates) {
    if ( (*mprStates & _BV(sensePin) ) ) {
       processState(time, true);
    } else {
       processState(time, false);
    }
}


void WTFButton::checkState(unsigned long time) {
    senseCurrent = touchRead(sensePin);
    if (senseCurrent > senseThreshold ) {
       processState(time, true);
    } else {
       processState(time, false);
    }
}

void WTFButton::processState(unsigned long time, bool newState) {
    if ( newState ) {
        if ( !state ) {
             // Track when the button was first pressed for sorting purposes
             buttonPressed = time;
             state = true;                     
        }
        // Reset the timer each time we think the button is actively pressed.
        buttonTimer.restart();
    } else if (buttonTimer.hasPassed(1000) ) {
        // If we haven't reset the timer in at least a second, turn the button off
        if (state) {
            state = false;
            buttonPressed = 0;
            buttonTimer.restart();
        }

    }

}

void WTFButton::drawColor(CRGB *leds, int r, int g, int b) {
   for (int x = 0; x < maestroSection->get_dimensions()->x; x++) {
       for (int y = 0; y < maestroSection->get_dimensions()->y; y++) {
           // Each button is part of an overall grid. sectionOffset updates the x,y values relative to that grid.
           int ypos = sectionOffset[1] + y;
           int xpos = sectionOffset[0] + x;
          // physicalLayout determines the actual LED index for a particular point in the overall grid.
           leds[physicalLayout[ypos][xpos]] = CRGB(r, g, b);
       }
    }
}

void WTFButton::blackLights(CRGB *leds) {
    drawColor(leds, 0,0,0);
    FastLED.show();          
}
void WTFButton::blueLights(CRGB *leds) {
    drawColor(leds,0,0,75);
    FastLED.show();          
}
        
void WTFButton::updateLights(CRGB *leds) {
   Colors::RGB pixelColor; 
   float fadeTime = buttonTimer.elapsed() * .001;
   for (int x = 0; x < maestroSection->get_dimensions()->x; x++) {
       for (int y = 0; y < maestroSection->get_dimensions()->y; y++) {
           pixelColor = maestro->get_pixel_color(maestroSectionId, x, y);
           // Each button is part of an overall grid. sectionOffset updates the x,y values relative to that grid.
           int ypos = sectionOffset[1] + y;
           int xpos = sectionOffset[0] + x;
          // physicalLayout determines the actual LED index for a particular point in the overall grid.
           if ( state ) {
               leds[physicalLayout[ypos][xpos]] = CRGB(pixelColor.r, pixelColor.g, pixelColor.b);
           } else if (buttonTimer.elapsed() < 1000 ) {
               // derpout is a linear interpolation to 0, used to fade the pixels to black after a button is deactivated.
               // fadeTime is a 0->1 value representing progress 
               leds[physicalLayout[ypos][xpos]] = CRGB(
                           derpout(pixelColor.r, fadeTime), 
                           derpout(pixelColor.g, fadeTime), 
                           derpout(pixelColor.b, fadeTime)
                       ); 
           } else {              
               leds[physicalLayout[ypos][xpos]] = BLACK;
           }
       }
    }          
}
        
