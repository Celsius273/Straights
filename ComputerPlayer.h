#ifndef _COMPUTERPLAYER_
#define _COMPUTERPLAYER_

#include "Command.h"
#include "Card.h"
#include "Deck.h"
#include "Player.h"

class ComputerPlayer : public Player{
public:
    ComputerPlayer(int);
    virtual bool isCpu();
    virtual void printPlayer(const std::map<Suit, std::set<Rank> >&);
    virtual bool playCard(Card&, std::map<Suit, std::set<Rank> >&);
    virtual bool discard(Card&, std::map<Suit, std::set<Rank> >&);
    
};


#endif