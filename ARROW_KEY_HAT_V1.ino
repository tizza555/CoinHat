/*
Coin Hat Game Controller
Tyler Jorge
DIGF-3010-301
OCAD University
Created on October 2nd, 2023

Based on:
Circuit Playground Fruit Drums
Carter Nelson
www.learn.adafruit.com/circuit-playground-fruit-drums/cirkey-cirkey
*/

#include <Adafruit_Debounce.h> //debounce feature

// Send keyboard and mouse events for each touch pad.
// https://www.arduino.cc/en/Reference/MouseKeyboard
#include <Keyboard.h>

#include <Adafruit_CircuitPlayground.h> //include circuit playground library
#include <Adafruit_Circuit_Playground.h>

#define CAP_THRESHOLD   530  // define the capaticance threshold - this value will need to be changed according to the environment the hat is in (static electricity messes with capacitance)
#define DEBOUNCE        1.5  // value of the debounce function, change this to increase the time between key presses


uint8_t pads[] = {3, 2, 0, 1};  // define number of pads, in my case this is four pads on the 0-3 pins
uint8_t numberOfPads = sizeof(pads)/sizeof(uint8_t);

boolean emulatorActive = false; 

////////////////////////////////////////////////////////////////////////////
void takeAction(uint8_t pad) {
  Serial.print("PAD "); Serial.println(pad);
  switch (pad) {
    case 3:
      sendKey(KEY_LEFT_ARROW);              //
      break;                                //
    case 2:                                 //
      sendKey(KEY_DOWN_ARROW);              //
      break;                                //
    case 0:                                 //     switch/case function for each of the four arrow key inputs
      sendKey(KEY_RIGHT_ARROW);             //
      break;                                //
    case 1:                                 //
      sendKey(KEY_UP_ARROW);                //
      break;                                //
    default:
      Serial.println("END");  // this will never print in the serial monitor, if it does, something is wrong
  }
}

////////////////////////////////////////////////////////////////////////////
boolean capButton(uint8_t pad) {   // turn each of the pads on the CPX into capacative touchpads
  // check if capacitive touch exceeds threshold.
  if (CircuitPlayground.readCap(pad) > CAP_THRESHOLD) {
    return true;  
  } else {
    return false;
  }
}

////////////////////////////////////////////////////////////////////////////
void sendKey(char key) {     // send key function sends key presses to keyboard library
  Keyboard.press(key);    
  Keyboard.releaseAll();
}

////////////////////////////////////////////////////////////////////////////
void setup() {
  // initialize serial monitor
  Serial.begin(9600); 
  
  // initialize Circuit Playground library.
  CircuitPlayground.begin();
}

////////////////////////////////////////////////////////////////////////////
void loop() {
  // Indicate emulator status on red LED.
  CircuitPlayground.redLED(emulatorActive);

  // Check slide switch.
  if (!CircuitPlayground.slideSwitch()) {                // slide switch on the CPX acts as an OFF/ON switch to change the state of the device. this is very helpful with debugging.

    //-----------| KEYBOARD EMULATOR OFF |-------------
    if (emulatorActive) {
      Keyboard.end();
      emulatorActive = false;
    }
    
  } else {

    //-----------| KEYBOARD EMULATOR ON |-------------
    if (!emulatorActive) {
      Keyboard.begin();
      emulatorActive = true;
    } 
 
    // Loop over every pad.
    for (int i=0; i<numberOfPads; i++) {
      
      // Check if pad is touched.
      if (capButton(pads[i])) {
        
        // Do something.
        takeAction(pads[i]);
        delay(DEBOUNCE);
      }
    }
  }
}
