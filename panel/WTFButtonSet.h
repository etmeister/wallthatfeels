#ifndef WTFBUTTONSET_DEFINED
#define WTFBUTTONSET_DEFINED
#include "WTFButton.h"


template <int numButtons>
class WTFButtonSet {
    private:

    public:
    WTFButton buttons[numButtons];

    void updateButtonSet(int pins[], int sensitivity, int screenOffsets[][2], AnimationType animations[], int delayed, Section sections[], Maestro* m);

    void calibrateButtons(CRGB leds[]);
    
    void checkStates(unsigned long time);
    
    void updateLights(CRGB leds[]);
};

#endif

