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

using namespace std;

// A blackjack playing card
// No base class
class Card
{
public:
    enum rank {ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
               JACK, QUEEN, KING};           
    enum suit {CLUBS, DIAMONDS, HEARTS, SPADES};

    // Overloading << operator so it can send the Card object to standard output 
    friend ostream& operator<<(ostream& os, const Card& aCard);
    
    Card(rank r = ACE, suit s = SPADES, bool ifu = true); 

    // Returns the value of a card, 1 - 11
    int GetValue() const;

    // Flips a card; if face up, becomes face down and vice versa
    void Flip();
    
private:
    // Rank of the card (ace, 2, 3, etc.)
    // rank is an enumeration for all 13 ranks
    rank m_Rank;

    // Suit of the card (clubs, diamonds, hearts, or spades)
    // suit is an enumeration for the four possible suits
    suit m_Suit;

    // Indicates whether the card is face up.
    // Affects how the card is displayed and the value it has.
    bool m_IsFaceUp;
};

Card::Card(rank r, suit s, bool ifu):  m_Rank(r), m_Suit(s), m_IsFaceUp(ifu)
{} 

int Card::GetValue() const
{
    // If a card is face down, its value is 0
    int value = 0;
    if (m_IsFaceUp)
    {
        // Value is the number showing on card
        value = m_Rank;
        // Value is 10 for face cards
        if (value > 10)
	{
            value = 10;
	}
    }
    return value;
}

void Card::Flip()
{
    m_IsFaceUp = !(m_IsFaceUp);
}

// A blackjack hand. A collection of Card objects
// No base class
class Hand
{
public:
    Hand();
    
    virtual ~Hand();

    // Adds a card to the hand
    // Adds a pointer to Card to the vector m_Cards.
    void Add(Card* pCard);
 
    // Clears hand of all cards
    // Removes all pointers in the vector m_Cards, deleting all associated
    // Card objects on the heap.
    void Clear();
 
    // Gets hand total value, intelligently treats aces as 1 or 11
    int GetTotal() const;

protected:
    // A collection of cards. Stores pointers to Card objects
    vector<Card*> m_Cards;
};

Hand::Hand()
{
    m_Cards.reserve(7);
}

Hand::~Hand()  
{
    Clear();
}

void Hand::Add(Card* pCard)
{
    m_Cards.push_back(pCard);
}
 
void Hand::Clear()
{
    // Iterate through vector, freeing all memory on the heap
    vector<Card*>::iterator iter = m_Cards.begin();
    for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
    {
        delete *iter;
        *iter = 0;
    }
    // Clear vector of pointers
    m_Cards.clear();
}

int Hand::GetTotal() const
{
    // If no cards in hand, return 0
    if (m_Cards.empty())
    {
        return 0;
    }
  
    // If a first card has value of 0, then card is face down; return 0
    if (m_Cards[0]->GetValue() == 0)
    {
        return 0;
    }
    
    // Add up card values, treat each Ace as 1
    int total = 0;
    vector<Card*>::const_iterator iter;
    for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
    {
        total += (*iter)->GetValue();
    }
                  
    // Determine if hand contains an Ace
    bool containsAce = false;
    for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
    {
        if ((*iter)->GetValue() == Card::ACE)
	{
            containsAce = true;
	}
    }
          
    // If hand contains Ace and total is low enough, treat Ace as 11
    if (containsAce && total <= 11)
    {
        // Add only 10 since we've already added 1 for the Ace
        total += 10;   
    }
            
    return total;
}

// A generic blackjack player.
// Not a full player but the common elements of a human player
// and the computer player.
// Base class is Hand.
class GenericPlayer : public Hand
{
    friend ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer);

public:
    GenericPlayer(const string& name = "");
    
    virtual ~GenericPlayer();

    // Indicates whether or not generic player wants to keep hitting
    // Pure virtual function.
    virtual bool IsHitting() const = 0;

    // Returns whether generic player has busted - has a total greater than 21
    bool IsBusted() const;

    // Announces that the generic player busts
    void Bust() const;

protected:
    // Generic player's name
    string m_Name;
};

GenericPlayer::GenericPlayer(const string& name): 
    m_Name(name)
{}

GenericPlayer::~GenericPlayer()  
{}

bool GenericPlayer::IsBusted() const
{ 
    return (GetTotal() > 21);
}

void GenericPlayer::Bust() const
{
    cout << m_Name << " busts.\n";
}

// Human blackjack player.
// Base class is GenericPlayer
class Player : public GenericPlayer
{
public:
    Player(const string& name = "");

    virtual ~Player();

    // Returns whether or not the player wants another hit       
    virtual bool IsHitting() const;

    // Announces that the player wins
    void Win() const;

    // Announces that the player loses
    void Lose() const;

    // Announces that the player pushes
    void Push() const;
};

Player::Player(const string& name): 
    GenericPlayer(name)
{}

Player::~Player()
{}
    
bool Player::IsHitting() const
{
    cout << m_Name << ", do you want a hit? (Y/N): ";
    char response;
    cin >> response;
    return (response == 'y' || response == 'Y');
}

void Player::Win() const
{
    cout << m_Name <<  " wins.\n";
}

void Player::Lose() const
{
    cout << m_Name <<  " loses.\n";
}

void Player::Push() const
{
    cout << m_Name <<  " pushes.\n";
}

// The computer player (the house)
// Base class is GenericPlayer
class House : public GenericPlayer
{
public:
    House(const string& name = "House");

    virtual ~House();

    // Indicates whether house is hitting - will always hit on 16 or less
    virtual bool IsHitting() const;

    // Flips over first card
    void FlipFirstCard();
};

House::House(const string& name): 
    GenericPlayer(name)
{}

House::~House()
{}

bool House::IsHitting() const
{
    return (GetTotal() <= 16);
}   

void House::FlipFirstCard()
{
    if (!(m_Cards.empty()))
    {
        m_Cards[0]->Flip();
    }
    else
    {
	cout << "No card to flip!\n";
    }
}

// A blackjack deck.
// Has extra functionality that Hand does not, such as shuffling and dealing.
// Base class is Hand
class Deck : public Hand
{
public:
    Deck();
    
    virtual ~Deck();

    // Create a standard deck of 52 cards
    void Populate();

    // Shuffle cards
    void Shuffle();

    // Deal one card to a hand
    void Deal(Hand& aHand);

    // Give additional cards to a generic player 
    void AdditionalCards(GenericPlayer& aGenericPlayer);
};

Deck::Deck()
{ 
    m_Cards.reserve(52);
    Populate();
}

Deck::~Deck()
{}

void Deck::Populate()
{
    Clear();
    // Create standard deck
    for (int s = Card::CLUBS; s <= Card::SPADES; ++s)
    {
        for (int r = Card::ACE; r <= Card::KING; ++r)
        {
            Add(new Card(static_cast<Card::rank>(r),
                         static_cast<Card::suit>(s)));
        }
    }
}

void Deck::Shuffle()
{
    random_shuffle(m_Cards.begin(), m_Cards.end());
}

void Deck::Deal(Hand& aHand)
{
    if (!m_Cards.empty())
    {
        aHand.Add(m_Cards.back());
        m_Cards.pop_back();
    }
    else
    {
        cout << "Out of cards. Unable to deal.";
    }
}

void Deck::AdditionalCards(GenericPlayer& aGenericPlayer)
{
    cout << endl;
    // Continue to deal a card as long as generic player isn't busted and
    // wants another hit
    while ( !(aGenericPlayer.IsBusted()) && aGenericPlayer.IsHitting() )
    {
        Deal(aGenericPlayer);
        cout << aGenericPlayer << endl;
        
        if (aGenericPlayer.IsBusted())
	{
            aGenericPlayer.Bust();
	}
    }
} 

// A blackjack game
// No base class
class Game
{
public:
    Game(const vector<string>& names);
    
    ~Game();
    
    // Plays the game of blackjack    
    void Play();

private:
    Deck m_Deck;
    House m_House;
    vector<Player> m_Players;  
};

Game::Game(const vector<string>& names)
{
    // Create a vector of players from a vector of names       
    vector<string>::const_iterator pName;
    for (pName = names.begin(); pName != names.end(); ++pName)
    {
        m_Players.push_back(Player(*pName));
    }

   // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));    
    m_Deck.Populate();
    m_Deck.Shuffle();
}

Game::~Game()
{}

void Game::Play()
{         
    // Deal initial 2 cards to everyone
    vector<Player>::iterator pPlayer;
    for (int i = 0; i < 2; ++i)
    {
	 for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer) 
	 {
	      m_Deck.Deal(*pPlayer);
	 }
         m_Deck.Deal(m_House);
    }
    
    // Hide house's first card
    m_House.FlipFirstCard();    
    
    // Display everyone's hand
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer) 
    {
        cout << *pPlayer << endl;
    }
    cout << m_House << endl;

    // Deal additional cards to players
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
    {
        m_Deck.AdditionalCards(*pPlayer);    
    }

    // Reveal house's first card
    m_House.FlipFirstCard();    
    cout << endl << m_House; 
  
    // Deal additional cards to house
    m_Deck.AdditionalCards(m_House);

    if (m_House.IsBusted())
    {
        // Everyone still playing wins
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer) 
	{
            if ( !(pPlayer->IsBusted()) )
	    {
                pPlayer->Win();
	    }
	}
    }
    else
    {
         // Compare each player still playing to house
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end();
             ++pPlayer)      
        {
            if ( !(pPlayer->IsBusted()) )
            {
                if (pPlayer->GetTotal() > m_House.GetTotal())
                {
                    pPlayer->Win();
                }
                else if (pPlayer->GetTotal() < m_House.GetTotal())
                {
                    pPlayer->Lose();
                }
                else
                {
                    pPlayer->Push();
                }
            }
        }

    }

    // Remove everyone's cards
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer) 
    {
        pPlayer->Clear();
    }
    m_House.Clear();
}

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
