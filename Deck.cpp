using namespace std;

#include <random>
#include "Command.h"
#include "Card.h"
#include "Deck.h"
#include <iostream>


namespace {
    //initialize the deck of the card in certain order
    const static int CARD_COUNT = 52;
    void initDeck(Card* (&cards)[52]){
        for (int i = 0; i < CARD_COUNT; i ++){
            cards[i] = new Card(Suit(i/13), Rank(i%13));

        }
    }
}

//deck constructor
Deck::Deck(const int& s){
    seed_ = s;
    initDeck(cards_);
    shuffle();
}

//shuffle the card deck (pre-ordered upon construction)
void Deck::shuffle(){
    static std::mt19937 rng(seed_);

    int n = CARD_COUNT;

    while (n > 1) {
        int k = (int)(rng() % n);
        --n;
        Card *c = cards_[n];
        cards_[n] = cards_[k];
        cards_[k] = c;
    }
}

void Deck::printDeck() const{
    for (int i = 0; i < CARD_COUNT; i++){
        cout << *(cards_[i]);
        if ((i+1)%13 == 0){
            cout << endl;
        }else{
            cout << " ";
        }
    }
    cout << ">";
}

// get card from the deck for the given index
Card* Deck::getCard(const int &i){
    return cards_[i];
}