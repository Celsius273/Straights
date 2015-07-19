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
            handFrame("Your hand"),
			discardFrame("Discarded Card's grave"){
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
		cardsInHand[i].set_sensitive(false);

        handBox.add(cardsInHand[i]);
    }

	gameBox.pack_start(discardFrame,Gtk::PACK_SHRINK);
	gameBox.pack_start(discardBox, Gtk::PACK_SHRINK);
	for(int i = 0; i < RANK_COUNT;i++){
		cardsInDiscard[i].set( deck.getNullImage() );
		discardBox.add(cardsInDiscard[i]);

		cardsInDiscard[i].set_sensitive(false);
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
    if (game_->gameState() == ENDGAME){
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


void View::updateTopBar(){
    // top bar:
    if (game_->gameState() == ENDGAME){
        startButton.set_sensitive(true);
    }else{
        startButton.set_sensitive(false);   
    }
    // endButton should ALWAYS be sensitive
}

void View::updatePlayedCards(){
    // To be implemented by David
    if(game_->lastCardPlayedIdx() != -1){
        int idx = game_->lastCardPlayedIdx();
        cardsOnTable[idx/13][idx%13]->set( deck.getCardImage(Card(Suit(idx/13),Rank(idx%13)) ) );
    }
    //else{
    if(game_->gameState() == ENDROUND || game_->gameState() == ENDGAME){
        //cout << "RESET PLAYED CARD" << endl;
        for(int i = 0; i < SUIT_COUNT;i++){
            for(int j = 0; j < RANK_COUNT;j++){
                cardsOnTable[i][j]->set( deck.getNullImage() );
            }
        }
    }
    //RESET PLAYEDCARD
}

void View::updatePlayerInfo(){
    // player windows:
    if (game_->gameState() == ENDGAME){
        for (int i=0;i<4;i++){
            playerButtons[i].set_label("H/C: Human");
            isPlayerHuman.at(i) = true;
            playerButtons[i].set_sensitive(true);
            scoreLabels[i].set_text("Score: 0");
            discardLabels[i].set_text("Discards: 0");
        }
    }
    else if (game_->gameState() == ENDROUND){
        Glib::ustring displayStr;
        std::stringstream infoStream;
        for (int i=0;i<4;i++){
            //update player scores
            infoStream << game_->pointsForPlayer(i);
            infoStream >> displayStr;
            scoreLabels[i].set_text("Score: "+displayStr);

            infoStream.clear();
            displayStr = "";            
            playerButtons[i].set_sensitive(true);
            discardLabels[i].set_text("Discards: 0");
        }
    }else{
        Glib::ustring displayStr;
        std::stringstream infoStream;
        for (int i=0;i<4;i++){
            infoStream.clear();
            displayStr = "";
            playerButtons[i].set_label("Ragequit!");

            infoStream << game_->pointsForPlayer(i);
            infoStream >> displayStr;
            scoreLabels[i].set_text("Score: "+displayStr);

            infoStream.clear();
            displayStr = "";

            infoStream << game_->discardsForPlayer(i);
            infoStream >> displayStr;
            discardLabels[i].set_text("Discards: "+displayStr);

            if (game_->curPlayer() == i){
                playerButtons[i].set_sensitive(true);
            }else{
                playerButtons[i].set_sensitive(false);
            }

            
        }
    }
}

void View::updateCardsInHand(){
    // card buttons
    if(game_->gameState() == PLAYING){

        vector<int> cardPile = game_->getCurPlayerHand();
        vector<int> discardPile = game_->getCurPlayerDiscard();
        vector<int> playablePile = game_->getPlayableCard();
        for(int c = 0; c < RANK_COUNT;c++){
            cardsInDiscard[c].set( deck.getNullImage() );
            cardsInHand[c].set_image( *(new Gtk:: Image( deck.getNullImage() )) );
            cardsInHand[c].set_sensitive(false);
        }
        for(int i = 0; i < cardPile.size();i++){  
            cardsInHand[i].set_image( *(new Gtk:: Image( deck.getCardImage(Card(Suit(cardPile.at(i)/13), Rank(cardPile.at(i)%13))) )) );
            /*for(int l = 0; l < playablePile.size(); l++){
                if(playablePile.at(l) == cardPile.at(i)){
                    cardsInHand[i].set_sensitive(false);
                }
                else{
                    cardsInHand[i].set_sensitive(true);
                }
            }*/
			cardsInHand[i].set_sensitive(true);
        }
        for(int j = 0; j < discardPile.size();j++){
            cardsInDiscard[j].set( deck.getCardImage(Card(Suit(j/13),Rank(j%13)) ) );
        }
    }
    if(game_->gameState() == ENDROUND || game_->gameState() == ENDGAME){
        //RESET PLAYEDCARD
        cout << "RESET PLAYED CARD" << endl;
        for(int i = 0; i < SUIT_COUNT;i++){
            for(int j = 0; j < RANK_COUNT;j++){
                cardsOnTable[i][j]->set( deck.getNullImage() );
            }
        }
        

        //RESET PLAYER CARD
        cout << "RESET PLAYER CARD" << endl;
        for(int i = 0; i < RANK_COUNT;i++){
            cardsInHand[i].set_image( *(new Gtk:: Image( deck.getNullImage() )) );    
        }
    }
}


//TODO: implement the update method here
void View::update(Notification n){
    // render the 4 components
    updateTopBar();
    updatePlayedCards();
    updatePlayerInfo();
    updateCardsInHand();
    //TODO: REFACTOR THE SHIT OUT OF THIS

    switch (n){
        case NEWSTART:{
            Gtk::Dialog dialog( "STARTING GAME LOL", *this );
            Gtk::Label nameLabel( "Game started with seed");
            Gtk::VBox* contentArea = dialog.get_vbox();
            contentArea->pack_start( nameLabel, true, false );
            nameLabel.show();

            Gtk::Button *okButton = dialog.add_button( Gtk::Stock::OK, Gtk::RESPONSE_OK);
            int result = dialog.run();
            break;
        }
        case NEWROUND:{
			/*Gtk::Dialog dialog( "No winner was decided, new round has started", *this );
			Gtk::Label newRoundLabel( "Game restarted with given seed" );
			Gtk::VBox* newRoundContent = dialog.get_vbox();
			newRoundContent->pack_start(newRoundLabel, true, false);
			newRoundLabel.show();
			
			Gtk::Button *okButton = dialog.add_button( Gtk::Stock::OK, Gtk::RESPONSE_OK);
			int result = dialog.run();*/ 
            break;
        }
        case WINNER:{
            break;
        }default:{
            break;
        }
    }

    /*
    // Create the message dialog box with stock "Ok" and "Cancel" buttons.
    Gtk::Dialog dialog( "Text Entry Dialog Box", *this );
    
    Gtk::Entry   nameField;                  // Text entry for the user's name
    Gtk::Label   nameLabel( "Please enter your name:" );
    
    // Add the text entry widget to the dialog box.
    // Add the text entry widget to the vertical box section of the dialog box.
    Gtk::VBox* contentArea = dialog.get_vbox();
    contentArea->pack_start( nameLabel, true, false );
    contentArea->pack_start( nameField, true, false );
    
    nameField.set_text( "" );
    nameLabel.show();
    nameField.show();
    
    // Add two standard buttons, "Ok" and "Cancel" with the appropriate responses when clicked.
    Gtk::Button * okButton = dialog.add_button( Gtk::Stock::OK, Gtk::RESPONSE_OK);
    Gtk::Button * cancelButton = dialog.add_button( Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    
    // Wait for a response from the dialog box.
    int result = dialog.run();
    std::string name;
    switch (result) {
        case Gtk::RESPONSE_OK:
        case Gtk::RESPONSE_ACCEPT:
            name = nameField.get_text();
            std::cout << "Entered '" << name << "'" << std::endl;
            break;
        case Gtk::RESPONSE_CANCEL:
            std::cout << "dialog cancelled" << std::endl;
            break;
        default:
            std::cout << "unexpected button clicked" << std::endl;
            break;
    } // switch
    */
}
