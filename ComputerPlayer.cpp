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

ComputerPlayer::ComputerPlayer(int p) : Player(p){}

bool ComputerPlayer::isCpu(){
    return true;
}

void ComputerPlayer::printPlayer(const map<Suit, set<Rank> > &playedCards){
    //do not print anything
}

bool ComputerPlayer::playCard(Card &card, map<Suit, set<Rank> > &playedCards){

    //play the first card that is playable
    if (card == Card(SPADE, SEVEN)){
        set<Rank> newSet;
        newSet.insert(card.getRank());
        playedCards[card.getSuit()] = newSet;
        return true;
    }


    vector<Card*> playHand = hand();
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
            return true;                  //break away from the loop
        }
    
    }

    card = *(playHand.at(0));
    discardAtIdx(0, true);
    return false;
}

bool ComputerPlayer::discard(Card &card, map<Suit, set<Rank> > &playedCards){
    return true; //not used
}