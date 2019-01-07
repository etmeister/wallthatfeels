#ifndef WTFBUTTONSET_DEFINED
#define WTFBUTTONSET_DEFINED
#include "WTFButton.h"

#include "Adafruit_MPR121-Wire1.h"

extern Adafruit_MPR121 cap;

template <int numButtons, int section_x, int section_y>
class WTFButtonSet {
    private:
    uint16_t mprStates = 0;
    bool mprFound;


    public:
    // The sections used by pixelMaestro.  Each one has a unique animation, and is tied to a specific button.
    Section sections[numButtons] = {
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y),
      Section(section_x, section_y)
    };
    WTFButton buttons[numButtons];
    Maestro maestro;
    WTFButtonSet(): maestro(sections,numButtons) { }
    void updateButtonSet(int pins[][2], int sensitivity, int sectionOffsets[][2], AnimationType animations[], int delayed, int brightness);

    void calibrateButtons();
    
    void checkStates(unsigned long time);
    
    void updateLights(CRGB leds[]);
};

#endif

