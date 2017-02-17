#ifndef at_nekapuzer_utils
#define at_nekapuzer_utils

struct coord {
  byte row;
  byte col;
};

struct ShiftPins {
  int dataPin;
  int latchPin;
  int clockPin;
};

#endif