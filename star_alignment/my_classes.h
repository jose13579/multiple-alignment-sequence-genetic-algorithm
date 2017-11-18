#include <cstdio>
#include <iostream>
#include <sys/time.h>

using namespace std;

class Score {
private:
    char type;
    double match;
    double mismatch;
    double gap;
    
public:
    Score(char t, double M, double m, double g);
    const char getType();
    const double getMatch();  
    const double getMismatch();
    const double getGap();
};

double rtclock();
