
#include "my_classes.h"

using namespace std;

#define MAX_OPT_SIZE 30 + 1
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) > (y)) ? (x) : (y))

const int kseq = 5;
const double INF = 0x3f3f3f3f / 5;
double dp[MAX_OPT_SIZE][MAX_OPT_SIZE][MAX_OPT_SIZE][MAX_OPT_SIZE][MAX_OPT_SIZE];
char alpha[kseq][MAX_OPT_SIZE];
int alpha_len[kseq];

Score *score;

double sp_column(char col[]) {
    bool only_gap = true;
    
    for(int i = 0; i < kseq; i++) {
        if(col[i] != GAP_C) {
            only_gap = false;
            break;
        }
    }
    
    if(only_gap) return 0;
    
    double sp = 0.0;
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

double inline MIN_MAX(double x, double y) {
    if(score->getType() == DISTANCE) return x <= y ? x : y;
    return x >= y ? x : y;
}

double opt5(int a, int b, int c, int d, int e) {
    const char type = score->getType();
    char col[kseq];
    
    dp[0][0][0][0][0] = 0;
    
    for(int i = 0; i <= a ; i++) {
        for(int j = 0; j <= b ; j++) {
            for(int k = 0; k <= c ; k++) {
                for(int l = 0; l <= d ; l++) {
                    for(int m = 0; m <= e ; m++) {
                        if(i == 0 && j == 0 && k == 0 && l == 0 && m == 0) continue;
                        
                        dp[i][j][k][l][m] = ((type == DISTANCE) ? (INF) : (-1*INF));
                        
                        for(int bitmask = 1; bitmask < (1 << 5); bitmask++) {
                            int x[kseq] = {i,j,k,l,m};
                            for(int n = 0; n < kseq; n++) {
                                if(bitmask & (1 << n) && x[n] > 0){
                                     col[n] = alpha[n][x[n]-1];
                                     x[n] -= 1;
                                 }
                                else col[n] = GAP_C; 
                            }
                            
                            dp[i][j][k][l][m] = MIN_MAX(dp[i][j][k][l][m], dp[x[0]][x[1]][x[2]][x[3]][x[4]] + sp_column(col));  
                        }
                    }
                }
            }
        }
 
    }
    
    return dp[a][b][c][d][e];
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
    double sp = opt5(alpha_len[0],alpha_len[1],alpha_len[2],alpha_len[3],alpha_len[4]);
    t_end = rtclock();

    cout << "Optimal Alignment of 5 sequences:" << endl;
    cout << "Score (SP): " << sp << endl;
    cout << "Time: " << fixed << setprecision(3) << t_end - t_start << " seconds" << endl;
    
    return 0;
}
