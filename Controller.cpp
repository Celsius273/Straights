#include "Command.h"
#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include "Game.h"
#include "Controller.h"

#include <vector>
#include <array>
#include <iostream>

using namespace std;

Controller::Controller(Game *g) : game_(g){}

void Controller::playCpu(Card &card){
    game_->playCpu(card);
}
void Controller::acceptCommand(const Command &command){
    //Controller only updates the game state
    switch (command.type) {
        case PLAY: {
            //checks if the play is valid, make the player get stuck in a loop if the play is invalid
            //when play is valid, update the state and output
            game_->playCard(command.card);

            break;
        }

        case DISCARD: {
            //checks if the discard is valid, make the player get stuck in a loop if the discard is invalid
            //when discard is valid, update the state and output
            game_->discard(command.card); //TODO: Implement
            break;
        }

        case DECK: { // prints the deck, does NOT update the game state
            game_->setState(PRINTDECK);
            break;
        }

        case QUIT: {    //quit this thingy
            game_->setState(ENDGAME);
            break;
        }

        case RAGEQUIT: {    //rage quit gracefully
            game_->setState(RAGEPRINT);
            game_->replacePlayer();
            break;
        }

        default: {
            cerr << "Invalid Command" << endl;
        }
    }
}