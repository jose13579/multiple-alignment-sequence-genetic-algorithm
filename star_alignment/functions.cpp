#include "functions.h"

Score::Score(double M, double m, double g) {
    match = M;
    mismatch = m;
    gap = g;
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

double score_sp(char alignment[MAX_SEQ][MAX_SIZE], int alignment_len, Score *score, int kseq) {
    double sp = 0;
    const double match = score->getMatch();
    const double mismatch = score->getMismatch();
    const double gap = score->getGap();
    
    for(int k = 0; k < alignment_len; k++) {
        for(int i = 0; i < kseq-1; i++) {
            for(int j = i+1; j < kseq; j++) {
                if(alignment[i][k] == alignment[j][k]) sp += match;
                else if(alignment[i][k] == GAP_C || alignment[j][k] == GAP_C) sp += gap; 
                else sp += mismatch; 
            }  
        }
    }

    return sp; 
}

void print_alignment(char alignment[MAX_SEQ][MAX_SIZE], int alignment_len, int kseq) {
    for(int i = 0 ;i < kseq; i++) {
        for(int j = 0; j < alignment_len; j++) {
            cout << alignment[i][j];
        }
        cout << endl;
    }
}

void print_alignment(char alignment[MAX_OPT_SEQ][MAX_OPT_SIZE], int alignment_len, int kseq) {
    for(int i = 0 ;i < kseq; i++) {
        for(int j = alignment_len-1; j >= 0; j--) {
            cout << alignment[i][j];
        }
        cout << endl;
    }
}
