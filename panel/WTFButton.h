#ifndef WTFBUTTON_DEFINED
#define WTFBUTTON_DEFINED
#include <PixelMaestro.h>
#include <core/maestro.h>
#include <Chrono.h>
#include <FastLED.h>

extern int physicalLayout[6][12];

class WTFButton {
    private:
        int *sectionOffset;
        int buttonActivated;
        Animation* animation;

    public:
        Maestro* maestro;
        bool state;
        Section* maestroSection;
        int maestroSectionId;
        long senseThreshold;
        long senseCurrent;
        int sensePin;
        int senseItivity;
        unsigned long buttonPressed;
        Chrono buttonTimer;
        
        WTFButton(){
        }

        void Setup(int pin, int sensitivity, int offset[2], AnimationType buttonAnimation, int delayed, Section* section, int sectionId, Maestro* m);

        bool operator< (const WTFButton &other) const {
          return buttonPressed <= other.buttonPressed;
        }

        void calibrate(CRGB *leds);
        void checkState(unsigned long time);
        
        void drawColor(CRGB *leds, int r, int g, int b);
        void blackLights(CRGB *leds);
        void blueLights(CRGB *leds);                
        void updateLights(CRGB *leds);
        
};

#endif
