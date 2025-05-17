#include "00_test.h"

_MAIN_HEAD_

struct pokeval_hand_t hands[] = {
    // Kings vs Queens
    {{{QUEEN, HEARTS}, {TEN, SPADES}, {QUEEN, CLUBS}, {THREE, DIAMONDS}, {FOUR, HEARTS}}},
    {{{TWO, SPADES}, {KING, CLUBS}, {SEVEN, CLUBS}, {KING, DIAMONDS}, {NINE, CLUBS}}},
    {{{TWO, SPADES}, {ACE, CLUBS}, {SEVEN, CLUBS}, {KING, DIAMONDS}, {NINE, CLUBS}}},

    // Aces vs Twos
    {{{ACE, HEARTS}, {TEN, SPADES}, {ACE, CLUBS}, {THREE, DIAMONDS}, {FOUR, HEARTS}}},
    {{{TWO, SPADES}, {THREE, CLUBS}, {SEVEN, CLUBS}, {KING, DIAMONDS}, {NINE, CLUBS}}},
    {{{TWO, SPADES}, {ACE, CLUBS}, {SEVEN, CLUBS}, {TWO, DIAMONDS}, {NINE, CLUBS}}},

    // Twos vs Nothing
    {{{TWO, SPADES}, {KING, CLUBS}, {SEVEN, CLUBS}, {ACE, DIAMONDS}, {NINE, CLUBS}}},
    {{{TWO, SPADES}, {ACE, CLUBS}, {SEVEN, CLUBS}, {KING, DIAMONDS}, {NINE, CLUBS}}},
    {{{TWO, HEARTS}, {TEN, SPADES}, {TWO, CLUBS}, {THREE, DIAMONDS}, {FOUR, HEARTS}}},
    // Twos tied
    {{{TWO, DIAMONDS}, {TEN, CLUBS}, {TWO, SPADES}, {THREE, CLUBS}, {FOUR, CLUBS}}},
    {{{TWO, SPADES}, {ACE, CLUBS}, {SEVEN, CLUBS}, {KING, DIAMONDS}, {NINE, CLUBS}}},
    {{{TWO, HEARTS}, {TEN, SPADES}, {TWO, CLUBS}, {THREE, DIAMONDS}, {FOUR, HEARTS}}},
};

size_t num_hands = sizeof hands / sizeof hands[0];
assert(num_hands % 3 == 0); // Make sure the total is a multiple of 3

for (size_t i = 0; i < num_hands; i += 3) {
  struct pokeval_need_comparing_t need_comparing[3] = {
      {.id = 0, .hand = hands[i]},
      {.id = 1, .hand = hands[i + 1]},
      {.id = 2, .hand = hands[i + 2]},
  };

  int n_wins = pokeval_compare_hands(need_comparing, 3);
  fprintf(stderr, "winners: %d\n", n_wins);

  for (int j = 0; j < 3; ++j) {
    fprintf(stderr, "%zd: hand %d %s\n", i / 3, j, need_comparing[j].won ? "won" : "lost");
  }

  switch (i / 3) {
  case 0:
    assert(need_comparing[1].won);
    assert(!need_comparing[0].won);
    assert(!need_comparing[2].won);
    break;
  case 1:
    assert(need_comparing[0].won);
    assert(!need_comparing[1].won);
    assert(!need_comparing[2].won);
    break;
  case 2:
    assert(!need_comparing[0].won);
    assert(!need_comparing[1].won);
    assert(need_comparing[2].won);
    break;
  case 3:
    assert(need_comparing[0].won);
    assert(!need_comparing[1].won);
    assert(need_comparing[2].won);
  };
}

_MAIN_TAIL_
