#include "WTFButtonSet.h"
#include <algorithm>

template <int numButtons, int section_x, int section_y>
void WTFButtonSet<numButtons, section_x, section_y>::updateButtonSet(int pins[], int sensitivity, int sectionOffsets[][2], AnimationType animations[], int delayed, int brightness, CRGB *leds ) {
      maestro.set_brightness(brightness);
//      maestro.set_sections(sections, numButtons);
      for (int i = 0; i < numButtons; i++) {
          buttons[i].Setup(pins[i], sensitivity, sectionOffsets[i], animations[i], delayed, &sections[i], i, &maestro);
          buttons[i].calibrate(leds);
      }
}

template <int numButtons, int section_x, int section_y>
void WTFButtonSet<numButtons, section_x, section_y>::checkStates(unsigned long time) {
    for ( WTFButton &button : buttons ) {
        button.checkState(time);
    }    
    std::sort(buttons, buttons+numButtons);
    
}

template <int numButtons, int section_x, int section_y>
void WTFButtonSet<numButtons, section_x, section_y>::updateLights(CRGB leds[]) {
    for ( WTFButton &button : buttons ) {
        button.updateLights(leds);
    }    
}


