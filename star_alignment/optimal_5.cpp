
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
char alignment[kseq][MAX_OPT_SIZE];
int alignment_len;
Score *score;

double inline MIN_MAX(double x, double y) {
    if(score->getType() == DISTANCE) return x <= y ? x : y;
    return x >= y ? x : y;
}

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

void print_opt_alignment(char x[kseq][MAX_OPT_SIZE], int x_len) {
    for(int i = 0; i < kseq; i++) {
        for(int j = x_len - 1; j >=0 ; j--) {
            cout << x[i][j];
        }
        cout << endl;
    }
}

bool inline isdifferent(int y[], int x[]) {
    for(int i = 0; i < kseq; i++) {
        if(y[i] != x[i]) return true;
    }
     
    return false;
}

void get_alignment(int y[]) {
    int sum = 0;
    for(int i = 0 ; i < kseq ; i++) sum += y[i];
    if(sum == 0) return;
    
    char col[kseq];
    for(int bitmask = 1; bitmask < (1 << kseq); bitmask++) {
        int x[kseq];

        for(int n = 0; n < kseq; n++) {
            x[n] = y[n];
            
            if(bitmask & (1 << n) && x[n] > 0){
                 col[n] = alpha[n][x[n]-1];
                 x[n] -= 1;
             }
            else col[n] = GAP_C; 
        }
        
        double sp = sp_column(col);
        
        if(dp[y[0]][y[1]][y[2]][y[3]][y[4]] == dp[x[0]][x[1]][x[2]][x[3]][x[4]] + sp && isdifferent(y,x)) {
            for(int i = 0 ; i < kseq ; i++) {
                alignment[i][alignment_len] = col[i];
            }
            
            alignment_len++;
            
            get_alignment(x);
            
            return;
        }  
    }
}

double opt5(int y[]) {
    const char type = score->getType();
    char col[kseq];
    
    dp[0][0][0][0][0] = 0;
    
    for(int i = 0; i <= y[0] ; i++) {
        for(int j = 0; j <= y[1] ; j++) {
            for(int k = 0; k <= y[2] ; k++) {
                for(int l = 0; l <= y[3] ; l++) {
                    for(int m = 0; m <= y[4] ; m++) {
                        if(i == 0 && j == 0 && k == 0 && l == 0 && m == 0) continue;
                        
                        dp[i][j][k][l][m] = ((type == DISTANCE) ? (INF) : (-1*INF));
                        
                        for(int bitmask = 1; bitmask < (1 << kseq); bitmask++) {
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
    
    return dp[y[0]][y[1]][y[2]][y[3]][y[4]];
}

int main(void) {
    double t_start, t_end;
    double m,M,g;
    char type;
    
    int nseq;
    cin >> nseq;
    
    for(int i = 0 ; i < kseq; i++) {
        cin >> alpha[i];
        alpha_len[i] = strlen(alpha[i]);
    }
    
    cin >> type >> M >> m >> g;
    
    score = new Score(type, M, m, g);
    
    t_start = rtclock(); 
    int y[kseq] = {alpha_len[0],alpha_len[1],alpha_len[2],alpha_len[3],alpha_len[4]};
    double sp = opt5(y);
    t_end = rtclock();
    
    memset(alignment,0,sizeof(alignment));
    alignment_len = 0;
    get_alignment(y);
    
    print_opt_alignment(alignment,alignment_len);

    cout << "Optimal Alignment of 5 sequences:" << endl;
    cout << "Score (SP): " << sp << endl;
    cout << "Time: " << fixed << setprecision(3) << t_end - t_start << " seconds" << endl;
    
    return 0;
}
