#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include "Game.h"
#include "Subject.h"
#include "Observer.h"

#include <cassert>
#include <vector>
#include <iostream>

using namespace std;

//Game constructor
Game::Game(){
    curPlayer_ = 0;
    gameState_ = ENDGAME;
	lastCardPlayedIdx_ = -1; //if this is -1 make sure not to get image to avoid out of bounds error in the view
}

void Game::setup(const vector<bool> &isPlayerHuman, const int &seed){
    lastCardPlayedIdx_=-1;
    playedCards_.clear();
    for (int i = 0; i < players_.size(); i++){
        players_.at(i)->resetCards();
        delete(players_.at(i));
    }
    //delete players
    players_.clear();
    gameState_ = PLAYING;

    for (int i = 0; i < 4; i++){
        if (isPlayerHuman.at(i)){
            // create human player
            players_.push_back(new HumanPlayer(i));
        }
        else{
            // create computer player
            players_.push_back(new ComputerPlayer(i));
        }
    }
    deck_ = new Deck(seed);
    deal();
    lastCardPlayedIdx_=-1;
    notify(NEWSTART);
}

void Game::restartGame(){
    lastCardPlayedIdx_=-1;
    playedCards_.clear();
    for (int i = 0; i < players_.size(); i++){
        players_.at(i)->resetCards();
        delete(players_.at(i));
    }
    //delete players
    players_.clear();

    gameState_ = ENDGAME;
    notify(NONE);
}

void Game::handleCard(const int &cardIdx){
    // assuming human player, of course
    // also assume that the cardIdx is totally valid
    Card c = *(players_.at(curPlayer_)->hand().at(cardIdx)); // not sure if will work

    if (playedCards_.size() == 0 && !(c.getRank() == SEVEN && c.getSuit() == SPADE)){           //if no cards are played and card c is not equal to seven of spade
        cout << "This is not a legal play" << endl;
        return; //do not call notify as we need to make the player play a legal move
    }else{
        if (players_.at(curPlayer_)->playCard(c, playedCards_)){
            // if the player can play the card
            lastCardPlayedIdx_ = ((int) c.getSuit())*RANK_COUNT + ((int) c.getRank() );
            cout << "Player " << curPlayer_+1 <<" plays "<< c << "." << endl;
            notify(NONE);
            nextPlayer();
        }else if (players_.at(curPlayer_)->discard(c, playedCards_)){
            // if the player can discard the card
            cout << "Player " << curPlayer_+1 <<" discards "<< c << "." << endl;
            notify(NONE);
            nextPlayer();
        }else{
            // if the move is illegal
            cout << "Illegal Move" << endl;
            //notify(ILLEGALMOVE);
        }
    }

    //checkround here
    checkRound();
}
void Game::invokeCpuLoop(){
    while (players_.at(curPlayer_)->isCpu() && gameState_ == PLAYING){
        // while current player is a CPU, play the game automatically until it gets to a state where human action is REQUIRED
        Card c = Card(SPADE, SEVEN);

        if (players_.at(curPlayer_)->playCard(c, playedCards_)){        //if you can play the given card as current player, print play
            lastCardPlayedIdx_ = ((int) c.getSuit())*RANK_COUNT + ((int) c.getRank() );
            //cout << c << endl;
        }
        else{       
            // empty placeholder else statement for when the cpu discards a card                                                    
            //cout << "CPU discards" << endl;
        }
        notify(NONE);
        nextPlayer();
        checkRound();
    }
}

int Game::lastCardPlayedIdx() const{
    return lastCardPlayedIdx_;
}

int Game::pointsForPlayer(const int& i) const{
    return players_.at(i)->score();
}

int Game::discardsForPlayer(const int& i) const{
    return players_.at(i)->numDiscards();
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

//replace player as computer player
void Game::replacePlayer(){
    ComputerPlayer* newPlayer = new ComputerPlayer(players_.at(curPlayer_));
    delete(players_.at(curPlayer_));
    players_.at(curPlayer_) = newPlayer;
    notify(NONE); //may change to ragequit
}


vector<int> Game::getCurPlayerHand() const{
	vector<int> cardVals;
	for (int i=0;i<players_.at(curPlayer_)->hand().size();i++){
		Card *c = players_.at(curPlayer_)->hand().at(i);
		cardVals.push_back( ((int) c->getSuit())*RANK_COUNT + ((int) c->getRank() ) );
		
	}
	return cardVals;
}

vector<int> Game::getCurPlayerDiscard() const{
	vector<int> discardVals;
	for(int i = 0; i <players_.at(curPlayer_)->discard().size();i++){
		Card *c = players_.at(curPlayer_)->discard().at(i);
		discardVals.push_back( ((int) c->getSuit())*RANK_COUNT + ((int) c->getRank() ) );
	}
	return discardVals;
}

vector<int> Game::getPlayableCard() const{
    vector<int> playableVal;
	vector<Card*> validPlayCards = players_.at(curPlayer_)->validPlay(playedCards_);
	
	for(int i = 0; i < validPlayCards.size();i++){
		Card *c = validPlayCards.at(i);
		playableVal.push_back( ((int) c->getSuit())*RANK_COUNT + ((int) c->getRank() ) );
	}

    return playableVal;
}

//print the winner
/*
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
*/
// check if a round has ended or not
void Game::checkRound(){
    for (int i = 0; i < players_.size(); i++){
        if (players_.at(i)->hand().size() != 0){
            //notify(); //round has not ended yet
            return;
        }
    }

    //by the time the for loop ends we know that everyone's hand is empty and the round ends
    gameState_ = ENDROUND;
    
    //now check if the GAME has ended by checking everyone's scores
    checkScores(); 

    if (gameState_ == ENDROUND){
        // if the game has not ended but the round has definitely ended
        restartRound();
        notify(NEWROUND); //TODO: only set the notification to newround if there is at least one human player
        return;
    }
    
    // if the game has ended
    notify(WINNER);
    cout << "Game ends" << endl;
}


// this can only be called if the game state is ENDROUND
void Game::restartRound(){
    lastCardPlayedIdx_=-1;
    playedCards_.clear();
    for (int i = 0; i < players_.size(); i++){
        players_.at(i)->resetCards();
    }
    notify(NONE);
    deck_->shuffle();
    deal();
    cout << "Start of a new round, game is not over" << endl;
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
    notify(NONE);
}

/*
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
        return;
    }
    else{                                                                                       //if not throw exception
        throw Game::InvalidException("You have a legal play. You may not discard.\n>");
    }
}




//print the deck
void Game::printDeck(){
    gameState_ = PLAYING;                                                       //change the state to PLAYING and call deck's print functinon
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
*/
