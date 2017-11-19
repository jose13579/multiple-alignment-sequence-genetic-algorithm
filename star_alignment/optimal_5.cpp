#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include "my_classes.h"
#include <cmath>
#include <cfloat>

using namespace std;

#define MAX_SIZE 11 + 1
#define SIMILARITY 'S'
#define DISTANCE 'D'
#define GAP_C '-'
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) > (y)) ? (x) : (y))

const int kseq = 5;
const double N_INFINITY = DBL_MIN/2;
double dp[MAX_SIZE][MAX_SIZE][MAX_SIZE][MAX_SIZE][MAX_SIZE];
char alpha[kseq][MAX_SIZE];
int alpha_len[kseq];

Score *score;

double sp_column(char col[]) {
    double sp = 0;
    const double match = score->getMatch();
    const double mismatch = score->getMismatch();
    const double gap = score->getGap();
    
    for(int i = 0 ; i < kseq-1; i++) {
        for(int j = i+1; j < kseq; j++) {
            if(col[i] == col[j]) sp += match;
            else if(col[i] == GAP_C || col[j] == GAP_C) sp += gap; 
            else sp += mismatch; 
        }  
    }
    
    return sp;
}

double opt5(int id[5]) {
    if(dp[id[0]][id[1]][id[2]][id[3]][id[4]] != N_INFINITY) return dp[id[0]][id[1]][id[2]][id[3]][id[4]];
    
    char col[5];
    int id2[5];
    
    for(int bitmask = 1 ; bitmask < (1 << 5); bitmask++) {
        bool call_rec = true;
        
        for(int j = 0 ; j < 5; j++) {
            if(bitmask & (1 << j) && id[j] > 0) {
                 col[j] = alpha[j][id[j]-1];
                 id2[j] = id[j]-1;
             }
            else {
                col[j] = GAP_C;
                id2[j] = id[j];
                if(id[j] == 0) call_rec = false;
            }
        }
        
        if(score->getType() == SIMILARITY)
            dp[id[0]][id[1]][id[2]][id[3]][id[4]] = 
                MAX(dp[id[0]][id[1]][id[2]][id[3]][id[4]],
                (call_rec ? opt5(id2) : 0)+ sp_column(col));
        else
            dp[id[0]][id[1]][id[2]][id[3]][id[4]] = 
                MIN(dp[id[0]][id[1]][id[2]][id[3]][id[4]],
                (call_rec ? opt5(id2) : 0)+ sp_column(col));
        
    }
    
    return dp[id[0]][id[1]][id[2]][id[3]][id[4]];
}


int main(void) {
    double t_start, t_end;
    double m,M,g;
    char type;
    
    for(int i = 0 ; i < kseq; i++) {
        cin >> alpha[i];
        alpha_len[i] = strlen(alpha[i]);
    }
    
    cin >> type >> M >> m >> g;
    
    score = new Score(type, M, m, g);
    
    t_start = rtclock(); 
       
    for(int i = 0; i < MAX_SIZE; i++) {
        for(int j = 0; j < MAX_SIZE; j++) {
            for(int k = 0; k < MAX_SIZE; k++) {
                for(int l = 0; l < MAX_SIZE; l++) {
                    for(int m = 0; m < MAX_SIZE; m++) {
                        dp[i][j][k][l][m] = N_INFINITY;
                    }
                }
            }
        }
    }
    
    dp[0][0][0][0][0] = 0;
    
    int id[5] = {alpha_len[0],alpha_len[1],alpha_len[2],alpha_len[3],alpha_len[4]};
    
    double sp = opt5(id);
    t_end = rtclock();
    
    cout << "Optimal Alignment of 5 sequences:" << endl;
    cout << "Score (SP): " << sp << endl;
    cout << "Time: " << fixed << setprecision(3) << t_end - t_start << " seconds" << endl;
    
    return 0;
}
