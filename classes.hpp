/**
 * class definitions using French cards
 */

#ifndef CLASSES_HPP
    #define CLASSESS_HPP
#endif

#include <algorithm>
#include <array>
#include <random>
#include <chrono>

///////////////////
///// OBJECTS /////
///////////////////

namespace rando
{
    // random seed source
    std::random_device rd;
    // pseudo-random sequence generator
    std::mt19937_64 rand_gen (rd());
}

/**
 * list of possible suits
 */
enum cardSuit
{
    clubs,
    diamonds,
    hearts,
    spades
};

std::vector<cardSuit> suitvector =
{
    clubs,
    diamonds,
    hearts,
    spades
};

/**
 * list of possible suit colors
 */
enum suitColors
{
    black,
    red
};

std::vector<suitColors> colorvector =
{
    black,
    red
};

/**
 * list of possible card values
 */
enum cardValue
{
    Ace,
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    Jack,
    Queen,
    King
};

std::vector<cardValue> valuevector =
{
    Ace,
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    Jack,
    Queen,
    King
};

/**
 * defines the individual playing cards --- mostly used within a Deck object
 */
class Card
{
    //! the suit of the card
    cardSuit internal_suit;
    //! the point value of the card
    cardValue internal_value;
  public:
    //! constructor
    Card(cardValue valval, cardSuit suitval) {internal_value = valval; internal_suit = suitval;}
    //! return the color of the card
    suitColors color() const {return ((internal_suit == diamonds || internal_suit == hearts) ? red : black);}
    //! return the value of the card
    cardValue value() const {return internal_value;}
    //! return the suit of the card
    cardSuit suit() const {return internal_suit;}
};

/**
 * defines a deck of cards
 */
class Deck
{
    //! the number of cards in a deck
    const unsigned short int number_of_cards = 52;
    //! the array of cards
    std::vector<Card> deck_cards;
    //! the number of cards remaining in the deck
    unsigned short int cards_left;
  public:
    //! constructor
    Deck();
    //! draw one card
    Card draw();
    //! shuffle the deck (randomize the order of Card objects in the array)
    void shuffle();
    //! return the number of cards left in the deck
    unsigned short int cardsRemaining() const {return cards_left;}
    //! cheat by looking at the next card
    Card cheat_peek() const;
    //! cheat by looking at the remainder of the deck
    std::vector<Card> cheat_showall() const;
};

/**
 * Defines a stack consisting of multiple decks' worth of cards.
 * A dealer's shoe is a device that holds up to eight decks shuffled together.
 */
class Shoe
{
    //! the number of 52-card decks in the shoe
    unsigned short int number_of_decks;
    //! the array of decks
    std::vector<Deck> shoe_decks;
    //! the array of cards acquired from the constituent decks
    std::vector<Card> shoe_cards;
    //! the number of cards remaining in the shoe
    unsigned short int cards_left;
  public:
    //! multi-deck constructor
    Shoe(unsigned short int);
    //! draw one card
    Card draw();
    //! shuffle the shoe (randomize the order of Card objects in the shoe)
    void shuffle();
    //! return the number of cards left in the deck
    unsigned short int cardsRemaining() const {return cards_left;}
    //! cheat by looking at the next card
    Card cheat_peek() const;
    //! cheat by looking at the remainder of the deck
    std::vector<Card> cheat_showall() const;
};

///////////////////////////////////////
///// MEMBER FUNCTION DEFINITIONS /////
///////////////////////////////////////

///// Deck object member functions

/**
 * Deck constructor --- build the deck
 */
Deck::Deck()
{

    // allocate the memory
    deck_cards.reserve(number_of_cards);

    // populate it with the standard 52 cards
    // iterate over values
    for (cardValue v : valuevector)
    {
        // iterate over suits
        for (cardSuit s : suitvector)
        {
            deck_cards.push_back(Card(v,s));
        }
    }

    cards_left = number_of_cards;

    // shuffle it
    shuffle();
}

/**
 * shuffling the deck
 */
void Deck::shuffle()
{
    // shuffle the cards
    std::shuffle(deck_cards.begin(), deck_cards.end(), rando::rand_gen);
    // reset the deck's card count
    cards_left = number_of_cards;
}

/**
 * draw a card from the deck
 */
Card Deck::draw()
{
    // if there are no more cards left, reshuffle the deck and reset the card count
    if (cards_left == 0)
    {
        shuffle();
    }
    // decrement the number of cards and return the next card
    // pre-decrement because the number is 1-indexed and the array is 0-indexed
    return deck_cards[--cards_left];
}

/**
 * cheat by looking at the next card
 */
Card Deck::cheat_peek() const
{
    // if there are no more cards left, just return the bottom card
    if (cards_left == 0)
    {
        return deck_cards[cards_left];
    }
    // return the next card without counting it as drawn
    // decrement because the number is 1-indexed and the array is 0-indexed
    return deck_cards[cards_left-1];
}

/**
 * cheat by looking at the remainder of the deck
 */
std::vector<Card> Deck::cheat_showall() const
{
    // construct a vector containing the cards from the bottom card to the current top card
    std::vector<Card> undealt;
    for (int i = 0; i < cards_left; i++)
    {
        undealt.push_back(deck_cards[i]);
    }

    return undealt;
}

///// Shoe object member functions

/**
 * multi-deck constructor
 */
Shoe::Shoe(unsigned short int ndecks)
{
    number_of_decks = ndecks;
    cards_left = 52*ndecks;
    // acquire the decks
    shoe_decks.reserve(ndecks);
    for (int i = 0; i < ndecks; i++) shoe_decks.push_back(Deck());
    // transfer the cards from the decks
    shoe_cards.reserve(52*number_of_decks);
    for (int i = 0; i < number_of_decks; i++)
    {
        for (int j = 0; j < 52; j++)
        {
            shoe_cards[j+i*52] = shoe_decks[i].draw();
        }
    }
    this->shuffle();
}

//! draw one card
Card Shoe::draw()
{
    // if there are no more cards left, reshuffle the deck and reset the card count
    if (cards_left == 0)
    {
        shuffle();
    }
    // decrement the number of cards and return the next card
    // pre-decrement because the number is 1-indexed and the array is 0-indexed
    return shoe_cards[--cards_left];
}
//! shuffle the shoe (randomize the order of Card objects in the shoe)
void Shoe::shuffle()
{
    // shuffle the cards
    std::shuffle(shoe_cards.begin(), shoe_cards.end(), rando::rand_gen);
    // reset the deck's card count
    cards_left = 52*number_of_decks;
}

/**
 * cheat by looking at the next card
 */
Card Shoe::cheat_peek() const
{
    // if there are no more cards left, just return the bottom card
    if (cards_left == 0)
    {
        return shoe_cards[cards_left];
    }
    // return the next card without counting it as drawn
    // decrement because the number is 1-indexed and the array is 0-indexed
    return shoe_cards[cards_left-1];
}

/**
 * cheat by looking at the remainder of the deck
 */
std::vector<Card> Shoe::cheat_showall() const
{
    // construct a vector containing the cards from the bottom card to the current top card
    //leave the 
    std::vector<Card> undealt;
    for (int i = 0; i < cards_left; i++)
    {
        undealt.push_back(shoe_cards[i]);
    }

    return undealt;
}
