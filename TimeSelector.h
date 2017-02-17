#include "LedControl.h"
#include "utils.h"

class TimeSelector {
  public:
    TimeSelector();
    TimeSelector(ShiftPins ledPins, char letter);
    void setValue(int newValue);
    void draw();
    // 15 max
    int value;
  private:
    char _letter;
    LedControl _leds;
};
