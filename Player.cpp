#include "Command.h"
#include "Card.h"
#include "Deck.h"
#include "Player.h"

#include <map>
#include <set>
#include <cassert>
#include <iostream>
using namespace std;

Player::Player(int i): id_(i){
    score_ = 0;
}

int Player::id() const{
    return id_;
}

int Player::score() const{
    return score_;
}

vector<Card*> Player::hand(){
    return hand_;
}

void Player::printDiscard() const{
    //ONLY FOR DEBUG PLEASE DELETE
    cout << "Discard Pile: ";
    for (int i = 0; i < discard_.size(); i++){
        cout << discard_.at(i) << " ";
    }
    cout << endl << endl;
}

void Player::addCard(Card* card){
    hand_.push_back(card);
}

void Player::updateScore(){
    score_ += calculateScore();
}

void Player::resetCards(){
    hand_.clear();
    discard_.clear();
}

void Player::printScores(){
    /*
    for all players:

    Player <x>'s discards: <list of discards>
    Player <x>'s score: <old score> + <score gained> = <new score>
    */
    cout << "Player " << id_+1 << "'s discards: ";
    for(int i = 0; i < discard_.size(); i++){
        cout << *(discard_.at(i));
        if (i == discard_.size() - 1){
            cout << endl;
        }
        else{
            cout << " ";
        }
    }

    int newScore = calculateScore();
    cout << "Player " << id_ + 1 << "'s score: " << score_ << " + " << newScore << " = " << score_+newScore << endl;
}

int Player::calculateScore(){
    int sc = 0;
    for (int i = 0; i < discard_.size(); i++){
        sc += (1 + int(discard_.at(i)->getRank()));
    }
    return sc;
}

bool Player::isCardLegal(Card& card, const std::map<Suit, std::set<Rank> >&playedCards){

    //Any suit with 7
    if (card.getRank() == SEVEN){
        return true;
    }

    map<Suit, set<Rank> >::const_iterator rankSet = playedCards.find(card.getSuit());

    if (rankSet == playedCards.end()){
        return false; //if the suit is not played yet
    }
    
    bool isLegal = false;
    if (int(card.getRank()) > 0){
        isLegal |= !(rankSet->second.find(static_cast<Rank>(int(card.getRank()) - 1)) == rankSet->second.end());
    }
    if (int(card.getRank()) < 12){
        isLegal |= !(rankSet->second.find(static_cast<Rank>(int(card.getRank()) + 1)) == rankSet->second.end());
    }

    return isLegal;
}

void Player::discardAtIdx(int idxToDiscard, bool addToDiscard){
    if (addToDiscard){
        discard_.push_back(hand_.at(idxToDiscard)); //add from hand to discard pile
    }
    //delete element from hand
    hand_.erase(hand_.begin() + idxToDiscard);
}
