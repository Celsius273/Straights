#ifndef _HUMANPLAYER_
#define _HUMANPLAYER_

#include "Card.h"
#include "Deck.h"
#include "Player.h"


class HumanPlayer : public Player{
public:
    HumanPlayer(int);
    virtual bool isCpu();
    virtual void printPlayer(const std::map<Suit, std::set<Rank> >&);
    virtual bool playCard(Card&, std::map<Suit, std::set<Rank> >&);
    virtual bool discard(Card&, std::map<Suit, std::set<Rank> >&);
};

#endif