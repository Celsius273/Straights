#ifndef _VIEW_
#define _VIEW_

#include "Card.h"
#include "Command.h"
#include "Deck.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "Game.h"
#include "Controller.h"

#include <vector>

//View class for MVC model
class View{
public:
    View(Controller*, Game*);
    void run();
private:
    Controller* controller_;
    Game* game_;
};

#endif