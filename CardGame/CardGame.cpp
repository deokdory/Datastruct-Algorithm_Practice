#include <time.h>

#include <algorithm>
#include <array>
#include <forward_list>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#define CARD_TOTAL 52
#define SHUFFLE_COUNT 200

#define STARTING_CARDS 13

enum Pattern {
  Spade = 0,
  Diamond,
  Clover,
  Heart,
};

enum CardNum { J = 11, Q, K };

struct Card {
  int pattern;
  int number;

  std::string GetPattern() const {
    switch (pattern) {
      case Spade:
        return "♠";
      case Diamond:
        return "◆";
      case Clover:
        return "♣";
      case Heart:
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

  friend bool operator==(Card left, Card right) {
    return left.number == right.number && left.pattern == right.pattern;
  }
  friend bool operator!=(Card left, Card right) {
    return left.number != right.number || left.pattern != right.pattern;
  }
};

struct Deck {
 private:
  std::string name;
  std::list<Card> deck;
  std::list<Card> hand;
  int hand_count = 0;

 public:
  Deck() = default;
  Deck(std::string name) : name(name) {}
  Deck(const Deck& other) {
    name = other.name;
    deck = other.deck;
    hand = other.hand;
    hand_count = other.hand_count;
  }

  std::string GetName() const { return name; }
  int GetHandCount() const { return hand_count; }
  std::list<Card>* GetHand() { return &hand; }

  void SetName(std::string _name) { name = _name; }
  void DecreaseHandCount() { hand_count--; }
  // 카드 덱 초기화
  void InitDeck() {
    Card* card = nullptr;
    for (int i = 0; i < 4; i++) {
      int pattern = i;
      for (int j = 1; j <= 13; j++) {
        card = new Card{i, j};
        deck.push_back(*card);
      }
    }
  }

  void Shuffle() {
    std::vector<Card> shuffler(deck.size());
    for (const auto& i : deck) {
      shuffler.push_back(i);
    }
    int a, b;
    for (int i = 0; i < SHUFFLE_COUNT; i++) {
      a = rand() % CARD_TOTAL;
      b = rand() % CARD_TOTAL;
      std::swap(shuffler[a], shuffler[b]);
    }
    deck.clear();
    for (const auto& i : shuffler) {
      deck.push_back(i);
    }
  }

  void Hand(int num_of_cards) {
    if (num_of_cards <= deck.size()) {
      auto rit = deck.rbegin();
      for (int i = 0; i < num_of_cards; i++) {
        hand.push_back(*rit);
        hand_count++;
        rit++;
        deck.pop_back();
      }
    }
  }

  void ReturnHand() {
    if (!hand.empty()) {
      auto rit = hand.rbegin();
      for (rit; rit != hand.rend(); rit++) {
        deck.push_front(*rit);
        rit++;
        hand.pop_back();
      }
      hand_count = 0;
    }
  }

  Card* DrawOnHand() {
    int d = rand() % hand.size();
    auto it = hand.begin();
    for (int i = 0; i < d; i++) {
      it++;
    }
    return &(*it);
  }
};

struct Game {
 public:
  using deck_vector = std::vector<Deck>;

 private:
  deck_vector player;
  std::forward_list<Deck*> winners;

 public:
  Game() {
    Deck* tmp;
    std::string name;
    for (int i = 0; i < 4; i++) {
      name = "Player";
      tmp = new Deck(name);
      player.push_back(*tmp);
    }
  }

  Game(const std::initializer_list<std::string>& ilist) {
    auto it = ilist.begin();
    std::string name;
    for (int i = 0; i < 4; i++) {
      if (it != ilist.end()) {
        name = *it;
        it++;
      } else {
        name = "Player" + (i + 1);
      }
      player[i].SetName(name);
    }
  }

  deck_vector GetPlayer() { return player; }

  void Init() {
    for (auto& i : player) {
      i.InitDeck();
      i.Hand(STARTING_CARDS);
    }
  }

  void CheckPair() {
    std::array<Card*, 4> drawn_card = {};
    std::array<bool, 4> paired = {};
    auto it = player.begin();
    for (int i = 0; i < 4; i++) {
      drawn_card[i] = it->DrawOnHand();
      std::cout << it->GetName() << "이(가) 선택한 카드: " << *drawn_card[i] << std::endl;
    }
    for (int i = 0; i < 4; i++) {
      for (int j = i + 1; j < 4; j++) {
        if (drawn_card[i] != nullptr && drawn_card[j] != nullptr) {
          if (drawn_card[i] == drawn_card[j]) {
            player[i].GetHand()->remove(*drawn_card[i]);
            player[j].GetHand()->remove(*drawn_card[j]);

            player[i].DecreaseHandCount();
            player[j].DecreaseHandCount();

            paired[i] = paired[j] = true;

            drawn_card[i] = drawn_card[j] = nullptr;
            break;
          }
        }
      }
    }
    for (int i = 0; i < 4; i++) {
      if (paired[i] == false) {
        int hand_count = player[i].GetHandCount();
        player[i].ReturnHand();
        player[i].Shuffle();
        player[i].Hand(hand_count);
      }
    }
  }

  void Round() {
    std::rotate(player.begin(), player.begin() + 1, player.end());
  }

  bool CheckWinner() {
    auto it = player.begin();
    for (int i = 0; i < 4; i++) {
      if (it->GetHandCount() == 0) 
      winners.push_front(&(*it));
    }
    if (!winners.empty()) {
      std::cout << "Winners: ";
      for (auto i : winners) {
        std::cout << i->GetName() << " ";
      }
      return true;
    }
    return false;
  }
};

int main() {
  srand((unsigned)time(NULL));

  Game game;

  game.Init();

  while (true) {
    if (game.CheckWinner()) break;

    for (auto i : game.GetPlayer()) {
      std::cout << i.GetName() << " | 손에 남은 카드 수:" << i.GetHandCount()
                << std::endl;
    }
    game.CheckPair();
    game.Round();
  }
}
