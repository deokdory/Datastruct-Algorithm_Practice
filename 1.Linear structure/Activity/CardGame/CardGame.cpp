#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include <chrono>

#define CARD_TOTAL 52
#define SHUFFLE_COUNT 200

#define STARTING_CARDS 13

enum CardNum { J = 11, Q, K };

struct Card {
  int number;
  enum Pattern { SPADE = 0, DIAMOND, CLOVER, HEART } pattern;

  std::string GetPattern() const {
    switch (pattern) {
      case SPADE:
        return "♠";
      case DIAMOND:
        return "◆";
      case CLOVER:
        return "♣";
      case HEART:
        return "♥";
      default:
        return "?";
    }
  }

  std::string GetNumber() const {
    switch (number) {
      case 1:
        return "A";
      case 11:
        return "J";
      case 12:
        return "Q";
      case 13:
        return "K";
      default:
        return std::to_string(number);
    }
  }

  friend std::ostream& operator<<(std::ostream& out, const Card& card) {
    out << card.GetPattern() << card.GetNumber();
    return out;
  }
};

struct Game {
  std::array<Card, 52> deck;
  std::vector<Card> player1, player2, player3, player4;

  void InitDeck() {
    for (int i = 0; i < 13; i++) {
      deck[i] = Card{i + 1, Card::HEART};
    }
    for (int i = 0; i < 13; i++) {
      deck[i + 13] = Card{i + 1, Card::SPADE};
    }
    for (int i = 0; i < 13; i++) {
      deck[i + 26] = Card{i + 1, Card::CLOVER};
    }
    for (int i = 0; i < 13; i++) {
      deck[i + 39] = Card{i + 1, Card::DIAMOND};
    }
  }

  void DivideDeck() {
    //srand((unsigned)time(NULL));
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
    player1 = {deck.begin(), deck.begin() + 13};
    player2 = {deck.begin() + 13, deck.begin() + 26};
    player3 = {deck.begin() + 26, deck.begin() + 39};
    player4 = {deck.begin() + 39, deck.end()};
  }

  bool CompareAndRemove(std::vector<Card>& p1, std::vector<Card>& p2) {
    if (p1.back().number == p2.back().number) {
      p1.pop_back();
      p2.pop_back();
      return true;
    }
    return false;
  }

  void PlayOneRound() {
    std::cout << "player1 card: " << player1.back() << std::endl;
    std::cout << "player2 card: " << player2.back() << std::endl;
    std::cout << "player3 card: " << player3.back() << std::endl;
    std::cout << "player4 card: " << player4.back() << std::endl;

    if (CompareAndRemove(player1, player2)) {
      CompareAndRemove(player3, player4);
      return;
    } else if (CompareAndRemove(player1, player3)) {
      CompareAndRemove(player2, player4);
      return;
    } else if (CompareAndRemove(player1, player4)) {
      CompareAndRemove(player2, player3);
      return;
    } else if (CompareAndRemove(player2, player3)) {
      return;
    } else if (CompareAndRemove(player2, player4)) {
      return;
    } else if (CompareAndRemove(player3, player4)) {
      return;
    }
   unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(player1.begin(), player1.end(), std::default_random_engine(seed));
    std::shuffle(player2.begin(), player2.end(), std::default_random_engine(seed));
    std::shuffle(player3.begin(), player3.end(), std::default_random_engine(seed));
    std::shuffle(player4.begin(), player4.end(), std::default_random_engine(seed));
  }

  bool IsGameCompleted() const {
    return player1.empty() || player2.empty() || player3.empty() ||
           player4.empty();
  }

  void PlayGame() {
    while (IsGameCompleted() == false) {
      PlayOneRound();
    }
  }

  int GetWinner() const {
    if (player1.empty()) return 1;
    if (player2.empty()) return 2;
    if (player3.empty()) return 3;
    if (player4.empty()) return 4;
  }

};

int main() {
  Game newGame;
  newGame.InitDeck();
  newGame.DivideDeck();
  newGame.PlayGame();
  auto winner = newGame.GetWinner();
  std::cout << "player" << winner << " is winner!" << std::endl;

  return 0;
}