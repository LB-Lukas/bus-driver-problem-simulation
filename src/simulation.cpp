#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>

/**
 * @brief Type alias for a card in the deck
 */
using Card = std::pair<std::string, int>;


/**
 * @brief Initialize deck of cards from 7 to Ace and return it
 * 
 * The deck is represented as a vector of pairs, where the first element of the pair is the suit of the card
 * and the second element is the rank of the card.
 * Suit and rank are represented as strings and integers respectively.
 * 
 * @return std::vector<Card> Vector of pairs representing the deck of cards
 */
std::vector<Card> get_deck() {
    std::vector<Card> deck;
    std::vector<std::string> suits = {"Clubs", "Diamonds", "Hearts", "Spades"};
    
    for (const std::string& suit : suits) {
        for (int rank = 7; rank <= 14; ++rank) {
            deck.push_back(make_pair(suit, rank));
        }
    }
    return deck;
}


/**
 * @brief Draw a card from the deck and return it
 * 
 * @param deck A reference to the deck of cards 
 * @return Card 
 */
Card draw_card(std::vector<Card>& deck) {
    Card card = deck.back();
    deck.pop_back();
    return card;
}


/**
 * @brief Simulate a round of the bus driver game
 * 
 * @return int The number of drinks the player has to take
 */
int simulate_busdriver_game() {
    std::vector<Card> deck = get_deck();
    std::vector<Card> pile;
    
    // Shuffle the deck
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
    
    int num_drinks = 0;

    // Round 1: Red or Black?
    while (!deck.empty()) {
        Card card = draw_card(deck);
        pile.push_back(card);
        std::string player_guess = (rand() % 2 == 0) ? "Red" : "Black";
        std::string color_of_card = (card.first == "Hearts" || card.first == "Diamonds") ? "Red" : "Black";
        
        if (player_guess == color_of_card) {
            break;
        } else {
            num_drinks++;
        }
    }

    // Round 2: Which suit?
    while (!deck.empty()) {
        Card card = draw_card(deck);
        pile.push_back(card);
        std::vector<std::string> suits = {"Clubs", "Diamonds", "Hearts", "Spades"};
        std::string player_guess = suits[rand() % 4];
        std::string suit_of_card = card.first;
        
        if (player_guess == suit_of_card) {
            break;
        } else {
            num_drinks++;
        }
    }

    // Round 3: Higher, Lower or Same?
    while (!deck.empty()) {
        Card card = draw_card(deck);
        pile.push_back(card);
        std::string player_guess;
        int random_value = rand() % 3;
        player_guess = (random_value == 0) ? ">" : (random_value == 1) ? "<" : "="; 
        
        if ((card.second > pile[pile.size() - 2].second && player_guess == ">") ||
            (card.second < pile[pile.size() - 2].second && player_guess == "<") ||
            (card.second == pile[pile.size() - 2].second && player_guess == "=")) {
            break;
        } else {
            num_drinks++;
        }
    }

    // Round 4: Inside or Outside?
    while (!deck.empty()) {
        Card card1 = pile[pile.size() - 2];
        Card card2 = pile[pile.size() - 1];
        int lower_bound = std::min(card1.second, card2.second);
        int upper_bound = std::max(card1.second, card2.second);
        Card card = draw_card(deck);
        pile.push_back(card);
        
        std::string player_guess = (rand() % 2 == 0) ? "Inside" : "Outside";
        
        if ((card.second > lower_bound && card.second < upper_bound && player_guess == "Inside") ||
            ((card.second < lower_bound || card.second > upper_bound) && player_guess == "Outside")) {
            break;
        } else {
            num_drinks++;
        }
    }

    return num_drinks;
}


/**
 * @brief Main function to run the simulation
 * 
 * @return int 
 */
int main() {
    int num_games = 1000000;
    std::vector<int> results;

    std::ofstream outfile("results.txt");

    int progress_interval = num_games / 100;  // Update progress every 1%
    
    for (int i = 0; i < num_games; ++i) {
        results.push_back(simulate_busdriver_game());
        outfile << results.back() << std::endl;  // Save the result to the file

        // Display progress
        if (i % progress_interval == 0) {
            int progress = (i * 100) / num_games;  // Calculate percentage
            std::cout << "\rProgress: " << progress << "% completed" << std::flush;
        }
    }

    std::cout << "\rProgress: 100% completed" << std::endl;  // Final update

    outfile.close();
    std::cout << "Simulation complete. Results written to results.txt." << std::endl;

    return 0;
}
