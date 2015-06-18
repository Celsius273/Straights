#ifndef _PLAYER_
#define _PLAYER_

#include "Card.h"
#include "Command.h"
#include "Deck.h"

#include <map>
#include <set>
#include <vector>

class Player{
public:
    // Accessors
    int id() const;
    int score() const;
    std::vector<Card*> hand();

    // std::vector<Card*> discard(); // may not need
    void addCard(Card* card); //adds a card to the player's hand
    void calculateScore(); //calculates a player's score based on their discard pile and previous score, we may need a new print function for this

    // To be implemented by HumanPlayer and ComputerPlayer
    virtual void printPlayer(std::map<std::string, std::set<std::string> >&) = 0;
    virtual void playCard(Card*, std::map<std::string, std::set<std::string> >&) = 0;
    virtual void discard(Card*) = 0;

protected:
    explicit Player(int); // Constructor, we should only call this from its subclasses

private:
    int id_;
    int score_;
    std::vector<Card*> hand_;
    std::vector<Card*> discard_;
};

#endif