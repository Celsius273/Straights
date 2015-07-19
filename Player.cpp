#include "Card.h"
#include "Deck.h"
#include "Player.h"

#include <map>
#include <set>
#include <cassert>
#include <iostream>
using namespace std;

//player constructor
Player::Player(int i): id_(i){
    score_ = 0;
}

// copy constructor
Player::Player(Player* other){ 
    score_ = other->score_;
    id_ = other->id_;
    hand_ = other->hand_;
    discard_ = other->discard_;
}

//return id
int Player::id() const{
    return id_;
}

//return score
int Player::score() const{
    return score_;
}

//return number of discards
int Player::numDiscards() const{
    int toReturn(discard_.size());
    return toReturn;
}

//return player's card deck
vector<Card*> Player::hand(){
    return hand_;
}

vector<Card*> Player::discard(){
	return discard_;
}

//returns the vector of the cards which are playable
vector<Card*> Player::validPlay(const map< Suit, set<Rank> > &playedCards){
//void Player::validPlay(const map< Suit, set<Rank> > &playedCards){ 
   	vector<Card*> PlayableCards;
	vector<Card*> playerHand = hand();
	
	//cout <<"playedCards.size(): " << playedCards.size() << endl;

	for(int i = 0; i < playerHand.size(); i++){
		if(isCardLegal(*(playerHand.at(i)), playedCards)){
			//cout <<"PLAYABLE @ "<<i<< endl;
			PlayableCards.push_back(playerHand.at(i));
		}		
	}
	//cout <<"PlayableCards.size(): " << 	PlayableCards.size() << endl;
	
	return PlayableCards;
}

//print the card that will be discarded
void Player::printDiscard() const{
    //ONLY FOR DEBUG PLEASE DELETE
    cout << "Discard Pile: ";
    for (int i = 0; i < discard_.size(); i++){
        cout << discard_.at(i) << " ";
    }
    cout << endl << endl;
}

//add another card into the deck
void Player::addCard(Card* card){
    hand_.push_back(card);
}

//update the player's score
void Player::updateScore(){
    score_ += calculateScore();
}

//reset
void Player::resetCards(){
    hand_.clear();
    discard_.clear();
}

//print every player's score after displaying discard piles
void Player::printScores(){
    //display discard piles of the player
    cout << "Player " << id_+1 << "'s discards: ";
    for(int i = 0; i < discard_.size(); i++){
        cout << *(discard_.at(i));
        if (i != discard_.size() - 1){
            cout << " ";
        }
    }
    cout << endl;

    //calculate player's new score and print
    int newScore = calculateScore();
    cout << "Player " << id_ + 1 << "'s score: " << score_ << " + " << newScore << " = " << score_+newScore << endl;
}

//calculate the total score
int Player::calculateScore(){
    int sc = 0;
    for (int i = 0; i < discard_.size(); i++){
        sc += (1 + int(discard_.at(i)->getRank()));
    }
    return sc;
}

//check if the selected card is a legal play
bool Player::isCardLegal(Card& card, const std::map<Suit, std::set<Rank> >&playedCards){
	
	//SEVEN SPADE MASTER RACE
	if (card.getRank() == SEVEN && card.getSuit() == SPADE){
		return true;
	} 
    //Any suit with 7 BUT AFTER SPADE
    if (card.getRank() == SEVEN && playedCards.size() > 0){
        return true;
    }

    map<Suit, set<Rank> >::const_iterator rankSet = playedCards.find(card.getSuit());

    if (rankSet == playedCards.end()){
        return false; //if the suit is not played yet
    }
    
    //check if the card can be played based on the map of the played cards
    bool isLegal = false;
    if (int(card.getRank()) > 0){
        isLegal |= !(rankSet->second.find(static_cast<Rank>(int(card.getRank()) - 1)) == rankSet->second.end());
    }
    if (int(card.getRank()) < 12){
        isLegal |= !(rankSet->second.find(static_cast<Rank>(int(card.getRank()) + 1)) == rankSet->second.end());
    }

    return isLegal;
}

//discard the card at index
void Player::discardAtIdx(int idxToDiscard, bool addToDiscard){
    if (addToDiscard){
        discard_.push_back(hand_.at(idxToDiscard)); //add from hand to discard pile
    }
    //delete element from hand
    hand_.erase(hand_.begin() + idxToDiscard);
}
