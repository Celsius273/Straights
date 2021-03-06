#ifndef _PLAYER_
#define _PLAYER_

#include "Card.h"
#include "Deck.h"

#include <map>
#include <set>
#include <vector>

class Player{
public:
    // Accessors
    int id() const; //for output PLEASE add 1 to this value
    int score() const;
    int numDiscards() const;
    std::vector<Card*> hand();
	std::vector<Card*> discard();
	std::vector<Card*> validPlay(const std::map<Suit, std::set<Rank> >&);    

    void printDiscard() const;            //PLEASE DELETE THIS DEBUG

    // std::vector<Card*> discard(); // may not need
    void addCard(Card*); //adds a card to the player's hand
    void updateScore();
    void resetCards();
    void printScores();


    // To be implemented by HumanPlayer and ComputerPlayer
    virtual bool isCpu() = 0;
    virtual void printPlayer(const std::map<Suit, std::set<Rank> >&) = 0;
    virtual bool playCard(Card&, std::map<Suit, std::set<Rank> >&) = 0;
    virtual bool discard(Card&, std::map<Suit, std::set<Rank> >&) = 0;

protected:
    explicit Player(int); // Constructor, we should only call this from its subclasses
    Player(Player* other);
    bool isCardLegal(Card&, const std::map<Suit, std::set<Rank> >&);
    void discardAtIdx(int, bool);

private:
    int id_;
    int score_;
    std::vector<Card*> hand_;
    std::vector<Card*> discard_;


    int calculateScore(); //calculates a player's score based on their discard pile and previous score
};

#endif
