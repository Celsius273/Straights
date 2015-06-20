#include "Command.h"
#include "Card.h"
#include "Deck.h"
#include "Game.h"
#include "Controller.h"
#include "View.h"

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
    if (argc > 1){
        shuffleSeed = atoi(argv[1]);
    }

    // We can have a vector of players of size 4, depending on h or c for the type of player we will invoke the constructor of the corresponding player class
    vector<string> players;

    for (int i = 1; i <= 4; i++){ // Accept h or c as inputs only, keep asking the user for input if input is not valid
        string playerType;
        cout << "Is player " << i << " a human(h) or a computer(c)?" << endl << ">";
        cin >> playerType;

        if (playerType != "h" && playerType != "c"){
            i--;
            continue;
        }
        players.push_back(playerType);
    }

    Game game(players, shuffleSeed);
    Controller controller(&game);
    View view(&controller, &game);

    view.run();

    return 0;
}
