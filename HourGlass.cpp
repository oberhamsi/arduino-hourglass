#include "HourGlass.h"

#define DEBUG_HOURGLASS 0

const int MOVE_DELAY = 450;
const int INITIAL_DROP_DOWN_DELAY = 1000;

const struct Directions directionsPerRotation[4] = {
  // UP
  {
    { +1, +1}, // down (row, col)
    { +1, 0}, // left
    { 0, +1} //right
  },
  // DOWN
  {
    { -1, -1}, // down
    { -1, 0}, // left
    { 0, -1} //right
  },
  // LEFT
  {
    { +1, -1}, // down
    { 0, -1}, // left
    { +1, 0} //right
  },
  // RIGHT
  {
    {-1, +1}, // down
    {0, +1}, // left
    {-1, 0} // right
  }
};

bool _canMoveTo(LedControl &leds, coord &pos) {
  return true == leds.isValid(pos) && false == leds.get(pos) && false == leds.getPrevious(pos);
}

/**
 * HourGlass
 */
HourGlass::HourGlass(){};
HourGlass::HourGlass(ShiftPins alphaPins, ShiftPins betaPins) {
  _alphaLeds = LedControl(betaPins);
  _betaLeds = LedControl(alphaPins);
  _delay = DelayLoop(MOVE_DELAY);
  _dropDownDelay = DelayLoop(INITIAL_DROP_DOWN_DELAY);
  _rotation = UP;
};
void HourGlass::changeDropDelay(long dropDelayMs) {
  _dropDownDelay.changeDelay(dropDelayMs);
}
void HourGlass::rotate(byte newRotation) {
  _rotation = newRotation;
};

void HourGlass::draw() {
  _alphaLeds.draw();
  _betaLeds.draw();
};

bool HourGlass::_moveCell(LedControl &leds, byte row, byte col) {
  coord pos = {row, col};
  coord left = _getLeft(pos);
  coord right = _getRight(pos);
  coord down = _getDown(pos);
  bool canLeft = _canMoveTo(leds, left);
  bool canRight = _canMoveTo(leds, right);
  bool canDown = _canMoveTo(leds, down);
  #if DEBUG_HOURGLASS
  Serial.println("ROW,COL");
  Serial.println(row);
  Serial.println(col);
  Serial.println("[Led::_moveLeds], canLeft, canRight, canDown");
  Serial.println(canLeft);
  Serial.println(canRight);
  Serial.println(canDown);
  #endif
  if (canLeft || canRight || canDown) {
    leds.set({row, col}, false);
    if (canDown) {
      leds.set(down, true);
    } else if (canLeft && !canRight) {
      leds.set(left, true);
    } else if (canRight && !canLeft) {
      leds.set(right, true);
    } else if (random(0, 2) == 1) {
      leds.set(left, true);
    } else {
      leds.set(right, true);
    }
  }
  #if DEBUG_HOURGLASS
  _alphaLeds.debug();
  #endif
  return canLeft || canRight || canDown;
}

bool HourGlass::_moveLeds(LedControl &leds) {
  bool didMove = false;
  for (byte row=0; row < 8; row++) {
    for (byte col=0; col < 8; col++) {
      if (false == leds.getPrevious(row, col)) {
        continue;
      }
      if (false == _moveCell(leds, row, col)) {
        didMove = true;
      }
    }
  }
  return didMove;
};
bool HourGlass::process(long currentMs) {
  bool didDropCell = false;
  if (true == _dropDownDelay.update(currentMs)) {
    didDropCell = _dropDownCell();
  }
  if (false == _delay.update(currentMs)) {
    return false;
  }
  bool didMoveAlpha = _moveLeds(_alphaLeds);
  bool didMoveBeta = _moveLeds(_betaLeds);
  return didMoveAlpha || didMoveBeta || didDropCell;
};
void HourGlass::fill() {
  for (int slice=0; slice < 12; slice++) {
    for (int row=slice; row >= 0; row--) {
      int col = (slice - row);
      _alphaLeds.set(coord {row, col}, true);
    }
  }
};

const coord HourGlass::_getDown(coord in) {
  in.row += directionsPerRotation[_rotation].down.row;
  in.col += directionsPerRotation[_rotation].down.col;
  return in;
};
const coord HourGlass::_getLeft(coord in) {
  in.row += directionsPerRotation[_rotation].left.row;
  in.col += directionsPerRotation[_rotation].left.col;
  return in;
};
const coord HourGlass::_getRight(coord in) {
  in.row += directionsPerRotation[_rotation].right.row;
  in.col += directionsPerRotation[_rotation].right.col;
  return in;
};

void HourGlass::clear() {
  _alphaLeds.clear();
  _betaLeds.clear();
}

bool HourGlass::_dropDownCell() {
  if (_rotation == LEFT || _rotation == RIGHT) {
    return false;
  }
  if (_rotation == UP) {
    if (true == _alphaLeds.get(coord {7, 7})) {
      _alphaLeds.set(coord {7, 7}, false);
      _betaLeds.set(coord {0, 0}, true);
      return true;
    }
  } else {
    if (true == _betaLeds.get(coord {0, 0})) {
      _betaLeds.set(coord {0, 0}, false);
      _alphaLeds.set(coord {7, 7}, true);
      return true;
    }
  }
  return false;
}
