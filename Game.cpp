#include "Card.h"
#include "Command.h"
#include "Deck.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include "Game.h"
#include "Subject.h"

#include <cassert>
#include <vector>
#include <iostream>

using namespace std;

//Game constructor
Game::Game(const vector<string> &playerStr, const int &seed){
    for (int i = 0; i < 4; i++){
        if (playerStr.at(i) == "h"){
            // create human player
            players_.push_back(new HumanPlayer(i));
        }
        else if (playerStr.at(i) == "c"){
            // create computer player
            players_.push_back(new ComputerPlayer(i));
        }
    }
    deck_ = new Deck(seed);
    deal();
    gameState_ = PLAYING;
}

//return deck
Deck* Game::deck(){
    return deck_;
}

//return current player#
int Game::curPlayer() const{
    return curPlayer_;
}

//see if the player is human or not
bool Game::isCurPlayerCpu() const{
    return players_.at(curPlayer_)->isCpu();
}

//return game's gamestate
GameState Game::gameState() const{
    return gameState_;
}

//set game's gamestate
void Game::setState(GameState g){
    gameState_ = g;
}

//replace player as computer player
void Game::replacePlayer(){
    ComputerPlayer* newPlayer = new ComputerPlayer(players_.at(curPlayer_));
    delete(players_.at(curPlayer_));
    players_.at(curPlayer_) = newPlayer;
}

//play for CPU
void Game::playCpu(Card &c){
    if (playedCards_.size() == 0){      //if no cards are played, card is equals to seven of spade
        c = Card(SPADE, SEVEN);
    }

    if (players_.at(curPlayer_)->playCard(c, playedCards_)){        //if you can play the given card as current player, print play
        gameState_ = PRINTPLAY;
    }
    else{                                                           //else print discard
        gameState_ = PRINTDISCARD;
    }
}


//play the card
void Game::playCard(Card c){
    if (playedCards_.size() == 0 && !(c.getRank() == SEVEN && c.getSuit() == SPADE)){           //if no cards are played and card c is not equal to seven of spade
        throw Game::InvalidException("This is not a legal play.\n>");                           //throw exception
        return;
    }

    if (players_.at(curPlayer_)->playCard(c, playedCards_)){                                    //if you can play the card c as current player
        gameState_ = PRINTPLAY;                                                                 //print play
    }
    else{
        throw Game::InvalidException("This is not a legal play.\n>");                           //else throw exception
    }
}

//discard the card
void Game::discard(Card c){
    if (players_.at(curPlayer_)->discard(c, playedCards_)){                                     //if you can discard the card as current player print discard
        gameState_ = PRINTDISCARD;
        //notify();
    }
    else{                                                                                       //if not throw exception
        throw Game::InvalidException("You have a legal play. You may not discard.\n>");
    }
}

//print the deck
void Game::printDeck(){
    gameState_ = PLAYING;                                                       //change the state to PLAYING and call deck's print functinon
    //notify();
    deck_->printDeck();
}

//print the player
void Game::printPlayer(){
    if (!printed_){                                                             //if not printed
        players_.at(curPlayer_)->printPlayer(playedCards_);                     //print player's played cards.
        //notify();
        printed_ = true;                                                        //change printed to be true
    }
}

//print the plays that are made
void Game::printPlay(Card c){
    cout << "Player " << curPlayer_+1 <<" plays "<< c << "." << endl;
    nextPlayer();                                                               //increment player index
    gameState_ = PLAYING;                                                       //change the state to PLAYING
}

//print the discard that are done
void Game::printDiscard(Card c){
    cout << "Player " << curPlayer_ + 1 << " discards " << c << "." << endl;
    nextPlayer();                                                               //increment player index
    gameState_ = PLAYING;                                                       //change the state to PLAYING
}

//print the winner
void Game::printWinner(){
    int lowestPt = -1;
    for (int i = 0; i < players_.size();i++){
        //find lowest point
        if (lowestPt == -1 || players_.at(i)->score() < lowestPt){
            lowestPt = players_.at(i)->score();
        }
    }
    for (int i = 0; i < players_.size(); i++){
        //print if the player has lowestPt (duplicate included)
        if (players_.at(i)->score() == lowestPt){
            cout << "Player " << i + 1 << " wins!"<<endl;
        }
    }
}

// check if a round has ended or not
void Game::checkRound(){
    for (int i = 0; i < players_.size(); i++){
        if (players_.at(i)->hand().size() != 0){
            return;
        }
    }

    //by the time the for loop ends we know that everyone's hand is empty and the round ends
    gameState_ = ENDROUND;
}

// this can only be called if the game state is ENDROUND
void Game::restartRound(){
    playedCards_.clear();
    for (int i = 0; i < players_.size(); i++){
        players_.at(i)->resetCards();
    }

    deck_->shuffle();
    deal();

    gameState_ = PLAYING;
}

//check the score if we have a winner
void Game::checkScores(){
    for (int i = 0; i < players_.size(); i++){
        players_.at(i)->printScores();

        players_.at(i)->updateScore();
        if (players_.at(i)->score() >= WIN_POINTS){
            gameState_ = ENDGAME;
        }
    }
}
//deal cards to all players
void Game::deal(){
    for (int i = 0; i < 52; i++){
        players_.at(i / 13)->addCard(deck_->getCard(i));

        //determine starting player
        if (*(deck_->getCard(i)) == Card(SPADE, SEVEN)){
            curPlayer_ = i / 13;
        }
    }
}

// advances the game state to the next player's turn
void Game::nextPlayer(){
    curPlayer_ = (curPlayer_ + 1) % 4;
    printed_ = false;
}
