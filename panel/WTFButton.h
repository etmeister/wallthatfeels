#ifndef WTFBUTTON_DEFINED
#define WTFBUTTON_DEFINED
#include <PixelMaestro.h>
#include <core/maestro.h>
#include <Chrono.h>
#include <FastLED.h>

#include "Adafruit_MPR121.h"

extern Adafruit_MPR121 cap;

extern int physicalLayout[16][12];

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
        int senseController;

        int senseItivity;
        unsigned long buttonPressed;
        Chrono buttonTimer;
        
        WTFButton(){
        }

        void Setup(int pin[2], int sensitivity, int offset[2], AnimationType buttonAnimation, int delayed, Section* section, int sectionId, Maestro* m);

        bool operator< (const WTFButton &other) const {
          return buttonPressed < other.buttonPressed;
        }

        void calibrate();
        void calibrate(uint16_t *mprStates);

        void checkState(unsigned long time);
        void checkState(unsigned long time, uint16_t *mprStates);
        void processState(unsigned long time, bool newState);
                 
        void updateLights(CRGB leds[]);
        
};

#endif
