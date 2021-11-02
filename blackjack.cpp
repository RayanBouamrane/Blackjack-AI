#include <algorithm>
#include <random>
#include <chrono>
#include "blackjack.h"

using namespace std;

Card::Card(Rank pRank, Suit pSuit) {
    aRank = pRank;
    aSuit = pSuit;
    //Extra private fields storing chars introduced for simplicity when displaying Cards

    if (aRank == ACE) {         //Value of Ace stored as 11, if 11 causes hand value to
        aValue = 11;            //exceed 21, then 1 will be added to hand value instead, in getTotal
        aRankDisplay = '1';
    } else if (aRank < 9) {     // Twos up to Nine can be displayed simply as their ascii value + 48
        aValue = aRank + 1;
        aRankDisplay = aValue + 48;
    } else {                    // Special cases
        aValue = 10;
        switch (aRank) {
            case TEN :
                aRankDisplay = 'T';
                break;
            case JACK :
                aRankDisplay = 'J';
                break;
            case QUEEN :
                aRankDisplay = 'Q';
                break;
            case KING :
                aRankDisplay = 'K';
                break;
        }
    }

    switch (aSuit) {           // Suit display
        case Suit::CLUBS :
            aSuitDisplay = 'C';
            break;
        case Suit::DIAMONDS :
            aSuitDisplay = 'D';
            break;
        case Suit::HEARTS :
            aSuitDisplay = 'H';
            break;
        case Suit::SPADES :
            aSuitDisplay = 'S';
            break;
    }
}

int Card::getValue() { return aValue; }

void Card::displayCard() { cout << aRankDisplay << aSuitDisplay; }

Rank Card::getRank() { return aRank; }

void Hand::clear() { aCards.clear(); }

int Hand::getTotal() {
    int total = 0;
    int numAces = 0;
    for (Card c : aCards) {         //add all non Ace values to total, those values are constant
        if (c.getRank() != ACE)
            total += c.getValue();
        if (c.getRank() == ACE) { numAces++; }
    }
    while (numAces > 0) {           //
        if (total + 10 + numAces <= 21) {     // if counting Ace as 11 and adding every other Ace as 1
            total += 11;                      // doesn't bust, do so, else count as 1
        } else {
            total++;
        }
        numAces--;
    }
    return total;
}

void Hand::add(Card pCard) { aCards.push_back(pCard); }

void Hand::displayHand() {
    for (Card card: aCards) {
        card.displayCard();
        cout << " ";
    }
    cout << "[" << getTotal() << "]\n";
}

void Deck::add(Card pCard) { aCards.push_back(pCard); }

void Deck::populate() {                 //
    for (int r = 0; r < 13; r++)
        for (int s = 0; s < 4; s++)
            aCards.emplace_back(Rank(r), Suit(s));
}

void Deck::shuffle() {
    long seed = chrono::duration_cast<chrono::nanoseconds>(chrono::time_point_cast<chrono::nanoseconds>
            (chrono::high_resolution_clock::now()).time_since_epoch()).count();
    std::shuffle(begin(aCards), end(aCards), default_random_engine(seed));
    //this gets the time since epoch in nano seconds, uses it as a seed for a random engine, shuffles accordingly
}

Card Deck::deal() {             //remove top of deck and return it
    Card c = aCards.back();
    aCards.pop_back();
    return c;
}

bool AbstractPlayer::isBusted() { return (aHand.getTotal() > 21); }

bool HumanPlayer::isDrawing() {
    char input;
    cout << "Do you want to draw? (y/n): ";
    cin >> input;
    return input == 'y';
}

void HumanPlayer::announce(int houseTotal) {

    if (houseTotal > 21 || (!isBusted() && aHand.getTotal() > houseTotal)) //player wins either if house busted
        cout << "Player Wins." << endl;             //and they haven't, or if they have have a bigger hand (without busting)
    else if (isBusted() || aHand.getTotal() < houseTotal)   //house wins if player busts or if total exceeds player
        cout << "House Wins." << endl;                  //note house busting checked earlier
    else if (aHand.getTotal() == houseTotal)            //push if equal
        cout << "Push: Draw." << endl;

}

bool ComputerPlayer::isDrawing() { return aHand.getTotal() <= 16; }

void BlackJackGame::play() {

    Deck Deck;
    Deck.populate();
    Deck.shuffle();

    HumanPlayer Player;
    Player.aHand.add(Deck.deal());          //Player draws two cards, displays
    Player.aHand.add(Deck.deal());

    cout << "Player: ";
    Player.aHand.displayHand();

    ComputerPlayer House;
    House.aHand.add(Deck.deal());           //House draws one card, displays

    cout << "Casino: ";
    House.aHand.displayHand();

    bool playerHasDrawn = true;             //this stores whether the player decided to draw or stop
                                            //initially true to give player choice to draw
    while (!Player.isBusted() && playerHasDrawn) {  //player stops drawing by choice or by busting
        playerHasDrawn = Player.isDrawing();
        if (playerHasDrawn) {
            Player.aHand.add(Deck.deal());
        }
        cout << "Player: ";
        Player.aHand.displayHand();
    }

    if (Player.isBusted()) cout << "Player Busted.\n";

    while (House.isDrawing() && !Player.isBusted()) { //if player busted, house doesnt draw
        House.aHand.add(Deck.deal());

        cout << "Casino: ";
        House.aHand.displayHand();
    }

    if (House.isBusted()) cout << "House Busted.\n";

    Player.announce(House.aHand.getTotal());        //announce results


}