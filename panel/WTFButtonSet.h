#ifndef WTFBUTTONSET_DEFINED
#define WTFBUTTONSET_DEFINED
#include "WTFButton.h"


template <int numButtons, int section_x, int section_y>
class WTFButtonSet {
    private:

    public:
    // The sections used by pixelMaestro.  Each one has a unique animation, and is tied to a specific button.
    Section sections[numButtons] = {
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
    };
    Maestro maestro = Maestro(sections, numButtons);
    WTFButton buttons[numButtons];
    WTFButtonSet() { }
    void updateButtonSet(int pins[], int sensitivity, int sectionOffsets[][2], AnimationType animations[], int delayed, int brightness, CRGB *leds);

    void calibrateButtons(CRGB leds[]);
    
    void checkStates(unsigned long time);
    
    void updateLights(CRGB leds[]);
};

#endif

