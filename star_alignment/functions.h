#include <iostream>
#include <cmath>
#include <cfloat>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sys/time.h>
#include <list>

using namespace std;

#define MAX_SIZE 5000 + 1
#define MAX_SEQ 100
#define MAX_OPT_SIZE 100 + 1
#define MAX_OPT_SEQ 20
#define GAP_C '-'
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

const double INF = 0x3f3f3f3f / 5;

class Score {
private:
    double match;
    double mismatch;
    double gap;
    
public:
    Score(double M, double m, double g);
    const double getMatch();  
    const double getMismatch();
    const double getGap();
};

double rtclock();
double score_sp(char alignment[MAX_SEQ][MAX_SIZE], int alignment_len, Score *score, int kseq);
void print_alignment(char alignment[MAX_SEQ][MAX_SIZE], int alignment_len, int kseq);
void print_alignment(char alignment[MAX_OPT_SEQ][MAX_OPT_SIZE], int alignment_len, int kseq);
