#include "my_classes.h"

Score::Score(char t, int M, int m, int g) {
    type = t;
    match = M;
    mismatch = m;
    gap = g;
}

const int Score::getType() {
    return type;
}

const int Score::getMatch() {
    return match;
}

const int Score::getMismatch() {
    return mismatch;
}

const int Score::getGap() {
    return gap;
}

