#ifndef at_nekapuzer_delayloop
#define at_nekapuzer_delayloop

/**
 * call function with delay
 */
class DelayLoop {
  public:
    DelayLoop();
    DelayLoop(long delayMs);
    bool update(long currentMillis);
    void changeDelay(long delayMs);
  private:
    long _delayMs;
    long _lastCallMs = 0;
};

#endif
