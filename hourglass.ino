#include "HourGlass.h"
#include "DelayLoop.h"
#include "TimeSelector.h"

const unsigned long BUTTON_DEBOUCE_DELAY_MS = 150;
const unsigned long TIMESELECT_TIMEOUT_MS = 1500;
/* struct ShiftPins
  int dataPin;
  int latchPin;
  int clockPin;
*/
const ShiftPins alphaLcdPins = ShiftPins {7, 6, 5};
const ShiftPins betaLcdPins = ShiftPins {12, 11, 10};
const int resetButtonPin = A4;
// A5 is broken
const int potentiometerPin = A3;

const int tiltBottomPin = A1;
const int tiltLeftPin = A2;

HourGlass hourGlass = HourGlass(alphaLcdPins, betaLcdPins);

volatile bool isTimeSelectMode = false;

TimeSelector hourTimeSelector = TimeSelector(alphaLcdPins, 'H');
TimeSelector minuteTimeSelector = TimeSelector(betaLcdPins, 'M');

/**
 * Reset particles
 */
volatile unsigned long lastResetDebounceTimestamp = 0;
void onResetButtonDown() {
  if (digitalRead(resetButtonPin) == LOW) {
    return;
  }
  unsigned long now = millis();
  if (now - lastResetDebounceTimestamp > BUTTON_DEBOUCE_DELAY_MS) {
    //Serial.println("THIS IS THE BUTTON");
    //Serial.println(now);
    lastResetDebounceTimestamp = now;
    hourGlass.clear();
    hourGlass.fill();
  }
};

unsigned long lastPotentiometerTimestamp = 0;
int potentiometerVal = -1;
const int minDiffTrigger= 20;
// 0 - 1024
void readPotentiometer() {
  unsigned long now = millis();
  int newPotVal = analogRead(potentiometerPin);
  bool isFirstRun = potentiometerVal == -1;
  if ( isFirstRun || abs(newPotVal - potentiometerVal) > minDiffTrigger) {
    potentiometerVal = newPotVal;
    lastPotentiometerTimestamp = now;
    isTimeSelectMode = true;
    //long dropDelayMs = 1000 + (potentiometerVal / 1024.0) * 10* 1000;
    unsigned long dropDelayMs;
    unsigned long totalDurationSecs;
    if (potentiometerVal < 700) {
      // potentiometer input from 0 - 600 for 60sec - 900sec
      totalDurationSecs = 60 + ( (900 - 60) * (potentiometerVal/700.));
    } else {
      // pot input from 700 - 1024 for 900 + 3600sec to 900 + 540000sec
      totalDurationSecs = (3600 + 900) + ( ( (54000 + 900) - (3600 + 900)) * ((potentiometerVal-700)/(1024.-700.)) );
    }
    dropDelayMs = (totalDurationSecs/58.) * 1000;
    hourGlass.changeDropDelay(dropDelayMs);
    int mins =  (int)(totalDurationSecs / 60) % 60;
    int hours = (int)((totalDurationSecs - mins * 60.) / 60./60.) % 60;
    hourTimeSelector.setValue(hours);
    minuteTimeSelector.setValue(mins);
    //Serial.print("pot, total, dropdelay");Serial.print(potentiometerVal);Serial.print('\t');Serial.print(totalDurationSecs);Serial.print('\t');Serial.println(dropDelayMs);
    //Serial.print("hours, mins   "); Serial.print((int)hours); Serial.print('\t');Serial.println((int)mins);
  }
  if (isFirstRun || (isTimeSelectMode && now - lastPotentiometerTimestamp > TIMESELECT_TIMEOUT_MS)) {
    isTimeSelectMode = false;
  }
}

void readTilt() {
  bool isLeftHigh = digitalRead(tiltLeftPin) == LOW;
  bool isBottomHigh = digitalRead(tiltBottomPin) == LOW;

  //Serial.print("bottom, left   "); Serial.print(isLeftHigh); Serial.println(isBottomHigh);

  if (isBottomHigh && isLeftHigh) {
    // up
    hourGlass.rotate(0);
  } else if (isBottomHigh == false && isLeftHigh == false) {
    // down
    hourGlass.rotate(1);
  } else if (isBottomHigh == false && isLeftHigh) {
    // left
    hourGlass.rotate(2);
  } else if (isBottomHigh && isLeftHigh == false) {
    hourGlass.rotate(3);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(resetButtonPin, INPUT);
  pinMode(resetButtonPin, LOW);
  
  pinMode(potentiometerPin, INPUT);
  
  pinMode(tiltBottomPin, INPUT);
  digitalWrite(tiltBottomPin, HIGH);
  pinMode(tiltLeftPin, INPUT);
  digitalWrite(tiltLeftPin, HIGH);
 
  hourGlass.fill();
  readPotentiometer();
};
void loop() {
  long now = millis();
  readPotentiometer();
  onResetButtonDown();
  
  if (true == isTimeSelectMode) {
    hourTimeSelector.draw();
    minuteTimeSelector.draw();
  } else {
    readTilt();
    bool didMove = hourGlass.process(now);
    hourGlass.draw();
  }
};

