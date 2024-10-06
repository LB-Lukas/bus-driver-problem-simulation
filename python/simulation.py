import random
import time
import sys

# Initialize deck of cards from 7 to Ace and return it
def get_deck():
    deck = []
    suits = ["Clubs", "Diamonds", "Hearts", "Spades"]
    
    for suit in suits:
        for rank in range(7, 15):
            deck.append((suit, rank))
    
    return deck

# Draw a card from the deck and return it
def draw_card(deck):
    return deck.pop()

# Simulate a round of the bus driver game
def simulate_busdriver_game():
    deck = get_deck()
    pile = []
    
    # Shuffle the deck
    random.shuffle(deck)
    
    num_drinks = 0

    # Round 1: Red or Black?
    while deck:
        card = draw_card(deck)
        pile.append(card)
        player_guess = random.choice(["Red", "Black"])
        color_of_card = "Red" if card[0] in ["Hearts", "Diamonds"] else "Black"
        
        if player_guess == color_of_card:
            break
        else:
            num_drinks += 1

    # Round 2: Which suit?
    while deck:
        card = draw_card(deck)
        pile.append(card)
        player_guess = random.choice(["Clubs", "Diamonds", "Hearts", "Spades"])
        suit_of_card = card[0]
        
        if player_guess == suit_of_card:
            break
        else:
            num_drinks += 1

    # Round 3: Higher, Lower or Same?
    while deck:
        card = draw_card(deck)
        pile.append(card)
        player_guess = random.choice([">", "<", "="])
        
        if (card[1] > pile[-2][1] and player_guess == ">") or \
           (card[1] < pile[-2][1] and player_guess == "<") or \
           (card[1] == pile[-2][1] and player_guess == "="):
            break
        else:
            num_drinks += 1

    # Round 4: Inside or Outside?
    while deck:
        card1 = pile[-2]
        card2 = pile[-1]
        lower_bound = min(card1[1], card2[1])
        upper_bound = max(card1[1], card2[1])
        card = draw_card(deck)
        pile.append(card)
        
        player_guess = random.choice(["Inside", "Outside"])
        
        if (lower_bound < card[1] < upper_bound and player_guess == "Inside") or \
           ((card[1] < lower_bound or card[1] > upper_bound) and player_guess == "Outside"):
            break
        else:
            num_drinks += 1

    return num_drinks

# Main function to simulate the bus driver game multiple times
if __name__ == "__main__":
    num_games = 1_000_000
    results = []
    
    with open("results.txt", "w") as outfile:
        progress_interval = num_games // 100  # Update progress every 1%
        
        for i in range(num_games):
            results.append(simulate_busdriver_game())
            outfile.write(f"{results[-1]}\n")  # Save the result to the file

            # Display progress
            if i % progress_interval == 0:
                progress = (i * 100) // num_games  # Calculate percentage
                sys.stdout.write(f"\rProgress: {progress}% completed")
                sys.stdout.flush()

    sys.stdout.write("\rProgress: 100% completed\n")  # Final update
    print("Simulation complete. Results written to results.txt.")
