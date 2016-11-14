#ifndef __TIMER_H__
#define __TIMER_H__

#include <ctime>

class Timer {
  public:
    Timer();
    double updateAnchorTime();
    double getTimeDiff();
  private:
    time_t anchorTime;
};

#endif
