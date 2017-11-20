#include "functions.h"

using namespace std;

int kseq;
double *dp;
int dp_len;

char alpha[MAX_OPT_SEQ][MAX_OPT_SIZE];
int alpha_len[MAX_OPT_SEQ];
char alignment[MAX_OPT_SEQ][MAX_OPT_SIZE];

int alignment_len;
Score *score;

int getid(const int y[]) {
    int id = 0;
    
    int lens = 1;
    for(int i = kseq-1 ; i >= 0; i--) {
        id += lens*y[i];
        lens *= alpha_len[i]+1;
    }
    return id;
}

void idToArray(int i, int x[]) {
    int lens = 1;
    int sum_id = 0;
    for(int j = kseq-1 ; j >= 0; j--) {
        x[j] = ((i-sum_id) / lens) % (alpha_len[j]+1);
        
        sum_id += x[j];
        lens *= alpha_len[j]+1;
    }
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

bool inline isdifferent(const int y[], int x[]) {
    for(int i = 0; i < kseq; i++) {
        if(y[i] != x[i]) return true;
    }
     
    return false;
}

void get_alignment(const int y[]) {
    int sum = 0;
    for(int i = 0 ; i < kseq ; i++) sum += y[i];
    if(sum == 0) return;
    
    char col[MAX_OPT_SEQ];
    int x[MAX_OPT_SEQ];
    
    for(int bitmask = 1; bitmask < (1 << kseq); bitmask++) {
        for(int n = 0; n < kseq; n++) {
            x[n] = y[n];
            
            if(bitmask & (1 << n) && x[n] > 0){
                 col[n] = alpha[n][x[n]-1];
                 x[n] -= 1;
             }
            else col[n] = GAP_C; 
        }
        
        double sp = sp_column(col);
        
        if(dp[getid(y)] == dp[getid(x)] + sp && isdifferent(y,x)) {
            for(int i = 0 ; i < kseq ; i++) {
                alignment[i][alignment_len] = col[i];
            }
            
            alignment_len++;
            
            get_alignment(x);
            
            return;
        }  
    }
}

double opt() {
    char col[MAX_OPT_SEQ];
    int x[MAX_OPT_SEQ];
    
    dp[0] = 0;
    
    for(int i = 1; i < dp_len ; i++) {             
        dp[i] = INF;
        
        for(int bitmask = 1; bitmask < (1 << kseq); bitmask++) {
            idToArray(i,x);
            for(int n = 0; n < kseq; n++) {
                if(bitmask & (1 << n) && x[n] > 0){
                     col[n] = alpha[n][x[n]-1];
                     x[n] -= 1;
                 }
                else col[n] = GAP_C; 
            }
            
            dp[i] = MIN(dp[i], dp[getid(x)] + sp_column(col));
        }
    }
    
    return dp[dp_len-1];
}

void allocate_dp() {
    dp_len=1;
    for(int i = 0; i < kseq ; i++) dp_len *= alpha_len[i]+1;
    dp = new double[dp_len];
}

void free_dp() {
    delete[] dp;
}

int main(void) {
    double t_start, t_end;
    double m,M,g;

    cin >> kseq;
    
    for(int i = 0 ; i < kseq; i++) {
        cin >> alpha[i];
        alpha_len[i] = strlen(alpha[i]);
    }
    
    allocate_dp();
    
    cin >> M >> m >> g;
    
    score = new Score(M, m, g);

    t_start = rtclock(); 
    double sp = opt();
    t_end = rtclock();
    
    alignment_len = 0;
    get_alignment(alpha_len);
    
    print_alignment(alignment,alignment_len,kseq);

    cout << "Optimal Alignment of "<< kseq << " sequences:" << endl;
    cout << "Score (SP) Distance: " << sp << endl;
    cout << "Time: " << fixed << setprecision(3) << t_end - t_start << " seconds" << endl;
    
    free_dp();
    return 0;
}
