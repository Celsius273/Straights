#include "Command.h"
#include <cassert>
#include <sstream>
using namespace std;

istream &operator>>(istream &in, Command &c){
    c.type = BAD_COMMAND;

    string cmd;
    in >> cmd;      //get command

    if (cmd == "play") {
        c.type = PLAY;
        in >> c.card;           //2 token input: get card value
    }
    else if (cmd == "discard") {
        c.type = DISCARD;
        in >> c.card;           //2 token input: get card value
    }
    else if (cmd == "deck") {
        c.type = DECK;
    }
    else if (cmd == "quit") {
        c.type = QUIT;
    }
    else if (cmd == "ragequit") {
        c.type = RAGEQUIT;
    }
    //rest are 1 token input just change the type

    assert(!in.fail() && c.type != BAD_COMMAND);

    return in;
}