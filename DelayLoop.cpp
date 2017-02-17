#include "DelayLoop.h"

DelayLoop::DelayLoop(){};
DelayLoop::DelayLoop(long delayMs) {
  _delayMs = delayMs;
};
void DelayLoop::changeDelay(long delayMs) {
  _delayMs = delayMs;
}
bool DelayLoop::update(long currentMillis) {
  if (_delayMs < currentMillis - _lastCallMs) {
    _lastCallMs = currentMillis;
    return true;
  }
  return false;
};
