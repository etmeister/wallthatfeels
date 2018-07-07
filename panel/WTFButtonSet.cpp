#include "WTFButtonSet.h"
#include <algorithm>

template <int numButtons>
void WTFButtonSet<numButtons>::updateButtonSet(int pins[], int sensitivity, int sectionOffsets[][2], AnimationType animations[], int delayed, Section sections[], Maestro* m) {
      for (int i = 0; i < numButtons; i++) {
          buttons[i].Setup(pins[i], sensitivity, sectionOffsets[i], animations[i], delayed, &sections[i], i, m);
          //WTFButton b ( );
          //buttons.push_back(new WTFButton(pins[i], thresholds[i], sectionOffsets[i], animations[i], i ));
      }
}


template <int numButtons>
void WTFButtonSet<numButtons>::calibrateButtons(CRGB leds[]) {
    for ( WTFButton &button : buttons ) {
        button.calibrate(leds);
    }
}

template <int numButtons>
void WTFButtonSet<numButtons>::checkStates(unsigned long time) {
    for ( WTFButton &button : buttons ) {
        button.checkState(time);
    }    
    std::sort(buttons, buttons+numButtons);
    
}

template <int numButtons>
void WTFButtonSet<numButtons>::updateLights(CRGB leds[]) {
    for ( WTFButton &button : buttons ) {
        button.updateLights(leds);
    }    
}


