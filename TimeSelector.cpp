#include "TimeSelector.h"

TimeSelector::TimeSelector() {};
TimeSelector::TimeSelector(ShiftPins ledPins, char letter) {
  _letter = letter;
  value = 15;
  _leds = LedControl(ledPins);
};
void TimeSelector::setValue(int newValue) {
  value = newValue;
}
void TimeSelector::draw() {
  _leds.set(4,2, true);
  _leds.set(5,3, true);
  _leds.set(6,4, true);
  _leds.set(7,5, true);

  _leds.set(2,4, true);
  _leds.set(3,5, true);
  _leds.set(4,6, true);
  _leds.set(5,7, true);

  if (_letter == 'M') {
    _leds.set(4,3, true);
    _leds.set(4,4, true);
    _leds.set(3,4, true);
  }
  if (_letter == 'H') {
    _leds.set(5,4, true);
    _leds.set(4,5, true);
  }
  int litCount = 0;
  for (int slice=0; slice < 5; slice++) {
    for (int row=slice; row >= 0; row--) {
      int col = (slice - row);
      if (litCount < value) {
        _leds.set(coord {row, col}, true);
        litCount++;
      } else {
        _leds.set(coord {row, col}, false);
      }
    }
  }
  _leds.draw();
}

