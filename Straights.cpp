#include "Command.h"
#include "Card.h";
#include <cassert>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {

    int shuffleSeed = 0; //get a shuffle seed
    if (argc > 0){
        shuffleSeed = atoi(argv[0]);
    }

    

    /*
    Classes we need:
    Player -> ComputerPlayer, HumanPlayer (make sure you ONLY call Player's constructor from the constructors of ComputerPlayer and HumanPlayer
    Exception -> There could be a bunch of these
    
    */

    // First build the test harness for commands, starting with the quit command

    // We can have a vector of players of size 4, depending on h or c for the type of player we will invoke the constructor of the corresponding player class
    vector<string> players; // will be a vector of Players once we implement the player class
    
    for (int i = 1; i <= 4; i++){ // Accept h or c as inputs only, keep asking the user for input if input is not valid
        string playerType;
        cout << "Is player " << i << " a human(h) or a computer(c)?" << endl;
        cin >> playerType;

        if (playerType != "h" && playerType != "c"){
            i--;
            continue;
        }
        players.push_back(playerType);
    }


    for (int i = 0; i < players.size(); i++){
        cout << i << " " << players.at(i) << endl;
    }

    //Command command;
    //cin >> command;

    /*
    1. quit command

    2. shuffle (with and without random seed), deck command

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

    /*
    while (!cin.eof()) {
        switch (command.type) {
        }
    }
    */

    return 0;
}