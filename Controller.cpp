#include "Command.h"
#include "Card.h"
#include "Deck.h"
#include "Game.h"
#include "Controller.h"

#include <vector>
#include <array>
#include <iostream>

using namespace std;

Controller::Controller(Game *g) : game_(g){}

void Controller::acceptCommand(const Command &command){
    //Controller only updates the game state
    switch (command.type) {
        case PLAY: {
            //TODO: to be implemented
            break;
        }

        case DISCARD: {
            //TODO: to be implemented
            break;
        }

        case DECK: { // prints the deck, does NOT update the game state
            break;
        }

        case QUIT: {
            break;
        }

        case RAGEQUIT: {
            //TODO: to be implemented
            break;
        }

        default: {
            cerr << "Invalid Command" << endl;
        }
    }
}