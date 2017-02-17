#ifndef at_nekapuzer_ledcontrol
#define at_nekapuzer_ledcontrol

#include "Arduino.h"
#include "utils.h"

class LedControl{
  public:
    LedControl();
    LedControl(ShiftPins shiftPins);
    void set(byte row, byte col, bool state);
    void set(coord pos, bool state);
    bool get(byte row, byte col);
    bool get(coord pos);
    bool getPrevious(byte row, byte col);
    bool getPrevious(coord pos);
    bool isValid(byte row, byte col);
    bool isValid(coord pos);
    void draw();
    void clear();
    void debug();
  private:
    void _debugSerialWrite();
    // shift register => LED
    // 1 - 8  => COL
    // 9 - 16 => ROW
    volatile byte _leds[8] = {
      B11111111,
      B11111111,
      B11111111,
      B11111111,
      B11111111,
      B11111111,
      B11111111,
      B11111111
    };
    byte _activeLeds[8] = {
      B11111111,
      B11111111,
      B11111111,
      B11111111,
      B11111111,
      B11111111,
      B11111111,
      B11111111
    };
    int _dataPin;
    int _latchPin;
    int _clockPin;
};

#endif