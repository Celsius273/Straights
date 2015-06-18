#ifndef _HUMANPLAYER_
#define _HUMANPLAYER_

#include "Command.h"
#include "Card.h"
#include "Deck.h"
#include "Player.h"


class HumanPlayer : public Player{
public:
    HumanPlayer(int);
    virtual void printPlayer(std::map<std::string, std::set<std::string> >&);
    virtual void playCard(Card*, std::map<std::string, std::set<std::string> >&);
    virtual void discard(Card*);
};

#endif