#include <iostream>
#include <cmath>
#include <cfloat>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sys/time.h>
#include <list>
#include <fstream>
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

void print_alignment(char alignment[MAX_SEQ][MAX_SIZE], int alignment_len, int kseq, double m, double n, double g) {
    	Score *score;	
	score = new Score(m,n,g);

	ofstream outFile;
    	outFile.open("star_output_input.txt");
	outFile<<kseq<<endl;
	for(int i = 0 ;i < kseq; i++) {
		string str = "";
		for(int j = 0; j < alignment_len; j++) {
		        cout << alignment[i][j];	
		}
		str = std::string(alignment[i]);
		outFile<<str<<endl;
		cout << endl;
	}
	outFile<<score->getMatch()<<" "<<score->getMismatch()<<" "<<score->getGap();
	outFile.close();


	outFile.open("star_output.txt");
	for(int i = 0 ;i < kseq; i++) {
		outFile<<std::string(alignment[i])<<endl;
		//cout << endl;
	}
	outFile.close();
}

void print_alignment(char alignment[MAX_OPT_SEQ][MAX_OPT_SIZE], int alignment_len, int kseq, double m, double n, double g) {
	Score *score;	
	score = new Score(m,n,g);
	
	ofstream outFile;
    	outFile.open("star_output_input.txt");
	outFile<<kseq<<endl;
	for(int i = 0 ;i < kseq; i++) {
		string str = "";
		for(int j = 0; j < alignment_len; j++) {
		        cout << alignment[i][j];
			str += std::string(alignment[i]);
		}
		outFile<<str<<endl;
		cout << endl;
	}
	outFile<<score->getMatch()<<" "<<score->getMismatch()<<" "<<score->getGap();
	outFile.close();

	outFile.open("star_output.txt");
	for(int i = 0 ;i < kseq; i++) {
		outFile<<std::string(alignment[i])<<endl;
		//cout << endl;
	}
	outFile.close();
}
