/* To ensure that the your cards are ordered the same way as
the given program, use this shuffling algorithm.

CARD_COUNT is the constant 52
cards_ is an array of pointers to cards
*/

#include "Command.h"
#include "Card.h";
#include <random>

namespace {
    int seed = 0; //this value can change depending on the command line arguments we pass in (see project specifications)
    const static int CARD_COUNT = 52;
}

void shuffle(Card *cards_[]){
    static std::mt19937 rng(seed);
    int n = CARD_COUNT;

    while (n > 1) {
        int k = (int)(rng() % n);
        --n;
        Card *c = cards_[n];
        cards_[n] = cards_[k];
        cards_[k] = c;
    }
}