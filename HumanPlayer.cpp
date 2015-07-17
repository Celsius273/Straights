#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "HumanPlayer.h"

#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <cassert>

using namespace std;

namespace{
    string suits[4] = { "Clubs:", "Diamonds:", "Hearts:", "Spades:" };
    string ranks[13] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
}

//constructor
HumanPlayer::HumanPlayer(int p) : Player(p){}

//returns false if asked "is it CPU?"
bool HumanPlayer::isCpu(){
    return false;
}


void HumanPlayer::printPlayer(const map<Suit, set<Rank> > &playedCards){

    bool played[13] = { false, false, false, false, false, false, false, false, false, false, false, false, false };

    //debug
    //cout << endl << "player: " << id() + 1 << "|| Score: " << score() << endl;
    //


    cout << "Cards on the table:" << endl;
    for (int i = 0; i < 4; i++){
        cout << suits[i];

        map<Suit, set<Rank> >::const_iterator rankSet = playedCards.find(static_cast<Suit>(i));
        if (rankSet != playedCards.end()){

            for (set<Rank>::iterator it = rankSet->second.begin(); it != rankSet->second.end(); ++it){
                played[int(*it)] = true;        //the card on this index is played
            }

            for (int j = 0; j < 13; j++){
                if (played[j] == true){         //if the card on that index is played
                    played[j] = false;          //set it as false for resetting purposes
                    cout << " " << ranks[j];           //output the card
                }
            }
        }
        cout << endl;
    }

    cout << "Your hand:";
    vector<Card*> playerhand = hand();                          //show your list of cards
    for (int i = 0; i < playerhand.size(); i++){
        cout <<  " " << *(playerhand.at(i));
    }
    cout << endl;


    cout << "Legal plays:";
    
    //show every play that is legal plays
    if (playedCards.size() == 0){                               //if game just started and nothing to play
        for (int i = 0; i < playerhand.size(); i++){            
            if (*(playerhand.at(i)) == Card(SPADE, SEVEN)){     //check if the player has seven of spades
                cout << " " <<*(playerhand.at(i)) << endl << ">";
                return;                                         //if yes, output that and break
            }
        }
    }
    else{                                                       //if game is ongoing
        for (int i = 0; i < playerhand.size(); i++){
            if (isCardLegal(*(playerhand.at(i)), playedCards)){
                cout << " " << *(playerhand.at(i)); //do not print space if this is the last legal card
            }
        }
        cout << endl << ">";
    }

    //debug printing method for printing the discard pile
    // printDiscard(); 
}

//play the card if it is legal
bool HumanPlayer::playCard(Card& card, map<Suit, set<Rank> > &playedCards){
    //if it is not legal, return false
    if (!isCardLegal(card, playedCards)){
        return false;
    }

    if (playedCards.find(card.getSuit()) == playedCards.end()){ //if the suit of the card that we wish to play has not been played before
        //create a new set of rank, insert the card's rank and add a new map content
        set<Rank> newSet;
        newSet.insert(card.getRank());
        playedCards[card.getSuit()] = newSet;
    }
    else{                                                       //if the suit has been played before
        playedCards[card.getSuit()].insert(card.getRank());     //just add your card to playedcard list
    }
    //discard the card we just played
    vector<Card*> playerhand = hand();                          //dummy vector that contains player's hand
    int idxToDiscard = -1;                                      //index to discard

    for (int i = 0; i < playerhand.size(); i++){                //iterate through the vector
        if (*(playerhand.at(i)) == card){                       //if i-th index of the vector is equal to the card that we wish to play
            idxToDiscard = i;                                   //set index to discard as i and get out of the loop
            break;
        }
    }

    if (idxToDiscard != -1){                                    //if we found an index to discard
        discardAtIdx(idxToDiscard, false);                      //call discard at index but do not add it into discard pack
    }

    return true;
}

//discard the card only if there is no card to play
bool HumanPlayer::discard(Card& card, map<Suit, set<Rank> > &playedCards){
    vector<Card*> playerhand = hand();                          //similar to playCard, but instead of adding it into playedCard deck, we add to discard deck
    int idxToDiscard = -1;

    // discard iff there are no valid plays
    //check all the hand and see if there is any valid plays
    for (int i = 0; i < playerhand.size(); i++){
        if (isCardLegal( *(playerhand.at(i)) , playedCards)){
            return false;
        }
        if (*(playerhand.at(i)) == card){
            idxToDiscard = i;
        }
    }

    if (idxToDiscard != -1){
        discardAtIdx(idxToDiscard, true);                       //call discard at index and add it into discard pack
    }

    return true;
}