#include "Card.h"
#include "Command.h"
#include "Deck.h"
#include "Game.h"

#include <vector>

using namespace std;


Game::Game(const vector<string> &players, const int &seed){
    deck_ = new Deck(seed);
}

Deck* Game::deck(){
    return deck_;
}
