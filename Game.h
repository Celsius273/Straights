#ifndef _GAME_
#define _GAME_

#include "Card.h"
#include "Subject.h"
#include "Deck.h"
#include "Player.h"
#include "HumanPlayer.h"

#include <vector>

enum GameState {
    PLAYING, ENDROUND, ENDGAME
};

// Model class for MVC model
class Game : public Subject {
public:
    class InvalidException{
    public:
        InvalidException(std::string message) : message_(message) {}
        std::string message() const { return message_; }

    private:
        std::string message_;
    };

    //Game(std::vector<std::string> const&, int const&);
    Game();

    Deck* deck();
    int curPlayer() const;
    bool isCurPlayerCpu() const;
    GameState gameState() const;

    void replacePlayer();
    void invokeCpuLoop();

    void handleCard(int const&);
    int lastCardPlayedIdx() const;

    //void printWinner();

    void checkRound();
    void restartRound(); //changes gamestate from ENDROUND to PLAY, shuffles deck, called only when a round ends after score calculation
    void checkScores(); //end the game if a player has more than 80 points, determines winner(s)

    void setup(std::vector<bool> const&, int const&); //boolean vector of player types and deck seed
    void restartGame(); //restarts game
    int pointsForPlayer(int const&) const;
    int discardsForPlayer(int const&) const;
    std::vector<int> getCurPlayerHand() const;

private:
    Deck* deck_;

    int curPlayer_; //game state on which player is currently playing, -1 corresponds to start state
    
    int lastCardPlayedIdx_;

    bool printed_;
    
    static const int WIN_POINTS = 80;
    GameState gameState_;
    std::vector<Player*> players_;
    std::map< Suit, std::set<Rank> > playedCards_;

    void deal();
    void nextPlayer(); //changes the current active player
};

#endif
