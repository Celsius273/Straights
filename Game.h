#ifndef _GAME_
#define _GAME_

#include "Card.h"
#include "Command.h"
#include "Deck.h"
#include "Player.h"
#include "HumanPlayer.h"

#include <vector>

//Model class for MVC model
class Game{
public:
    Game(std::vector<std::string> const&, int const&);
    //return array cards
    //return players
    std::vector<Player*> players();
    Deck* deck();
    //void acceptCommand(Command const&);


private:

    //Game(std::vector<std::string> const&, int const&);
    Deck* deck_;
    
    int curPlayer; //game state on which player is currently playing
    int startPlayer;
    bool startRound; //TODO: use an enum here

    std::vector<Player*> players_;
    std::map<string, std::set<string> > playedCards_;
};

#endif