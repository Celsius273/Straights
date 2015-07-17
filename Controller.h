#ifndef _CONTROLLER_
#define _CONTROLLER_

#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "Game.h"

#include <vector>
#include <array>

//Controller class for MVC model
class Controller{
public:
    Controller(Game*);

    void startGame(std::vector<bool> const&, int const&);
    void endGame();

    void handlePlayer(int const);
    void handleCard(int const);

private:
    Game* game_;
};

#endif