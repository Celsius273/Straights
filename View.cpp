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

    //day 1: 3 hours spent, only need to build test "harness" for game and make game accept commands and execute them
    MILESTONE for day 2: finish human players, we SHOULD have enough time for this shit, edit: NO NO WE DONT FUCK DAMMIT
    */

    //it seems that when we start the program after initializing the players the cards are dealt and we start the game

    //so something like while game.state != endGame, and within it we have a nested loop saying while game.state != endRound

    Command command;
    while (game_->gameState() != ENDGAME){
        cout << "A new round begins. Itfs player "<< game_->curPlayer()+1 << "fs turn to play." << endl;

        while (game_->gameState() == PLAYING){
            if (game_->isCurPlayerCpu()){
                Card toPrint(SPADE, ACE);
                controller_->playCpu(toPrint);

                switch (game_->gameState()){
                    case ENDGAME:{
                        return;
                    }
                    case PRINTPLAY:{
                        game_->printPlay(toPrint);
                        break;
                    }
                    case PRINTDISCARD:{
                        game_->printDiscard(toPrint);
                        break;
                    }
                }
            }
            else{
                //output the player in play
                game_->printPlayer();

                //if we see that the current player is a computer player then we skip the command/accepting the command and make the game 

                cin >> command;

                //surround this and the case statement with a try catch
                //this would catch cards that are in your hand but are not legal plays, and attempts at discard when legal plays still exist
                controller_->acceptCommand(command);

                switch (game_->gameState()){
                    case ENDGAME:{
                        return;
                    }
                    case RAGEPRINT:{
                        cout << "Player " << game_->curPlayer()+1 << " ragequits. A computer will now take over." << endl;
                        game_->setState(PLAYING);
                        break;
                    }
                    case PRINTDECK:{
                        game_->printDeck();
                        break;
                    }
                    case PRINTPLAY:{
                        game_->printPlay(command.card);
                        break;
                    }
                    case INVALIDPLAY:{
                        game_->printPlay(command.card);
                        break;
                    }
                    case PRINTDISCARD:{
                        game_->printDiscard(command.card);
                        break;
                    }

                    case INVALIDDISCARD:{
                        game_->printDiscard(command.card);
                        break;
                    }
                }
            }
            //determine if round has ended or not
            game_->checkRound();

        }

        
        game_->checkScores();
        //print scores and add scores here
        //round has ended, determine if game has ended or not
        /*
        for all players:

        Player <x>'s discards: <list of discards>
        Player <x>'s score: <old score> + <score gained> = <new score>
        */

        if (game_->gameState() == ENDROUND){
            game_->restartRound();
        }

        //if gamestate becomes play again, reset the round here
    }
    game_->printWinner();

    cout << "GAME OVER BITCHES" << endl;
    //game has ended, print winner(s)
}