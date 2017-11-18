#include "my_classes.h"

Score::Score(char t, double M, double m, double g) {
    type = t;
    match = M;
    mismatch = m;
    gap = g;
}

const char Score::getType() {
    return type;
}

const double Score::getMatch() {
    return match;
}

const double Score::getMismatch() {
    return mismatch;
}

const double Score::getGap() {
    return gap;
}

double rtclock() {
  struct timezone Tzp;
  struct timeval Tp;
  int stat;
  stat = gettimeofday (&Tp, &Tzp);
  if (stat != 0) printf("Error return from gettimeofday: %d",stat);
  return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}
