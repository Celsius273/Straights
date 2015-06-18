#ifndef _GAME_
#define _GAME_

#include "Card.h"
#include "Command.h"
#include "Deck.h"

#include <vector>

//Model class for MVC model
class Game{
public:
    Game(std::vector<std::string> const&, int const&);
    //return array cards
    //return players
    Deck* deck();
    //void acceptCommand(Command const&);


private:
    //static Game game_;

    //Game(std::vector<std::string> const&, int const&);
    Deck* deck_;
    //std::vector<Player> players;
};

#endif