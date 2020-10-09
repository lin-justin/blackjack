// Blackjack
// =========
// PLayers are dealt cards with point values.
// Each player tries to reach a total of 21 without exceeding that amount.
// Numbered cards count as their face value.
// An ace counts as either 1 or 11 (whichever is best for the player), and
// any jack, queen, or king counts as 10.

// The computer is the house (casino) and it competes against one to seven players.
// At the beginning of the round, all participants (including the house) are dealt two cards.
// Players can see all of their cards, along with their total.
// However, one of the house's cards is hidden for the time being.

// Each player gets the chance to take one additional card at a time for as long as the player likes.
// If a player's total exceeds 21 (busting), the player loses.
// After all players have had the chance to take additional cards, the house reveals its hidden card.
// The house must then take additional cards as long as its total is 16 or less.
// If the houses busts, all players who have not busted win.
// Otherwise, each remaining player's total is compared to the houses's total.
// If the player's total is greater than the house's, the player wins.
// If the player's total is less than the house's, the player loses.
// If the two totals are the same, the player ties the house (pushing).

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

#include "blackjack.h"

using namespace std;

// Function prototypes
ostream& operator<<(ostream& os, const Card& aCard);
ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer);

int main()
{
    cout << "\t\tWelcome to Blackjack!\n\n";
    
    int numPlayers = 0;
    while (numPlayers < 1 || numPlayers > 7)
    {
        cout << "How many players? (1 - 7): ";
        cin >> numPlayers;
    }   

    vector<string> names;
    string name;
    for (int i = 0; i < numPlayers; ++i)
    {
        cout << "Enter player name: ";
        cin >> name;
        names.push_back(name);
    }
    cout << endl;

    // Game loop        
    Game aGame(names);
    char again = 'y';
    while (again != 'n' && again != 'N')
    {
        aGame.Play();
        cout << "\nDo you want to play again? (Y/N): ";
        cin >> again;
    } 

    return 0;
}

// Overloads << operator so Card object can be sent to cout
ostream& operator<<(ostream& os, const Card& aCard)
{
    const string RANKS[] = {"0", "A", "2", "3", "4", "5", "6", "7", "8", "9", 
                            "10", "J", "Q", "K"};
    const string SUITS[] = {"c", "d", "h", "s"};

    if (aCard.m_IsFaceUp)
    {
        os << RANKS[aCard.m_Rank] << SUITS[aCard.m_Suit];
    }
    else
    {
        os << "XX";
    }

    return os;
}

// Overloads << operator so a GenericPlayer object can be sent to cout
ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer)
{
    os << aGenericPlayer.m_Name << ":\t";
    
    vector<Card*>::const_iterator pCard;
    if (!aGenericPlayer.m_Cards.empty())
    {
        for (pCard = aGenericPlayer.m_Cards.begin(); 
             pCard != aGenericPlayer.m_Cards.end(); 
             ++pCard)
        {
            os << *(*pCard) << "\t";
        }


        if (aGenericPlayer.GetTotal() != 0)
		{
            cout << "(" << aGenericPlayer.GetTotal() << ")";
		}
    }
    else
    {
        os << "<empty>";
    }
        
    return os;
}