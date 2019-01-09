#ifndef WTFPANEL_DEFINED
#define WTFPANEL_DEFINED

#include "Adafruit_MPR121-Wire1.h"
#define MPRADDR 0x5A
#define NUM_LEDS 150
#define DATA_PIN 13
#define SECTION_X 4
#define SECTION_Y 4
#define NUM_PER_ROW 3
#define NUM_ROWS 8
#define TOTAL_X SECTION_X*NUM_PER_ROW
#define TOTAL_Y SECTION_Y*NUM_ROWS
#define NUM_BUTTONS NUM_PER_ROW*NUM_ROWS

/*
 * 
 * mapping of touch sensor pins and controllers to buttons. format is {pin,controller}
 * where controller 0 uses the native teensy touchread and controller 1 is an mpr121
 */

#endif

