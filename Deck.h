#ifndef _DECK_
#define _DECK_

#include "Command.h"
#include "Card.h"
#include <random>
#include <vector>
#include <array>


/* To ensure that the your cards are ordered the same way as
the given program, use this shuffling algorithm.

CARD_COUNT is the constant 52
cards_ is an array of pointers to cards
*/

class Deck{ // singleton
public:
    Deck(const int&);
    void shuffle();
    Card* getCard(const int&);
    void printDeck() const;

private:
    //initial card deck with following order
    Card* cards_[52];
    int seed_;
};

#endif