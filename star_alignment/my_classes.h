#include <iostream>
#include <cmath>
#include <cfloat>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sys/time.h>

using namespace std;

#define MAX_SIZE 5000 + 1
#define MAX_SEQ 100
#define SIMILARITY 'S'
#define DISTANCE 'D'
#define GAP_C '-'

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
double score_sp(char alignment[MAX_SEQ][MAX_SIZE], int alignment_len, Score *score, int kseq);
