#include "Card.h"
#include "Command.h"
#include "Deck.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include "Game.h"

#include <cassert>
#include <vector>
#include <iostream>

using namespace std;

namespace{
}

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

Deck* Game::deck(){
    return deck_;
}

int Game::curPlayer() const{
    return curPlayer_;
}

bool Game::isCurPlayerCpu() const{
    return players_.at(curPlayer_)->isCpu();
}

GameState Game::gameState() const{
    return gameState_;
}

void Game::setState(GameState g){
    gameState_ = g;
}

void Game::playCpu(Card &c){
    if (playedCards_.size() == 0){
        c = Card(SPADE, SEVEN);
    }

    if (players_.at(curPlayer_)->playCard(c, playedCards_)){
        gameState_ = PRINTPLAY;
    }
    else{
        gameState_ = PRINTDISCARD;
    }
}

void Game::playCard(Card c){
    if (playedCards_.size() == 0 && !(c.getRank() == SEVEN && c.getSuit() == SPADE)){
        gameState_ = INVALIDPLAY;
        return;
    }

    if (players_.at(curPlayer_)->playCard(c, playedCards_)){
        gameState_ = PRINTPLAY;
    }
    else{
        gameState_ = INVALIDPLAY;
    }
}

void Game::discard(Card c){
    if (players_.at(curPlayer_)->discard(c, playedCards_)){
        gameState_ = PRINTDISCARD;
    }
    else{
        gameState_ = INVALIDDISCARD;
    }
}

void Game::printDeck(){
    gameState_ = PLAYING;
    deck_->printDeck();
    cout << endl;
}

void Game::printPlayer(){
    players_.at(curPlayer_)->printPlayer(playedCards_);
}

void Game::printPlay(Card c){
    if (gameState_ == PRINTPLAY){
        cout << "Player " << curPlayer_+1 <<" plays "<< c << "." << endl;
        nextPlayer();
    }
    else if (gameState_ == INVALIDPLAY){
        cout << "This is not a legal play." << endl << endl;
    }
    gameState_ = PLAYING;
}

void Game::printDiscard(Card c){
    if (gameState_ == PRINTDISCARD){
        cout << "Player " << curPlayer_ + 1 << " discards " << c << "." << endl;
        nextPlayer();
    }
    else if (gameState_ == INVALIDDISCARD){
        cout << "You have a legal play. You may not discard." << endl << endl;
    }
    gameState_ = PLAYING;
}

void Game::printWinner(){
    int lowestPt = -1;
    for (int i = 0; i < players_.size();i++){
        if (lowestPt == -1 || players_.at(i)->score() < lowestPt){
            lowestPt = players_.at(i)->score();
        }
    }
    for (int i = 0; i < players_.size(); i++){
        if (players_.at(i)->score() == lowestPt){
            cout << "Player " << i + 1 << " wins!"<<endl;
        }
    }
}

void Game::checkRound(){
    // check if a round has ended or not
    for (int i = 0; i < players_.size(); i++){
        if (players_.at(i)->hand().size() != 0){
            return;
        }
    }

    //by the time the for loop ends we know that everyone's hand is empty and the round ends
    gameState_ = ENDROUND;
}

void Game::restartRound(){
    // this can only be called if the game state is ENDROUND
    playedCards_.clear();
    for (int i = 0; i < players_.size(); i++){
        players_.at(i)->resetCards();
    }

    deck_->shuffle();
    deal();
    
    gameState_ = PLAYING;
}

void Game::checkScores(){
    for (int i = 0; i < players_.size(); i++){
        players_.at(i)->printScores();

        players_.at(i)->updateScore();
        if (players_.at(i)->score() >= WIN_POINTS){
            gameState_ = ENDGAME;
        }
    }
}

void Game::deal(){
    //deal cards to all players
    for (int i = 0; i < 52; i++){
        players_.at(i / 13)->addCard(deck_->getCard(i));

        //determine starting player
        if (*(deck_->getCard(i)) == Card(SPADE, SEVEN)){
            curPlayer_ = i / 13;
        }
    }
}

void Game::nextPlayer(){
    // advances the game state to the next player's turn
    curPlayer_ = (curPlayer_ + 1) % 4;
}
