#include "LedControl.h"

#define DEBUG_LEDCONTROL 0

LedControl::LedControl() {};
LedControl::LedControl(ShiftPins shiftPins) {
  _dataPin = shiftPins.dataPin;
  _latchPin = shiftPins.latchPin;
  _clockPin = shiftPins.clockPin;
  pinMode(_latchPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);
};

bool LedControl::isValid(byte row, byte col) {
  return row >= 0 && row <= 7 && col >= 0 && col <= 7;
}
bool LedControl::isValid(coord pos) {
  return isValid(pos.row, pos.col);
}

void LedControl::set(byte row, byte col, bool state) {
  if (false == isValid(row, col)) {
    return false;
  }
  if(true == state) {
    // force bit at col to be 0
    _leds[row] &= ~(B10000000 >> col);
  } else {
    // force bit at col to be 1
    _leds[row] |= (B10000000 >> col);
  }
  #if DEBUG_LEDCONTROL
  Serial.println("[Led::set] Set state, row, col");
  Serial.println(state);
  Serial.println(row);
  Serial.println(col);
  #endif
  return;
};

void LedControl::set(coord pos, bool state) {
  return set(pos.row, pos.col, state);
}

bool LedControl::getPrevious(byte row, byte col) {
  if (false == isValid(row, col)) {
    return false;
  };
  return !(_activeLeds[row] & (B10000000 >> col));
};

bool LedControl::getPrevious(coord pos) {
  return get(pos.row, pos.col);
}

bool LedControl::get(byte row, byte col) {
  if (false == isValid(row, col)) {
    return false;
  }
  return !(_leds[row] & (B10000000 >> col));
}

bool LedControl::get(coord pos) {
  return get(pos.row, pos.col);
}

void LedControl::draw() {
  for (byte x=0; x<8; x++) {
    digitalWrite(_latchPin, LOW);
    // activate row x
    shiftOut(_dataPin, _clockPin, LSBFIRST, (B10000000 >> x));
    // write out row bytes
    shiftOut(_dataPin, _clockPin, LSBFIRST, _leds[x]);
    digitalWrite(_latchPin, HIGH);
    _activeLeds[x] = _leds[x];
  };
};

void LedControl::_debugSerialWrite() {
  for (byte x=0; x<8; x++) {
    Serial.print("[led] row ");
    Serial.println(x);
    Serial.print(_leds[x]);
    Serial.print("\t");
    Serial.println(_leds[x], BIN);
  }
};

void LedControl::debug() {
  Serial.println("LED Status");
  for (byte x=0; x < 8; x++) {
    Serial.println(_activeLeds[x], BIN);
  }
  Serial.println("LED next Status");
  for (byte x=0; x < 8; x++) {
    Serial.println(_leds[x], BIN);
  }
}

void LedControl::clear() {
  for (byte x=0; x < 8; x++) {
    _leds[x] = B11111111;
  }
}