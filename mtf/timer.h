#ifndef __TIMER_H__
#define __TIMER_H__

#include <ctime>

// A quick class to have diagnostic on time

class Timer {
  public:
    Timer();
    double updateAnchorTime();
    double getTimeDiff();
  private:
    time_t anchorTime;
};

#endif
