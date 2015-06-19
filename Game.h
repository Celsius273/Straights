#ifndef _GAME_
#define _GAME_

#include "Card.h"
#include "Command.h"
#include "Deck.h"
#include "Player.h"
#include "HumanPlayer.h"

#include <vector>

enum GameState { PLAYING, PRINTDECK, PRINTPLAY, INVALIDPLAY, PRINTDISCARD, INVALIDDISCARD, ENDROUND, ENDGAME };

// Model class for MVC model
class Game{
public:
    Game(std::vector<std::string> const&, int const&);
    //return array cards
    //return players
    
    //std::vector<Player*> players() const;
    Deck* deck();
    int curPlayer() const;
    GameState gameState() const;
    void setState(GameState);
    
    void playCard(Card);
    void discard(Card);

    void printDeck(); //prints the deck and reverts gamestate from DECK to PLAY
    void printPlayer();
    void printPlay(Card);
    void printDiscard(Card);
    void printWinner();

    void checkRound();
    void restartRound(); //changes gamestate from ENDROUND to PLAY, shuffles deck, called only when a round ends after score calculation
    void checkScores(); //end the game if a player has more than 80 points, determines winner(s)

private:
    Deck* deck_;
    
    int curPlayer_; //game state on which player is currently playing
    bool startRound_; //TODO: use an enum here
    static const int WIN_POINTS = 80;
    GameState gameState_;
    std::vector<Player*> players_;      
    std::map< Suit, std::set<Rank> > playedCards_;

    void deal();
    void nextPlayer(); //changes the current active player
};

#endif