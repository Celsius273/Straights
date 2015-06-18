#include "Card.h"
#include "Command.h"
#include "Deck.h"
#include "Player.h"
#include "HumanPlayer.h"
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
    Classes we need:
    Deck -> a collection of cards
    Player -> ComputerPlayer, HumanPlayer (make sure you ONLY call Player's constructor from the constructors of ComputerPlayer and HumanPlayer)

    Exception -> There could be a bunch of these, might as well throw the acceptable one

    Game -> to take in input and change states/etc, basically an MVC model thingy

    */

    // First build the test harness for commands, starting with the quit command




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
    while (!cin.eof()) {

        //game.printPlayer(); //depends on the player's state
        cin >> command;

        controller_->acceptCommand(command);

        switch (command.type) {
            case PLAY: {
                break;
            }

            case DECK: {
                game_->deck()->printDeck();
                break;
            }

            case QUIT: {
                return;
            }

            default: {
                cerr << "Invalid Command" << endl;
            }
        }
    }
}