#include "timer.h"

Timer::Timer() {
    anchorTime = time(NULL);
}

double Timer::updateAnchorTime() {
    time_t now = time(NULL);
    double ret = difftime(now, anchorTime);

    this->anchorTime = now;
    return ret;
}

double Timer::getTimeDiff() {
    return difftime(time(NULL), anchorTime);
}
