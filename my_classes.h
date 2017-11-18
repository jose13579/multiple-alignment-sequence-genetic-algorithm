#include <cstdio>
#include <iostream>

using namespace std;

class Score {
private:
    char type;
    int match;
    int mismatch;
    int gap;
    
public:
    Score(char t, int M, int m, int g);
    const int getType();
    const int getMatch();  
    const int getMismatch();
    const int getGap();
};
