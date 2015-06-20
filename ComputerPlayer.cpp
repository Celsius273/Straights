#include "Command.h"
#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "ComputerPlayer.h"

#include <string>
#include <iostream>
#include <array>
#include <map>
#include <set>
#include <vector>
#include <cassert>

using namespace std;

//computer player constructor
ComputerPlayer::ComputerPlayer(int p) : Player(p){}

//copy constructor
ComputerPlayer::ComputerPlayer(Player* other) : Player(other){}

//return true since this player is inhumane
bool ComputerPlayer::isCpu(){
    return true;
}

void ComputerPlayer::printPlayer(const map<Suit, set<Rank> > &playedCards){
    //do not print anything
}


//playcard if not, discard first card in hand
bool ComputerPlayer::playCard(Card &card, map<Suit, set<Rank> > &playedCards){
    vector<Card*> playHand = hand();
    //play the first card that is playable
    if (card == Card(SPADE, SEVEN)){
        set<Rank> newSet;
        newSet.insert(card.getRank());
        playedCards[card.getSuit()] = newSet;

        //discard 7 of spades
        for (int i = 0; i < playHand.size(); i++){
            if (*(playHand.at(i)) == card){
                discardAtIdx(i, false);
                return true;
            }
        }
        return true;
    }

    for (int i = 0; i < playHand.size(); i++){

        if (isCardLegal(*(playHand.at(i)), playedCards)){                //is it legal?
            card = *(playHand.at(i));
            if (playedCards.find( playHand.at(i)->getSuit() ) == playedCards.end()){      //is the suit new suit?
                set<Rank> newSet;
                
                newSet.insert(card.getRank());
                playedCards[card.getSuit()] = newSet;                               //add new suit
            }
            else{                                                                  //NO?
                playedCards[card.getSuit()].insert(card.getRank());                 //well just insert it
            }

            discardAtIdx(i, false);
            return true;                  //return true so you don't go down to discarding part
        }
    
    }

    card = *(playHand.at(0));                   //if you come this far, you don't have any valid play so just discard first card from the hand
    discardAtIdx(0, true);
    return false;
}

bool ComputerPlayer::discard(Card &card, map<Suit, set<Rank> > &playedCards){
    return true; //not used
}