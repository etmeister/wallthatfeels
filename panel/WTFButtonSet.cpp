#include "WTFButtonSet.h"
#include <algorithm>

template <int numButtons, int section_x, int section_y>
void WTFButtonSet<numButtons, section_x, section_y>::updateButtonSet(int pins[][2], int sensitivity, int sectionOffsets[][2], AnimationType animations[], int delayed, int brightness, CRGB leds[] ) {

    maestro.set_brightness(brightness);
    maestro.update(millis());
    if (!cap.begin(MPRADDR)) {
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
    calibrateButtons(leds);
}

template <int numButtons, int section_x, int section_y>
void WTFButtonSet<numButtons, section_x, section_y>::calibrateButtons(CRGB leds[]) {
    Serial.println("Calibrating buttons.");
    for (int i = 0; i < 400; i++) {
        for ( WTFButton &button : buttons ) {        
            if (button.senseController == 0) {
                button.calibrate();
                if ( i % 100 == 0 ) {
                    Serial.print(button.maestroSectionId);
                    Serial.print(":");
                    Serial.println(button.senseThreshold);
                }
            }
        }
    }
    Serial.println(" Buttons calibrated.");
}

template <int numButtons, int section_x, int section_y>
void WTFButtonSet<numButtons, section_x, section_y>::checkStates(unsigned long time) {
    if (mprFound) {
        mprStates = cap.touched();
    }
    for ( WTFButton &button : buttons ) {
          bool pressed = false;
          if (button.senseController == 1)  {
             if(mprFound) {
                   pressed = button.checkState(time, &mprStates);
             }
          } else {
              pressed = button.checkState(time);
          }
          if ( pressed ) {
              int palette = checkNeighbors(button);
              Serial.print("Setting color ");
              Serial.print(palette);
              Serial.print(" for button ");
              Serial.println(button.maestroSectionId);
              button.animation->set_palette(&palettes[palette]);
              button.currentPalette = palette;
          }

    }
    std::sort(buttons, buttons+numButtons);
    
}

template <int numButtons, int section_x, int section_y>\
void WTFButtonSet<numButtons, section_x, section_y>::getColor(int buttonY, int buttonX, buttonResults* choice) {
    int nextButton = buttonY*NUM_PER_ROW + buttonX;
    Serial.print(" ");
    Serial.print(nextButton);
    Serial.print(":");
    for ( WTFButton &button : buttons ) {
      if (button.maestroSectionId == nextButton) {       
          if ( (button.state) || (button.buttonTimer.elapsed() < 500) ) {
              if ( (button.buttonPressed > choice->latest) && (button.currentPalette != 16) ) {
                  choice->latest = button.buttonPressed;
                  choice->color = button.currentPalette;
                  Serial.print("*");
              }
          } 
      }
    }
}

template <int numButtons, int section_x, int section_y>\
int WTFButtonSet<numButtons, section_x, section_y>::checkNeighbors(WTFButton button) {
    buttonResults choice = { 0, 16};
    int buttonX = button.maestroSectionId % NUM_PER_ROW;
    int buttonY = button.maestroSectionId / NUM_PER_ROW;
    bool upY = ((buttonY+1) < NUM_ROWS);
    bool downY = ((buttonY-1) >= 0);
    bool upX = ((buttonX+1) < NUM_PER_ROW);
    bool downX = ((buttonX-1) >= 0);

    if (upY) {
        getColor(buttonY+1, buttonX, &choice);
        Serial.print(choice.color);
        if (upX) {
          getColor(buttonY+1, buttonX+1, &choice);
        Serial.print(choice.color);
        }
        if (downX) {
          getColor(buttonY+1, buttonX-1, &choice);
        Serial.print(choice.color);
        }        
    }
    if (downY) {
        getColor(buttonY-1, buttonX, &choice);
        Serial.print(choice.color);
        if (upX) {
          getColor(buttonY-1, buttonX+1, &choice);
        Serial.print(choice.color);
        }
        if (downX) {
          getColor(buttonY-1, buttonX-1, &choice);
        Serial.print(choice.color);
        }        
    }
    if (upX) {
        getColor(buttonY, buttonX+1, &choice);
        Serial.print(choice.color);
    }
    if (downX) {
        getColor(buttonY, buttonX-1, &choice);
        Serial.print(choice.color);
    }
    Serial.println("");
    if (choice.color == 16) {
        choice.color = random(15);      
    } else {
    }
    return choice.color;
}

template <int numButtons, int section_x, int section_y>
void WTFButtonSet<numButtons, section_x, section_y>::updateLights(CRGB leds[]) {
    for ( WTFButton &button : buttons ) {
        button.updateLights(leds);
    }    
}


