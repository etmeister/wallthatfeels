#ifndef WTFBUTTON_DEFINED
#define WTFBUTTON_DEFINED
#include <PixelMaestro.h>
#include <core/maestro.h>
#include <Chrono.h>
#include <FastLED.h>
#include "WTFPanel.h"

extern Adafruit_MPR121 cap;

extern int physicalLayout[TOTAL_Y][TOTAL_X];

class WTFButton {
    private:
        int *sectionOffset;
        int buttonActivated;

    public:
        Animation* animation;
        Maestro* maestro;
        bool state;
        Section* maestroSection;
        int maestroSectionId;
        long senseThreshold;
        long senseCurrent;
        int sensePin;
        int currentPalette;
        int senseController;

        int senseItivity;
        unsigned long buttonPressed;
        Chrono buttonTimer;
        
        WTFButton(){
        }

        void Setup(int pin[2], int sensitivity, int offset[2], AnimationType buttonAnimations[], int delayed, Section* section, int sectionId, Maestro* m);

        bool operator< (const WTFButton &other) const {
          return buttonPressed < other.buttonPressed;
        }

        void calibrate();
        void calibrate(uint16_t *mprStates);
 
        bool checkState(unsigned long time);
        bool checkState(unsigned long time, uint16_t *mprStates);
        bool processState(unsigned long time, bool newState);
                 
        void forceLights(CRGB leds[], CRGB color);
        void updateLights(CRGB leds[]);
        
};

#endif
