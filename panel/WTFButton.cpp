#include "WTFButton.h"

float derpout(float v0, float t) { return (1 - t) * v0; };
CRGB BLACK (0,0,0);

void WTFButton::Setup(int pin, int sensitivity, int offset[2], AnimationType buttonAnimation, int delayed, Section* section, int sectionId, Maestro* m) {
        sensePin = pin;
        maestroSection = section;
        maestroSectionId = sectionId;
        maestro = m;
        senseItivity = sensitivity;
        screenOffset = offset;
        animation = maestroSection->set_animation(buttonAnimation);
        animation->set_palette(&ColorPresets::Colorwheel_Palette);
        animation->set_timer(delayed);
}


void WTFButton::calibrate(CRGB *leds) {
    blueLights(leds);
    long threshold = 0;
    for (int i = 0; i < 5000; i++) {
        threshold = max(threshold, touchRead(sensePin));
    }
    senseThreshold = threshold + senseItivity;
    Serial.print(sensePin);
    Serial.print(" done calibrating: ");
    Serial.println(senseThreshold);
    blackLights(leds);
}

void WTFButton::checkState(unsigned long time) {
    senseCurrent = touchRead(sensePin);
    if (senseCurrent > senseThreshold ) {
        if (state == false) {
             buttonPressed = time;
             state = true;                     
        }

        buttonTimer.restart();
       // animation->set_palette(&ColorPresets::Colorwheel_Palette);
    } else if (buttonTimer.hasPassed(1000) ) {
        if (state) {
            state = false;
            buttonPressed = 0;
//                  animation->set_timer(500);
            //animation->set_palette(&blackoutPalette);
            buttonTimer.restart();
        }

    }

}
void WTFButton::blackLights(CRGB *leds) {
   for (int x = 0; x < maestroSection->get_dimensions()->x; x++) {
       for (int y = 0; y < maestroSection->get_dimensions()->y; y++) {
           int ypos = screenOffset[1] + y;
           int xpos = screenOffset[0] + x;
           leds[physicalLayout[ypos][xpos]] = CRGB(0,0,0);
       }
    }
    FastLED.show();          
}

void WTFButton::blueLights(CRGB *leds) {
   for (int x = 0; x < maestroSection->get_dimensions()->x; x++) {
       for (int y = 0; y < maestroSection->get_dimensions()->y; y++) {
           int ypos = screenOffset[1] + y;
           int xpos = screenOffset[0] + x;
           leds[physicalLayout[ypos][xpos]] = CRGB(0,0,255);
       }
    }
    FastLED.show();          
}
        
void WTFButton::updateLights(CRGB *leds) {
   Colors::RGB pixelColor; 
   for (int x = 0; x < maestroSection->get_dimensions()->x; x++) {
       for (int y = 0; y < maestroSection->get_dimensions()->y; y++) {
           pixelColor = maestro->get_pixel_color(maestroSectionId, x, y);
           int ypos = screenOffset[1] + y;
           int xpos = screenOffset[0] + x;
           if ( state ) {
               leds[physicalLayout[ypos][xpos]] = CRGB(pixelColor.r, pixelColor.g, pixelColor.b);
           } else if (buttonTimer.elapsed() < 1000 ) {

               float fadeTime = buttonTimer.elapsed() * .001;
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
        
