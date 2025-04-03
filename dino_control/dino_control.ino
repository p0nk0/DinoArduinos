#include <Arduino.h>

#include "ServoEasing.hpp"
#include "MedianFilterLib.h"

// PWM pins
const int RPWM_Output_1 = 2;
const int LPWM_Output_1 = 3;

const int PWM_min = 100; // need to determine
const int PWM_max = 200;

const int jawPWM = 8;
const int jaw_close = 0; // closed
const int jaw_open = 20; // open
const int jaw_gnash = 10;

bool mouthOpen = false;

const int roar_duration = 1200;
const int growl_duration = 3500;

const int LEyePWM = 9;
const int l_eye_forward = 30;
const int l_eye_back = 150;

const int REyePWM = 10;
const int r_eye_forward = 150;
const int r_eye_back = 30;

const int LidPWM = 11;
const int lid_closed = 60;
const int lid_open = 120;

const int neutral = 90;

// digital pins
const int R_EN_1 = 24;
const int L_EN_1 = 25;
const int relayPin = 26;

int relayState = LOW;
unsigned long previousMillis = 0;
const unsigned long flash_interval = 500;

unsigned long prevAnimationTime = 0;
unsigned long animationTimer = 0;
bool animationEnabled = false;

// analog pins
const int potPin = A0;

// global variables for readSerial() to set
String userInput = "";
bool stringComplete = false;

int potVal = 0;
const int potMin = 0;
const int potMax = 330;

ServoEasing jaw;
ServoEasing LEye;
ServoEasing REye;
ServoEasing Lid;

MedianFilter<int> medianFilter(5);

void setup() {
  pinMode(RPWM_Output_1, OUTPUT);
  pinMode(LPWM_Output_1, OUTPUT);
  pinMode(R_EN_1, OUTPUT);
  pinMode(L_EN_1, OUTPUT);
  pinMode(potPin, INPUT);
  pinMode(relayPin, INPUT);
  
  userInput.reserve(100);

  digitalWrite(R_EN_1, LOW);
  digitalWrite(L_EN_1, LOW);
  digitalWrite(relayPin, LOW);

  medianFilter.AddValue(0);
  medianFilter.AddValue(0);
  medianFilter.AddValue(0);
  medianFilter.AddValue(0);
  medianFilter.AddValue(0);

  jaw.attach(jawPWM, 0);
  LEye.attach(LEyePWM, 90);
  REye.attach(REyePWM, 90);
  Lid.attach(LidPWM, 90);

  jaw.setSpeed(180);
  LEye.setSpeed(180);
  REye.setSpeed(180);
  Lid.setSpeed(300); 

  Serial.begin(115200); // Starts the serial communication

}

void loop() {
  readSerial();

  digitalWrite(R_EN_1, LOW);
  digitalWrite(L_EN_1, LOW);
  // digitalWrite(relayPin, relayState);
  
  if(stringComplete){
    // process user input
    userInput.trim();
    unsigned long start_time = millis();
    if(userInput.equals("j")){
      // jumpscare: come out of hiding, light flashes
      LEye.write(l_eye_forward);
      REye.write(r_eye_forward);
      Lid.write(lid_closed);
      jaw.write(jaw_close);
      while(potVal < (potMax-5)){
        int speedVal = 0;
        if(potVal < 100){
          // ramp up to full speed
          speedVal = map(potVal, potMin, 100, PWM_min, PWM_max);
        } else if(potVal < 200){
          speedVal = PWM_max;
        } else{
          // ramp down to min speed
          speedVal = map(potVal, 200, potMax, PWM_max, PWM_min);
        }
        analogWrite(RPWM_Output_1, speedVal);
        analogWrite(LPWM_Output_1, 0);
        digitalWrite(R_EN_1, HIGH);
        digitalWrite(L_EN_1, HIGH);
        jaw.write(jaw_close);

        int reading = analogRead(potPin);
        if (reading < 350){
          medianFilter.AddValue(reading);
        }
        potVal = medianFilter.GetFiltered();
      }
      analogWrite(RPWM_Output_1, 0);
      analogWrite(LPWM_Output_1, 0);
      digitalWrite(R_EN_1, LOW);
      digitalWrite(L_EN_1, LOW);

      relayState = LOW;
      
      Serial.print("c\n");
    } else if (userInput.equals("r")){
      // roar: open jaw, roar sound plays
      jaw.write(jaw_close);
      Lid.write(lid_open);
      jaw.write(jaw_open);
      delay(roar_duration);
      jaw.write(jaw_close);

      Serial.print("c\n");
      animationEnabled = true;
    } else if (userInput.equals("h")){
      // hide: return to hiding
      LEye.write(l_eye_forward);
      REye.write(r_eye_forward);
      Lid.write(neutral);
      while(potVal > (potMin+5)){
        int speedVal = 100;
        analogWrite(RPWM_Output_1, 0);
        analogWrite(LPWM_Output_1, speedVal);
        digitalWrite(R_EN_1, HIGH);
        digitalWrite(L_EN_1, HIGH);
        
        int reading = analogRead(potPin);
        if (reading < 350){
          medianFilter.AddValue(reading);
        }
        potVal = medianFilter.GetFiltered();
      }
      analogWrite(RPWM_Output_1, 0);
      analogWrite(LPWM_Output_1, 0);
      digitalWrite(R_EN_1, LOW);
      digitalWrite(L_EN_1, LOW);

      relayState = LOW;
      
      Serial.print("c\n");
      animationEnabled = false;
    } else if (userInput.equals("g")){
      // growl: gnash teeth
      Lid.write(neutral);
      jaw.write(jaw_open);
      delay(growl_duration);
      jaw.write(jaw_close);

      Serial.print("c\n");
      animationEnabled = true;
    } else {
      digitalWrite(R_EN_1, LOW);
      digitalWrite(L_EN_1, LOW);
      digitalWrite(relayPin, LOW);
      Serial.print("n\n");
    }
    stringComplete = false;
    userInput = "";
  } else {
    if (animationEnabled){
      int reading = analogRead(potPin);
      if (reading < 350){
        medianFilter.AddValue(reading);
      }
      potVal = medianFilter.GetFiltered();
      
      // roll for random animation
      
      unsigned long currentMillis = millis();
      if(currentMillis - prevAnimationTime >= animationTimer){
        animationTimer = (unsigned long) (random(15*1000, 30*1000));
        int animation = random(7);
        switch(animation){
          case 0:
            // blink
            Lid.easeTo(lid_closed, 180);
            delay(50);
            Lid.easeTo(lid_open, 180);
            break;
          case 1:
            // look left
            LEye.startEaseTo(neutral);
            REye.startEaseTo(r_eye_forward);
            break;
          case 2:
            // look right
            LEye.startEaseTo(l_eye_forward);
            REye.startEaseTo(neutral);
            break;
          case 3:
            // narrow
            LEye.startEaseTo(l_eye_forward);
            REye.startEaseTo(r_eye_forward);
            Lid.easeTo(neutral);
            break;
          case 4:
            // wide
            Lid.easeTo(lid_open, 60);
            break;
          case 5:
            // squint
            Lid.easeTo(neutral);
          case 6:
            // focus
            LEye.startEaseTo(l_eye_forward);
            REye.startEaseTo(r_eye_forward);
          default:
            break;
        }
      }
      if(mouthOpen){
        jaw.easeTo(0, 15);
        mouthOpen = false;
    } else {
      jaw.easeTo(jaw_gnash, 15);
      mouthOpen = true;
    }
    }
  }

}

// example from Christian's Microcontroller lecture
// read from serial in a non-blocking manner
void readSerial(){
  if (Serial.available()){
    char inChar = (char)Serial.read();
    userInput += inChar;
    if (inChar == '\n'){
      stringComplete = true;
    }
  }
}
