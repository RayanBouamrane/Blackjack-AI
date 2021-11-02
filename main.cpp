
#include "blackjack.h"

using namespace std;

int main() {
    cout << "Welcome to the Comp322 Blackjack game!" << endl << endl;
    BlackJackGame game;
    // The main loop of the game
    bool playAgain = true;
    char answer = 'y';
    while (playAgain) {
        game.play();
        // Check whether the player would like to play another round
        cout << "Would you like another round? (y/n): ";
        cin >> answer;
        cout << endl;
        playAgain = answer == 'y';
    }
    cout << "Game over!";
    return 0;

}
