#include "WTFButtonSet.h"
#include <algorithm>

template <int numButtons, int section_x, int section_y>
void WTFButtonSet<numButtons, section_x, section_y>::updateButtonSet(int pins[][2], int sensitivity, int sectionOffsets[][2], AnimationType animations[], int delayed, int brightness, CRGB *leds ) {
    maestro.set_brightness(brightness);
    if (!cap.begin(0x5A)) {
      Serial.println("MPR121 not found, check wiring?");
      while (1);
    }
    Serial.println("MPR121 found!");
    for (int i = 0; i < numButtons; i++) {
        buttons[i].Setup(pins[i], sensitivity, sectionOffsets[i], animations[i], delayed, &sections[i], i, &maestro);
            buttons[i].calibrate(leds);
    }
}

template <int numButtons, int section_x, int section_y>
void WTFButtonSet<numButtons, section_x, section_y>::checkStates(unsigned long time) {
    mprStates = cap.touched();
    for ( WTFButton &button : buttons ) {
          if (button.senseController == 1) {
              button.checkState(time, &mprStates);
          } else {
              button.checkState(time);
          }
    }    
    std::sort(buttons, buttons+numButtons);
    
}

template <int numButtons, int section_x, int section_y>
void WTFButtonSet<numButtons, section_x, section_y>::updateLights(CRGB leds[]) {
    for ( WTFButton &button : buttons ) {
        button.updateLights(leds);
    }    
}


