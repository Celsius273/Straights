#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include "Game.h"
#include "Controller.h"
#include "View.h"

#include <gtkmm.h>
#include <vector>
#include <cassert>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <exception>

using namespace std;

View::View(Controller *c, Game *g) : game_(g), controller_(c), 
            startButton("Start new game with seed"),
            endButton("End current game"),
            cardFrame("Cards on table"),
            handFrame("Your hand"){
    g->subscribe(this); // for the observer pattern, subscribe the view
    const Glib::RefPtr<Gdk::Pixbuf> nullCardImage = deck.getNullImage();
    vector<Glib::RefPtr<Gdk::Pixbuf> > cardImages;
    for (int i=0;i<SUIT_COUNT ;i++){
        for (int j=0; j<RANK_COUNT ;j++){
            cardImages.push_back( deck.getCardImage(Card(Suit(i), Rank(j))) );
        }
    }

    add(gameBox);
    gameBox.pack_start(topBox, Gtk::PACK_SHRINK);

    topBox.add(startButton);
    topBox.add(nameField);
    topBox.add(endButton);

    gameBox.pack_start(cardFrame, Gtk::PACK_SHRINK);
    
    for (int i=0;i<SUIT_COUNT;i++){
        for (int j=0;j<RANK_COUNT;j++){
            cardsOnTable[i][j] = new Gtk::Image(nullCardImage);
            suitsBox[i].add(*cardsOnTable[i][j]);
        }
    }

    for (int i=0;i<SUIT_COUNT;i++){
        gameBox.pack_start(suitsBox[i]);
        gameBox.set_spacing(5);
    }

    gameBox.pack_start(playerBox, Gtk::PACK_SHRINK);
    for (int i=0;i<4;i++){
        Glib::ustring strVal;
        std::stringstream ss;
        ss << i+1;
        ss >> strVal;

        playerBoxes[i].add(playerButtons[i]);
        playerBoxes[i].add(scoreLabels[i]);
        playerBoxes[i].add(discardLabels[i]);

        scoreLabels[i].set_text("Score: 0");
        discardLabels[i].set_text("Discards: 0");
        playerButtons[i].set_label("H/C: Human");
        playerFrames[i].set_label("Player " +strVal);
        playerFrames[i].add(playerBoxes[i]);

        playerBox.add(playerFrames[i]);
        isPlayerHuman.push_back(true); //initially set all players to human
    }

    gameBox.pack_start(handFrame, Gtk::PACK_SHRINK);     
    gameBox.pack_start(handBox, Gtk::PACK_SHRINK);    
    for (int i=0;i<RANK_COUNT;i++){
        cardsInHand[i].set_image( *(new Gtk::Image( nullCardImage )) );

        handBox.add(cardsInHand[i]);
    }

    show_all();

    // BINDING ALL FUNCTIONS

    // binding the start button
    startButton.signal_clicked().connect( sigc::mem_fun( *this, &View::startButtonClicked ) );

    // binding the end button
    endButton.signal_clicked().connect( sigc::mem_fun( *this, &View::endButtonClicked ) );

    // binding each of the player buttons
    for (int i=0;i<4;i++){
        playerButtons[i].signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &View::playerButtonClicked), i));
    }

    // binding each of the card functions
    for (int i=0;i<RANK_COUNT;i++){
        cardsInHand[i].signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &View::cardButtonClicked), i));
    }

}

View::~View(){

}

void View::startButtonClicked(){
    // Behavior: pass in the shuffle seed and the boolean array of player types into the game
    // game will call function setup(vector<boolean> isPlayerHuman, int seed)

    // afterwards, set the sensitive of this button to false when model calls notify
    int seed = 0;
    try{
        if( !( isdigit( stoi( nameField.get_text() ) ) ) ){
            seed = stoi(nameField.get_text());
        }
        else{
            seed = 0;
            throw;
        }
    }
    catch (exception& e){
        seed = 0;
    }

    controller_->startGame(isPlayerHuman, seed);
}

void View::endButtonClicked(){
    // Behavior: ask if the player is sure to end the game
    // if yes:   restart the game into the initial start state
    // if no:    don't do anything
    controller_->endGame();
}

void View::playerButtonClicked(const int playerIdx){
    // Behavior: switch gamestate:
    // if game has not started, toggle string between "H/C: Human" and "H/C: Computer"
    // if game has started, cause the player to ragequit and set button sensitive to false
    if (game_->curPlayer() == -1){
        if (isPlayerHuman.at(playerIdx)){
            playerButtons[playerIdx].set_label("H/C: Computer");
            isPlayerHuman.at(playerIdx) = false;
        }else{
            playerButtons[playerIdx].set_label("H/C: Human");
            isPlayerHuman.at(playerIdx) = true;
        }
    }else{
        controller_->handlePlayer(playerIdx);
    }
}

void View::cardButtonClicked(const int cardIdx){
    controller_->handleCard(cardIdx);
}

//TODO: implement the update method here
void View::update(){
    // render the 4 components

    //TODO: REFACTOR THE SHIT OUT OF THIS

    // top bar:
    if (game_->gameState() == ENDGAME){
        startButton.set_sensitive(true);
    }else{
        startButton.set_sensitive(false);   
    }
    // endButton should ALWAYS be sensitive

    // played cards:


    // player windows:
    if (game_->gameState() == ENDGAME){
        for (int i=0;i<4;i++){
            playerButtons[i].set_label("H/C: Human");
            isPlayerHuman.at(i) = true;

            scoreLabels[i].set_text("Score: 0");
            discardLabels[i].set_text("Discards: 0");
        }
    }else{
        for (int i=0;i<4;i++){
            playerButtons[i].set_label("Ragequit!");
            if (game_->curPlayer() == i){
                playerButtons[i].set_sensitive(true);
            }else{
                playerButtons[i].set_sensitive(false);
            }

            Glib::ustring scoreStr;
            std::stringstream scoreStream;
            scoreStream << game_->pointsForPlayer();
            scoreStream >> scoreStr;

            Glib::ustring discardStr;
            std::stringstream discardStream;
            discardStream << game_->discardsForPlayer();
            discardStream >> discardStr;
            //TODO: make this less hacky

            scoreLabels[i].set_text("Score: "+scoreStr);
            discardLabels[i].set_text("Discards: "+discardStr);
        }
    }

    // card buttons
}