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
    Gtk::Main  kit( argc, argv ); 

    Game game;
    Controller controller(&game);
    View view(&controller, &game);

    Gtk::Main::run( view ); 
    return 0;
}
