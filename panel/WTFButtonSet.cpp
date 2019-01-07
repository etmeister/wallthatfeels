#include "WTFButtonSet.h"
#include <algorithm>

template <int numButtons, int section_x, int section_y>
void WTFButtonSet<numButtons, section_x, section_y>::updateButtonSet(int pins[][2], int sensitivity, int sectionOffsets[][2], AnimationType animations[], int delayed, int brightness ) {

    maestro.set_brightness(brightness);
    maestro.update(millis());
    if (!cap.begin(0x5A)) {
       Serial.println("MPR121 not found, check wiring?");
       mprFound = false;
    } else {
        Serial.println("MPR121 found!");
        mprFound = true;
    }
    Serial.print("Buttons: ");
    Serial.println(numButtons);
    for (int i = 0; i < numButtons; i++) {
      
        buttons[i].Setup(pins[i], sensitivity, sectionOffsets[i], animations, delayed, &sections[i], i, &maestro);
    }
    calibrateButtons();
}

template <int numButtons, int section_x, int section_y>
void WTFButtonSet<numButtons, section_x, section_y>::calibrateButtons() {
    for ( WTFButton &button : buttons ) {
        if (button.senseController == 0) {
            Serial.print("Calibrating ");
            Serial.print(button.senseController);
            Serial.print(":");
            Serial.print(button.sensePin);
            Serial.print(" : ");
            button.calibrate();
            Serial.println(button.senseThreshold);
        }
    }
}

template <int numButtons, int section_x, int section_y>
void WTFButtonSet<numButtons, section_x, section_y>::checkStates(unsigned long time) {
    if (mprFound) {
        mprStates = cap.touched();
    }
    Serial.print("[false");
    for ( WTFButton &button : buttons ) {
          if (button.senseController == 1)  {
             if(mprFound) {
                  button.checkState(time, &mprStates);
             }
          } else {
              button.checkState(time);
          }
    }
    Serial.println("]");
    std::sort(buttons, buttons+numButtons);
    
}

template <int numButtons, int section_x, int section_y>
void WTFButtonSet<numButtons, section_x, section_y>::updateLights(CRGB leds[]) {
    for ( WTFButton &button : buttons ) {
        button.updateLights(leds);
    }    
}


