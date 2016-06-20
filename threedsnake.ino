/*
 *  3D Snake LED CUBE
 *  
 *  HARDWARE:
 *  74HC595 Shift Registers - led column control
 *  MOSFET - led layer control
 *  6 buttons - control the snake
 *  
 *  SOURCES
 *  ShiftPWM Library by Elco Jacobs http://www.elcojacobs.com/shiftpwm/
 *  
 */

// Pin 8 for shift register latch
// Data & Clock pin are define in the shiftpwn library using the SPI pins
const int ShiftPWN_latchPin = 8;

// Inverts the output of shift register so LOW is HIGH and HIGH is LOW
const bool ShiftPWM_invertOutputs = true;

// Turn off option to shift by 8 at a time compare to all at once
const bool ShiftPWM_balanceLoad = false;

// ShiftPWM Library by Elco Jacobs
// http://www.elcojacobs.com/shiftpwm/
// https://github.com/elcojacobs/ShiftPWM/
#include <ShiftPWM.h> 

// Set up options for the ShiftPWN library
unsigned char maxBrightness = 255;
unsigned char pwmFrequency = 75;
unsigned int numRegisters = 8;
unsigned int numOutputs = numRegisters*8;
unsigned int numRGBLeds = numRegisters*8/3;
unsigned int fadingMode = 0; 
unsigned long startTime = 0;

// Controller Pins
int upPin = 20;
int rightPin = 3;
int downPin = 21;
int leftPin = 19;
int forwardPin = 2;
int backPin = 18;

// LED cube layer pins
int layerPins[4] = {4, 5, 6, 7};


void setup() {


}

void loop() {

}
