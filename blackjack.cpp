/**
 * Blackjack Monte Carlo simulator
 * Hayden Cornwell
 * Started 19-October-2018
 */

///// LIBRARIES & HEADERS /////

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cmath>

# include "classes.hpp"

///// PROTOTYPES & OPERATORS /////

std::ostream& operator<<(std::ostream&, const Card&);
unsigned short int valueOf(const std::vector<Card>&);

//////////////////
///// main() /////
//////////////////

int main()
{
    unsigned short int number_of_decks;
    unsigned int iterations;
    unsigned short int who_won = 2; // 2 == dealer wins, 1 == push, 0 == player wins
    unsigned long long int dealer_wins = 0;
    unsigned long long int pushes = 0;
    unsigned long long int player_wins = 0;
    unsigned short int countdown = 0;
    std::vector<Card> dealerhand, playerhand;
    // get the number of decks; must be in the interval [1,8]
    do
    {
        std::cout << "With how many decks would you like to play (1-8)?" << std::endl;
        std::cin  >> number_of_decks;
    } while ((number_of_decks < 1) || (number_of_decks > 8));
    // get the number of iterations
    std::cout << "How many games would you like to play?" << std::endl;
    std::cin  >> iterations;

    std::cout << "Playing " << iterations << " rounds with " << number_of_decks
              << " deck" << (number_of_decks != 1 ? "s" : "") << "..." << std::endl;
    // fill the shoe
    Shoe shoe(number_of_decks);

    // play the rounds
    for (unsigned int i = 0; i < iterations; i++)
    {
        // clear the vectors
        dealerhand.clear();
        playerhand.clear();

        // draw two cards each; player draws first
        playerhand.push_back(shoe.draw());
        playerhand.push_back(shoe.draw());
        dealerhand.push_back(shoe.draw());
        dealerhand.push_back(shoe.draw());
    
        // play a simplified round
        while (valueOf(dealerhand) < 17 && valueOf(playerhand) < 21)
        {
            playerhand.push_back(shoe.draw());
            dealerhand.push_back(shoe.draw());
        }

        // dealer busts, player doesn't
        if (valueOf(dealerhand) > 21 && valueOf(playerhand) <= 21)
            who_won = 0;
        // player busts, dealer doesn't
        else if (valueOf(dealerhand) <= 21 && valueOf(playerhand) > 21)
            who_won = 2;
        // both bust
        else if (valueOf(dealerhand) > 21 && valueOf(playerhand) > 21)
            who_won = 1;
        // nobody busts, player high
        else if (valueOf(dealerhand) < valueOf(playerhand))
            who_won = 0;
        // nobody busts, push
        else if (valueOf(dealerhand) == valueOf(playerhand))
            who_won = 1;
        // dealer wins otherwise
        else
            who_won = 2;

        // tally victories
        switch (who_won)
        {
            case 0:
                player_wins++;
                break;
            case 1:
                pushes++;
                break;
            default:
                dealer_wins++;
                break;
        }

        if (countdown)
        {
            // output the result of the first few hands
            std::cout << "Dealer: ";
            for (Card n : dealerhand) std::cout << n << ", ";
            std::cout << valueOf(dealerhand) << " points." << std::endl;
    
            std::cout << "Player: ";
            for (Card n : playerhand) std::cout << n << ", ";
            std::cout << valueOf(playerhand) << " points." << std::endl;
    
            std::cout << (who_won == 0 ? "Player wins." : (who_won == 1 ? "Push." : "Dealer wins.")) << std::endl;

            countdown--;
        }

        // output the win percentages about every 10% to check convergence
        if (i % (iterations/10) == 0 && i != 0)
        {
            std::cout << dealer_wins/static_cast<double>(i) << ", "
                      << pushes/static_cast<double>(i)      << ", "
                      << player_wins/static_cast<double>(i) << std::endl;
        }
    }

    std::cout << "(dealer | push | player): ("
              << dealer_wins << " | "
              << pushes      << " | "
              << player_wins << ")" << std::endl;

    return 0;
}

////////////////////////////////
///// ADDITIONAL FUNCTIONS /////
////////////////////////////////

/**
 * output the identity of a card via the stream insertion operator
 */
std::ostream& operator<<(std::ostream& stream, const Card& card)
{
    std::string cardsuit, cardvalue;

    switch (card.suit())
    {
        case clubs:
            cardsuit = "clubs";
            break;
        case diamonds:
            cardsuit = "diamonds";
            break;
        case hearts:
            cardsuit = "hearts";
            break;
        default:
            cardsuit = "spades";
            break;
    }

    switch (card.value())
    {
        case Ace:
            cardvalue = "Ace";
            break;
        case two:
            cardvalue = "two";
            break;
        case three:
            cardvalue = "three";
            break;
        case four:
            cardvalue = "four";
            break;
        case five:
            cardvalue = "five";
            break;
        case six:
            cardvalue = "six";
            break;
        case seven:
            cardvalue = "seven";
            break;
        case eight:
            cardvalue = "eight";
            break;
        case nine:
            cardvalue = "nine";
            break;
        case ten:
            cardvalue = "ten";
            break;
        case Jack:
            cardvalue = "Jack";
            break;
        case Queen:
            cardvalue = "Queen";
            break;
        default:
            cardvalue = "King";
            break;
    }
    stream << cardvalue << " of " << cardsuit;

    return stream;
}

/**
 * determine the point value of a sequence of cards
 */
unsigned short int valueOf(const std::vector<Card>& hand)
{
    unsigned short int points = 0;
    unsigned short int aces = 0;

    for (Card n : hand)
    {
        switch (n.value())
        {
        case two:
            points += 2;
            break;
        case three:
            points += 3;
            break;
        case four:
            points += 4;
            break;
        case five:
            points += 5;
            break;
        case six:
            points += 6;
            break;
        case seven:
            points += 7;
            break;
        case eight:
            points += 8;
            break;
        case nine:
            points += 9;
            break;
        case ten:
            points += 10;
            break;
        case Jack:
            points += 10;
            break;
        case Queen:
            points += 10;
            break;
        case King:
            points += 10;
            break;
        default:
            aces++;
            break;
        }
    }

    // Only aces have variable values, so they are counted last.
    // If there's only one ace, then contribution from aces can only be 1 or 11.
    if (aces == 1)
    {
        // Count the ace in a way that avoids busting.
        points += (points + 11 > 21) ? 1 : 11;
    }
    // If there are two or more aces, then things are a little more complicated.
    // Counting more than one ace as 11 would be an automatic bust, so the possibility is not considered.
    // Therefore, only the first ace can ever be 11. Whether or not that ace is 11 points is determined
    // by the sum of all aces such that all aces are 1 point. That first ace is 11 points iff 10 more
    // than the 1-indexed count of aces is less than or equal to the limit, 21.
    else
    {
        // add the sum of all aces to the total such that all aces are 1 point each
        points += aces;
        // if adding 10 points to the total still does not bust, add 10 points
        // i.e. allow the first ace to be 11 points
        if (points + 10 <= 21) points += 10;
    }

    return points;
}
