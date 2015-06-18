#include "Card.h"
#include "Command.h"
#include "Deck.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "Game.h"

#include <vector>

using namespace std;


Game::Game(const vector<string> &playerStr, const int &seed){
    for (int i = 0; i < 4; i++){
        if (playerStr.at(i) == "h"){
            // create human player
            players_.push_back(new HumanPlayer(i));
        }
        else if (playerStr.at(i) == "c"){
            // create computer player
        }
    }
    deck_ = new Deck(seed);

    //deal cards to all players
    for (int i = 0; i < 52; i++){
        players_.at((i + 1) / 13)->addCard(deck_->getCard(i));
    }
}

Deck* Game::deck(){
    return deck_;
}
