#include "functions.h"
#include <fstream>

// TODO MULTI THREAD

using namespace std;

double dp[MAX_SIZE][MAX_SIZE];

//char aux1[MAX_SIZE], aux2[MAX_SIZE];
int aux_size;

char alpha[MAX_SEQ][MAX_SIZE];
int alpha_len[MAX_SEQ];

int alignment_len;
int kseq;

Score *score;

void print_mat(int mat[MAX_SEQ][MAX_SEQ], int n, int m) {
    for(int i = 0 ; i < n ; i++) {
        for(int j = 0 ; j < m ; j++) {
             cout << right << setw(5) << mat[i][j];
        }
        cout << endl;
    }
}

void get_alignment(char a[], char b[], int m, int n, char aux1[MAX_SIZE], char aux2[MAX_SIZE]) {
    if(m == 0 && n == 0) return;
    
    if(a[m-1] == b[n-1] && dp[n][m] == dp[n-1][m-1]+score->getMatch()) {
        aux1[aux_size] = a[m-1];
        aux2[aux_size] = b[n-1];
        aux_size++;
        
        get_alignment(a,b,m-1,n-1,aux1,aux2);
    }
    
    else if(n > 0 && dp[n][m] == dp[n-1][m]+score->getGap()) {
        aux1[aux_size] = GAP_C;
        aux2[aux_size] = b[n-1];
        aux_size++;

        get_alignment(a,b,m,n-1,aux1,aux2); 
    }
    
    else if(m > 0 && dp[n][m] == dp[n][m-1]+score->getGap()) {
        aux1[aux_size] = a[m-1];
        aux2[aux_size] = GAP_C;
        aux_size++;
        
        get_alignment(a,b,m-1,n,aux1,aux2);
    }
    
    else if(a[m-1] != b[n-1] && dp[n][m] == dp[n-1][m-1]+score->getMismatch()) {
        aux1[aux_size] = a[m-1];
        aux2[aux_size] = b[n-1];
        aux_size++;
        
        get_alignment(a,b,m-1,n-1,aux1,aux2);
    }
    
    else {
        cout << "WARNING!" << endl;
    }
}

double needleman_wunsch(char a[], char b[], int alen, int blen) {
    const double gap = score->getGap();
    const double match = score->getMatch();
    const double mismatch = score->getMismatch();

    for(int i = 1 ; i <= blen ; i++) {
        dp[i][0] = i*gap;
    }
    
    for(int j = 0 ; j <= alen ; j++) {
        dp[0][j] = j*gap;
    }

    for(int i = 1 ; i <= blen ; i++) {
        for(int j = 1 ; j <= alen ; j++) {
            dp[i][j] = MIN(dp[i-1][j-1]+((a[j-1]==b[i-1])?match:mismatch), MIN(dp[i][j-1]+gap, dp[i-1][j]+gap)); 
        }        
    }
    
    return dp[blen][alen];
}



void star_aligment(char alignment[MAX_SEQ][MAX_SIZE], int *alignment_len) {
    list<char> ll_alignment[MAX_SEQ];
    list<char>::iterator ll_it[MAX_SEQ];
    double anchor_score[MAX_SEQ][MAX_SEQ];
    double anchor_score_sum[MAX_SEQ];
    
    int anchor;
    
    // get scores to pick the anchor
    memset(anchor_score_sum,0.0,sizeof(anchor_score_sum));
    for(int i = 0; i < kseq; i++) {
        for(int j = i; j < kseq; j++) {
            anchor_score[i][j] = anchor_score[j][i] = (j == i) ? 0 : needleman_wunsch(alpha[i],alpha[j],alpha_len[i],alpha_len[j]);
            anchor_score_sum[i] += anchor_score[i][j];
            anchor_score_sum[j] += anchor_score[i][j];
        }
    }
    
    // pick the anchor
    anchor = 0;
    for(int i = 1; i < kseq; i++) {
        anchor = anchor_score_sum[anchor] <= anchor_score_sum[i] ? anchor : i;
    }
    
    // put anchor in the first linked list
    // linked lists are being used due to O(1) gap insertion in the middle of the list
    for(int j = 0 ; j < alpha_len[anchor]; j++) {
        ll_alignment[0].push_back(alpha[anchor][j]);
    }
    
    // position of current sequence to be added in the linked list
    int curr = 0;
    
    char aux1[MAX_SIZE];
    char aux2[MAX_SIZE];

    // adds the sequences one by one to the multiple aligment
    for(int i = 0 ; i < kseq; i++) {
        if(i == anchor) continue;
        curr++;
        
        needleman_wunsch(alpha[anchor], alpha[i], alpha_len[anchor], alpha_len[i]);
        aux_size = 0;

	
        get_alignment(alpha[anchor],alpha[i], alpha_len[anchor], alpha_len[i], aux1, aux2);


        cout << "array1 " << aux1 <<endl;
	cout << "array2 " << aux2 <<endl;

	char l_sequence[MAX_SEQ][MAX_SIZE];

	cout << "arrays " << l_sequence[0] << endl;

	memcpy(l_sequence[0], aux1, sizeof(char)*MAX_SIZE);
	memcpy(l_sequence[1], aux2, sizeof(char)*MAX_SIZE);

	cout << "arrayss " << l_sequence[0] << " "<< aux1 <<endl;
	print_alignment(l_sequence,MAX_SIZE,kseq, score->getMatch(),score->getMismatch(),score->getGap());
        



        // iterators to linkedlist of aligments
        for(int j = 0; j < curr; j++) {
            ll_it[j] = ll_alignment[j].begin();
        }
        
        // Implements once a gap, always a gap
        int k;
        for(k = aux_size-1; k >=0 || ll_it[0] != ll_alignment[0].end(); k--) {
            if(k < 0) {
                ll_alignment[curr].push_back(GAP_C);
                for(int j = 0; j < curr; j++) ll_it[j]++; 
            }
            
            else if(ll_it[0] == ll_alignment[0].end()) {
                ll_alignment[curr].push_back(aux2[k]);
                
                // add gap for every sequence already aligned
                for(int j = 0; j < curr; j++) {
                    ll_alignment[j].insert(ll_it[j], GAP_C);
                }
            }
            
            // do not add a new gap
            else if(*ll_it[0] == aux1[k] || (*ll_it[0] != GAP_C && aux1[k] != GAP_C)) {
                ll_alignment[curr].push_back(aux2[k]);
                for(int j = 0; j < curr; j++) ll_it[j]++;
            }
            
            // gap in sequence already aligned, just add a gap to the sequence to be added
            else if(*ll_it[0] == GAP_C) {
                ll_alignment[curr].push_back(GAP_C);
                k++;
                for(int j = 0; j < curr; j++) ll_it[j]++;                
            }
            
            else {
                ll_alignment[curr].push_back(aux2[k]);
                
                // add gap for every sequence already aligned
                for(int j = 0; j < curr; j++) {
                    ll_alignment[j].insert(ll_it[j], GAP_C);
                }
            }
        }         
    }
    
    // list to array
    *alignment_len = ll_alignment[0].size();
    for(int i = 0 ; i < kseq; i++) {
        int j = 0;
        for(list<char>::iterator it=ll_alignment[i].begin(); it != ll_alignment[i].end(); ++it,j++) {
            alignment[i][j] = *it;
        }
    }
}

int main(void) {
    double t_start, t_end;
    char alignment[MAX_SEQ][MAX_SIZE];
    int alignment_len;
    
    double m,M,g;

    cin >> kseq;
    
    for(int i = 0 ; i < kseq; i++) {
        cin >> alpha[i];
        alpha_len[i] = strlen(alpha[i]);
    }
    
    cin >> M >> m >> g;
    
    score = new Score(M, m, g);
    
    t_start = rtclock();
    star_aligment(alignment, &alignment_len);
    t_end = rtclock();
    
    cout << "Star Alignment:" << endl;
    print_alignment(alignment,alignment_len,kseq,M,m,g);
    cout << "Score (SP) Distance: "  << score_sp(alignment,alignment_len,score,kseq) << endl;
    cout << "Time: " << fixed << setprecision(3) << t_end - t_start << " seconds" << endl;
    
    return 0;
}
