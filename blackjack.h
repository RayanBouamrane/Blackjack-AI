#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

enum Rank {
    ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN,
    EIGHT, NINE, TEN, JACK, QUEEN, KING
};

enum Suit {
    CLUBS, DIAMONDS, HEARTS, SPADES
};

class Card {

    Rank aRank;
    Suit aSuit;
    int aValue;
    char aRankDisplay;          //for convenience when displaying Cards
    char aSuitDisplay;

public:
    int getValue();
    void displayCard();
    Card(Rank pRank, Suit pSuit);
    Rank getRank();

};

class Hand {
    vector<Card> aCards;
public :
    void add(Card pCard);
    void clear();
    int getTotal();
    void displayHand();
};

class Deck {
    vector<Card> aCards;
    void add(Card pCard);
public :
    void populate();
    void shuffle();
    Card deal();
};

class AbstractPlayer
{
public:
    Hand aHand;
    virtual bool isDrawing() = 0;
    bool isBusted();
};

class HumanPlayer : public AbstractPlayer
{
public:
    bool isDrawing() override;
    void announce(int houseTotal);      //pass through the house total as param to compare to Player's hand total
};

class ComputerPlayer : public AbstractPlayer
{
public:
    bool isDrawing() override;
};

class BlackJackGame
{
    Deck m_deck;
    ComputerPlayer m_casino;
    HumanPlayer m_player;
public:
    void play();
};

#endif