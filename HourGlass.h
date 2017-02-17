#include "DelayLoop.h"
#include "LedControl.h"
#include "utils.h"


bool _canMoveTo(LedControl &leds, coord &pos);

struct Directions {
    struct coord down;
    struct coord left;
    struct coord right;
};

const byte UP = 0;
const byte DOWN = 1;
const byte LEFT = 2;
const byte RIGHT = 3;

class HourGlass {
  public:
    HourGlass();
    HourGlass(ShiftPins alphaLeds, ShiftPins betaLeds);
    bool process(long currentMs);
    void draw();
    void fill();
    void rotate(byte rotation);
    void clear();
    void changeDropDelay(long dropDelayMs);
  private:
    LedControl _alphaLeds;
    LedControl _betaLeds;
    DelayLoop _delay;
    DelayLoop _dropDownDelay;
    // updated by interrupt
    volatile byte _rotation;
    bool _moveLeds(LedControl &leds);
    bool _moveCell(LedControl &leds, byte row, byte col);
    bool _dropDownCell();
    const coord _getDown(coord pos);
    const coord _getLeft(coord pos);
    const coord _getRight(coord pos);
};
