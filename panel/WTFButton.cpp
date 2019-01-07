#include "WTFButton.h"
#include <colorpresets.h>
#include <algorithm>

int derpout(float v0, float t) { return (int)max(0.0,(1 - t) * v0); };
CRGB BLACK (0,0,0);

void WTFButton::Setup(int pin[2], int sensitivity, int offset[2], AnimationType buttonAnimations[], int delayed, Section* section, int sectionId, Maestro* m) {
        sensePin = pin[0];
        senseController = pin[1];
        maestroSection = section;
        maestroSectionId = sectionId;
        maestro = m;
        senseItivity = sensitivity;
        sectionOffset = offset;
        animation = maestroSection->set_animation(buttonAnimations[0]);
        animation->set_palette(&ColorPresets::Colorwheel_Palette);
        animation->set_timer(delayed);
}

void WTFButton::calibrate(uint16_t *mprStates) {
    uint8_t threshold =  cap.readRegister8(MPR121_TOUCHTH_0 + 2*sensePin);
    Serial.println(threshold);

}

void WTFButton::calibrate() {
    long threshold = 0;
    for (int i = 0; i < 4000; i++) {
        threshold = max(threshold, touchRead(sensePin));
    }
    senseThreshold = threshold + senseItivity;
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
        Serial.print(",\"");
        Serial.print(senseController);
        Serial.print(":");
        Serial.print(sensePin);
        Serial.print("\"");
        if ( !state ) {
             // Track when the button was first pressed for sorting purposes
             buttonPressed = time;
             state = true;                     
        }
        // Reset the timer each time we think the button is actively pressed.
        buttonTimer.restart();
    } else if (buttonTimer.hasPassed(20) ) {
        // If we haven't reset the timer in at least a second, turn the button off
        if (state) {
            state = false;
            buttonPressed = 0;
            buttonTimer.restart();
        }

    }

}
        
void WTFButton::updateLights(CRGB leds[]) {
   Colors::RGB pixelColor; 
   float fadeTime = buttonTimer.elapsed() * .002;
   for (int x = 0; x < maestroSection->get_dimensions()->x; x++) {
       for (int y = 0; y < maestroSection->get_dimensions()->y; y++) {
           pixelColor = maestro->get_pixel_color(maestroSectionId, x, y);
           // Each button is part of an overall grid. sectionOffset updates the x,y values relative to that grid.
           int ypos = sectionOffset[1] + y;
           int xpos = sectionOffset[0] + x;
          // physicalLayout determines the actual LED index for a particular point in the overall grid.
           if (physicalLayout[ypos][xpos] < 150) {
                if ( state ) {
                    leds[physicalLayout[ypos][xpos]] = CRGB(pixelColor.r, pixelColor.g, pixelColor.b);
                } else if (buttonTimer.elapsed() < 500 ) {
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
}
        
