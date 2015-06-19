#include "Command.h"
#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "Game.h"
#include "Controller.h"

#include <vector>
#include <array>
#include <iostream>

using namespace std;

void existLegalPlays(Player* player, Card* card){

}

Controller::Controller(Game *g) : game_(g){}

void Controller::acceptCommand(const Command &command){
    //Controller only updates the game state
    switch (command.type) {
        case PLAY: {
            //TODO: to be implemented
            //checks if the play is valid, make the player get stuck in a loop if the play is invalid
            //when play is valid, update the state and output
            game_->playCard(command.card);

            break;
        }

        case DISCARD: {
            //TODO: to be implemented
            //checks if the discard is valid, make the player get stuck in a loop if the discard is invalid
            //when discard is valid, update the state and output
            game_->discard(command.card); //TODO: Implement
            break;
        }

        case DECK: { // prints the deck, does NOT update the game state
            game_->setState(PRINTDECK);
            break;
        }

        case QUIT: {
            game_->setState(ENDGAME);
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