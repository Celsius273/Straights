#ifndef _CONTROLLER_
#define _CONTROLLER_

#include "Command.h"
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
    void acceptCommand(Command const&);

private:
    Game* game_;
};

#endif