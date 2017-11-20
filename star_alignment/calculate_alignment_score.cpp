#include "my_classes.h"

Score *score;
int kseq;

int main(void) {
    char alignment[MAX_SEQ][MAX_SIZE];
    int alignment_len;
    
    double m,M,g;
    char type;
    
    cin >> kseq;
    
    for(int i = 0 ; i < kseq; i++) {
        cin >> alignment[i];
    }
    
    alignment_len = strlen(alignment[0]);
    
    cin >> type >> M >> m >> g;
    
    score = new Score(type, M, m, g);
    
    cout << "Score (SP): " << score_sp(alignment,alignment_len,score,kseq) << endl;
    return 0;
}
