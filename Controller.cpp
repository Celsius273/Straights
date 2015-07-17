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

void Controller::startGame(const vector<bool> &isPlayerHuman, const int &seed){
    game_->setup(isPlayerHuman, seed);
    game_->invokeCpuLoop();
}

void Controller::endGame(){
    game_->restartGame();
    // do not invoke CPU loop here as there is no way that a CPU can start playing after this command finishes
}

void Controller::handlePlayer(const int playerIdx){
    // we know that right here, the curPlayer_ MUST NOT be -1
    // take that player and make them ragequit
    if (playerIdx == game_->curPlayer() && !(game_->isCurPlayerCpu()) ) {
        game_->replacePlayer();
    }
    game_->invokeCpuLoop();
}

void Controller::handleCard(const int cardIdx){
    game_->handleCard(cardIdx);
    game_->invokeCpuLoop();
}

/*
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
*/