#include "Command.h"
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
    string suits[4] = { "Clubs: ", "Diamonds: ", "Hearts: ", "Spades: " };
    string ranks[13] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
}

HumanPlayer::HumanPlayer(int p) : Player(p){}

bool HumanPlayer::isCpu(){
    return false;
}

void HumanPlayer::printPlayer(const map<Suit, set<Rank> > &playedCards){

    bool played[13] = { false, false, false, false, false, false, false, false, false, false, false, false, false };

    //debug
    //cout << endl << "player: " << id() + 1 << "|| Score: " << score() << endl;
    //


    cout << "Cards on the table: " << endl;
    for (int i = 0; i < 4; i++){
        cout << suits[i];
        int hiPlayed = 0; // for formatting purposes

        map<Suit, set<Rank> >::const_iterator rankSet = playedCards.find(static_cast<Suit>(i));
        if (rankSet != playedCards.end()){

            //for (set<Rank>::iterator it = playedCards[static_cast<Suit>(i)]  .begin(); it != playedCards[static_cast<Suit>(i)].end(); ++it){
            for (set<Rank>::iterator it = rankSet->second.begin(); it != rankSet->second.end(); ++it){
                played[int(*it)] = true;
                
                if (int(*it) > hiPlayed){
                    hiPlayed = int(*it);
                }
            }

            for (int j = 0; j < 13; j++){
                if (played[j] == true){
                    played[j] = false;
                    cout << ranks[j];
                    if (j == hiPlayed){
                        break;
                    }
                    else{
                        cout << " ";
                    }
                }
            }
        }
        cout << endl;
    }

    cout << "Your hand: ";
    vector<Card*> playerhand = hand();
    for (int i = 0; i < playerhand.size()-1; i++){
        cout << *(playerhand.at(i)) << " ";
    }
    cout << *(playerhand.at(playerhand.size() - 1)) << endl;


    cout << "Legal plays: ";
    
    if (playedCards.size() == 0){
        for (int i = 0; i < playerhand.size(); i++){
            if (*(playerhand.at(i)) == Card(SPADE, SEVEN)){
                cout << *(playerhand.at(i)) << endl << ">";
                return;
            }
        }
    }
    else{
        int hiLegal = 0;
        for (int i = 0; i < playerhand.size(); i++){
            if (isCardLegal(*(playerhand.at(i)), playedCards)){
                hiLegal = i;
            }
        }

        for (int i = 0; i < playerhand.size(); i++){
            if (isCardLegal(*(playerhand.at(i)), playedCards)){
                cout << *(playerhand.at(i)); //do not print space if this is the last legal card
                if (i != hiLegal){
                    cout << " ";
                }
            }
        }
        cout << endl << ">";
    }

    //debug printing method for printing the discard pile
    // printDiscard(); 
}

bool HumanPlayer::playCard(Card& card, map<Suit, set<Rank> > &playedCards){
    if (!isCardLegal(card, playedCards)){
        return false;
    }

    if (playedCards.find(card.getSuit()) == playedCards.end()){ //if suit has not been played
        set<Rank> newSet;
        newSet.insert(card.getRank());
        playedCards[card.getSuit()] = newSet;
    }
    else{
        playedCards[card.getSuit()].insert(card.getRank());
    }
    //discard the card we just played
    vector<Card*> playerhand = hand();
    int idxToDiscard = -1;

    for (int i = 0; i < playerhand.size(); i++){
        if (*(playerhand.at(i)) == card){
            idxToDiscard = i;
            break;
        }
    }

    if (idxToDiscard != -1){
        discardAtIdx(idxToDiscard, false);
    }

    return true;
}

bool HumanPlayer::discard(Card& card, map<Suit, set<Rank> > &playedCards){
    vector<Card*> playerhand = hand();
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
        discardAtIdx(idxToDiscard, true);
    }

    return true;
}