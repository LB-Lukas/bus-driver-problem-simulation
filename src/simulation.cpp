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
 * 
 * @return std::vector<Card> Vector of pairs representing the deck of cards
 */
std::vector<Card> get_deck() {
    std::vector<Card> deck;
    deck.reserve(32); // Pre-allocate space for 32 cards
    const std::vector<std::string> suits = {"Clubs", "Diamonds", "Hearts", "Spades"};
    
    for (const std::string& suit : suits) {
        for (int rank = 7; rank <= 14; ++rank) {
            deck.emplace_back(suit, rank);
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
 * @param deck Pre-shuffled deck passed by reference
 * @param rng Random number generator
 * @return int The number of drinks the player has to take
 */
int simulate_busdriver_game(std::vector<Card>& deck, std::mt19937& rng) {
    std::vector<Card> pile;
    pile.reserve(4);  // Pre-allocate space for the pile
    
    int num_drinks = 0;
    int round_num = 1;

    while (!deck.empty()) {
        if (round_num == 1) {
            Card card = draw_card(deck);
            pile.push_back(card);
            const std::vector<std::string> suits = {"Clubs", "Diamonds", "Hearts", "Spades"};
            std::string player_guess = suits[rng() % 4];
            std::string suit_of_card = card.first;

            if (player_guess == suit_of_card) {
                round_num++;
            } else {
                num_drinks++;
                round_num = 1;
            }

        } else if (round_num == 2) {
            Card card = draw_card(deck);
            pile.push_back(card);
            std::string player_guess = (rng() % 2 == 0) ? "Red" : "Black";
            std::string color_of_card = (card.first == "Hearts" || card.first == "Diamonds") ? "Red" : "Black";

            if (player_guess == color_of_card) {
                round_num++;
            } else {
                num_drinks++;
                round_num = 1;
            }
        } else if (round_num == 3) {
            if (pile.size() < 2) {
                return num_drinks;  // Safety check in case there are not enough cards for comparison
            }
            Card card = draw_card(deck);
            pile.push_back(card);
            std::string player_guess;
            int random_value = rng() % 3;
            player_guess = (random_value == 0) ? ">" : (random_value == 1) ? "<" : "="; 
            
            if ((card.second > pile[pile.size() - 2].second && player_guess == ">") ||
                (card.second < pile[pile.size() - 2].second && player_guess == "<") ||
                (card.second == pile[pile.size() - 2].second && player_guess == "=")) {
                round_num++;
            } else {
                num_drinks++;
                round_num = 1;
            }
        } else if (round_num == 4) {
            if (pile.size() < 3) {
                return num_drinks;  // Safety check in case there are not enough cards for comparison
            }
            Card card1 = pile[pile.size() - 3];
            Card card2 = pile[pile.size() - 2];
            int lower_bound = std::min(card1.second, card2.second);
            int upper_bound = std::max(card1.second, card2.second);
            Card card = draw_card(deck);
            pile.push_back(card);
            
            std::string player_guess = (rng() % 2 == 0) ? "Inside" : "Outside";
            
            if ((card.second > lower_bound && card.second < upper_bound && player_guess == "Inside") ||
                ((card.second < lower_bound || card.second > upper_bound) && player_guess == "Outside")) {
                return num_drinks;
            } else {
                num_drinks++;
                round_num = 1;
            }
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
    const int num_games = 1'000'000;
    std::vector<int> results;
    results.reserve(num_games);  // Pre-allocate space for results
    
    std::ofstream outfile("results.txt");
    if (!outfile) {
        std::cerr << "Failed to open results file!" << std::endl;
        return 1;
    }

    std::vector<Card> deck = get_deck();  // Create deck once
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());  // Mersenne Twister RNG
    
    const int progress_interval = num_games / 100;  // Update progress every 1%
    
    for (int i = 0; i < num_games; ++i) {
        deck = get_deck();  // Reset deck before each game
        std::shuffle(deck.begin(), deck.end(), rng);  // Shuffle the deck before each game
        results.push_back(simulate_busdriver_game(deck, rng));  // Store result in memory
        
        // Display progress
        if (i % progress_interval == 0) {
            std::cout << "\rProgress: " << (i * 100) / num_games << "% completed" << std::flush;
        }
    }

    std::cout << "\rProgress: 100% completed" << std::endl;  // Final update
    
    // Write all results to file at once
    for (const auto& result : results) {
        outfile << result << std::endl;
    }
    
    outfile.close();
    std::cout << "Simulation complete. Results written to results.txt." << std::endl;

    return 0;
}
