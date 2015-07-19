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

vector<Card*> Player::validPlay(const map< Suit, set<Rank> > &playedCards){
    vector<Card*> PlayableCards;
	
	if(playedCards.size() > 0){    
		const map< Suit, set<Rank> > currPlayed = playedCards;
	
		cout << "--------------------------" << endl;	        
		cout << "Played card slot size: " << playedCards.size() << endl;
		cout << "Player " << id_+1 << "'s turn"<< endl;
		cout << "Player's card deck size " << hand_.size() << endl;
		cout << "--------------------------" << endl;	        
				
		for(int i = 0 ; i < hand_.size();i++){
						
			if(isCardLegal(*(hand_.at(i)),currPlayed)){
		        PlayableCards.push_back(hand_.at(i));
		    }
		}
	}
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

    //Any suit with 7
    if (card.getRank() == SEVEN){
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
