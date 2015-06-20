#include "Card.h"
#include "Command.h"
#include "Deck.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include "Game.h"
#include "Controller.h"
#include "View.h"

#include <vector>
#include <cassert>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

View::View(Controller *c, Game *g) : game_(g), controller_(c){}

void View::run(){
    Command command;
    /*
    1. quit command -- DONE, though this does not mean much

    2. shuffle (with and without random seed), deck command -- DONE, though we should still test WITH a random seed

    3. human players only
    a) print player's hand (identify First player, print legal moves)
    b) play command (legal and illegal plays)
    c) discard command (legal and illegal discards)
    d) scoring

    4. computer players
    a) play command
    b) discard command

    5. ragequit command

    */
    //run this until the game is not ended
    while (game_->gameState() != ENDGAME){
        cout << "A new round begins. It's player "<< game_->curPlayer()+1 << "'s turn to play." << endl;

        //run this while players are playing
        while (game_->gameState() == PLAYING){
            //check if the player is CPU
            if (game_->isCurPlayerCpu()){
                Card toPrint(SPADE, ACE);           //dummy card initialization
                controller_->playCpu(toPrint);

                switch (game_->gameState()){
                    case ENDGAME:{
                        return;             //if it is end game, break off
                    }
                    case PRINTPLAY:{
                        game_->printPlay(toPrint);      //print the card that will be played
                        break;
                    }
                    case PRINTDISCARD:{
                        game_->printDiscard(toPrint);   //print the card that will be discarded
                        break;
                    }
                }
            }
            //if not CPU, it must be player
            else{
                //output the player in play
                game_->printPlayer();

                //if we see that the current player is a computer player then we skip the command/accepting the command and make the game

                cin >> command;

                //surround this and the case statement with a try catch
                //this would catch cards that are in your hand but are not legal plays, and attempts at discard when legal plays still exist
                try{
                    controller_->acceptCommand(command);

                    switch (game_->gameState()){
                        case ENDGAME:{
                            return;         //if it is end game, break off
                        }
                        case RAGEPRINT:{
                            cout << "Player " << game_->curPlayer() + 1 << " ragequits. A computer will now take over." << endl;
                            game_->setState(PLAYING);           //change state to playing
                            break;
                        }
                        case PRINTDECK:{
                            game_->printDeck();                 //print the deck (debug purpose)
                            break;
                        }
                        case PRINTPLAY:{
                            game_->printPlay(command.card);     //print the card that will be played
                            break;
                        }
                        case PRINTDISCARD:{
                            game_->printDiscard(command.card);  //print the card that will be discarded
                            break;
                        }
                    }
                }
                catch (const Game::InvalidException &e){
                    cout << e.message();                        //send exception
                }
            }
            //determine if round has ended or not
            game_->checkRound();

        }


        game_->checkScores();
        //print scores and add scores here
        //round has ended, determine if game has ended or not

        //if gamestate becomes play again, reset the round here
        if (game_->gameState() == ENDROUND){
            game_->restartRound();
        }
    }

    //game has ended, print winner(s)
    game_->printWinner();
}
