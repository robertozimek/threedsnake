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

boolean gameStarted = false; // Boolean used to start game when button is pressed

// Snake body array
// -----------------------------
// (column, level, direction change)
// -1 - no snake body at array position
//
// directions changes at position
// 0 - forward
// 1 - backward
// 2 - up
// 3 - left
// 4 - down
// 5 - right
int snake[10][3] = {
  { 27,  2,  0},  // (column, level in array, direction change) 
  {  -1,  -1, -1},
  {  -1,  -1, -1},
  {  -1,  -1, -1},
  {  -1,  -1, -1},
  {  -1,  -1, -1},
  {  -1,  -1, -1},
  {  -1,  -1, -1},
  {  -1,  -1, -1},
  {  -1,  -1, -1}
};

int snakeLength = 1;

// Random point for snake to eat
int randomPoint[2] = {-1, -1}; // column, level

// Snake speed
int snakeSpeed = 800; // inital speed

void setup() {
  // Loops over layer pins and sets to output
  for (int i = 0; i < 4; i++) {
    pinMode(layerPins[i], OUTPUT);
  }

  // Seed random number using noise from analog pin 0
  randomSeed(analogRead(0));

  // Set up controller pins
  pinMode(upPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(downPin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(forwardPin, INPUT);
  pinMode(backPin, INPUT);

  // Hardware interrupt for buttons
  attachInterrupt(digitalPinToInterrupt(upPin), upPressed, RISING);
  attachInterrupt(digitalPinToInterrupt(rightPin), rightPressed, RISING);
  attachInterrupt(digitalPinToInterrupt(downPin), downPressed, RISING);
  attachInterrupt(digitalPinToInterrupt(leftPin), leftPressed, RISING);
  attachInterrupt(digitalPinToInterrupt(forwardPin), forwardPressed, RISING);
  attachInterrupt(digitalPinToInterrupt(backPin), backPressed, RISING);

  // Set number of registers used
  ShiftPWM.SetAmountOfRegisters(numRegisters);

  // Start the shift PWM 
  ShiftPWM.Start(pwmFrequency,maxBrightness);
}

void loop() {
  
}


/*
 * Button Press Functions
 */
void startGameIfNotStarted() {
  if(!gameStarted) {
    gameStarted = true;
  }
}

void upPressed() {
  startGameIfNotStarted());
  snake[0][2] = 2;
}

void rightPressed() {
  startGameIfNotStarted());
  snake[0][2] = 5;
}

void downPressed() {
  startGameIfNotStarted());
  snake[0][2] = 4;
}

void leftPressed() {
  startGameIfNotStarted());
  snake[0][2] = 3;
}

void forwardPressed() {
  startGameIfNotStarted());
  snake[0][2] = 0;
}

void backPressed() {
  startGameIfNotStarted());
  snake[0][2] = 1;
}
