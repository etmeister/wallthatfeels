#ifndef WTFBUTTONSET_DEFINED
#define WTFBUTTONSET_DEFINED
#include "WTFButton.h"

#include "WTFPanel.h"

extern Adafruit_MPR121 cap;
extern Palette palettes[15];
extern CFastLED FastLED;
struct buttonResults {
    unsigned long latest;
    int color;
};

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
    void updateButtonSet(int pins[][2], int sensitivity, int sectionOffsets[][2], AnimationType animations[], int delayed, int brightness, CRGB leds[]);

    void calibrateButtons(CRGB leds[]);
    
    void checkStates(unsigned long time);
    int checkNeighbors(WTFButton button);
    void getColor(int buttonY, int buttonX, buttonResults* choice);
    void updateLights(CRGB leds[]);
};

#endif

