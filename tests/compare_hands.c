#include "00_test.h"

_MAIN_HEAD_

struct pokeval_hand_t hands1[3] = {
    // Hand 0: Full House (Aces over Kings)
    {{{ACE, HEARTS}, {ACE, SPADES}, {KING, CLUBS}, {KING, DIAMONDS}, {ACE, CLUBS}}},
    // Hand 1: Straight Flush (5-6-7-8-9 of Spades)
    {{{FIVE, SPADES}, {SIX, SPADES}, {SEVEN, SPADES}, {EIGHT, SPADES}, {NINE, SPADES}}},
    // Hand 2: Four of a Kind (Queens)
    {{{QUEEN, HEARTS}, {QUEEN, SPADES}, {QUEEN, CLUBS}, {QUEEN, DIAMONDS}, {TWO, HEARTS}}},
};

struct pokeval_need_comparing_t need_comparing1[3] = {
    {.id = 0, .hand = hands1[0]},
    {.id = 1, .hand = hands1[1]},
    {.id = 2, .hand = hands1[2]},
};

uint8_t n_wins = pokeval_compare_hands(need_comparing1, 3);
fprintf(stderr, "winners: %d\n", n_wins);
assert(n_wins == 1);
assert(need_comparing1[0].won == false);
assert(need_comparing1[1].won); // Hand 1 should win (straight flush)
assert(need_comparing1[2].won == false);

for (size_t i = 0; i < 3; ++i) {
  if (need_comparing1[i].won)
    fprintf(stderr, "Winner: %d\n", need_comparing1[i].id);
}

// --- Second test with tie between two straights ---

struct pokeval_hand_t hands2[3] = {
    // Hand 0: Four of a Kind (Queens)
    {{{QUEEN, HEARTS}, {QUEEN, SPADES}, {QUEEN, CLUBS}, {QUEEN, DIAMONDS}, {TWO, HEARTS}}},
    // Hand 1: Straight (5-6-7-8-9 of Clubs)
    {{{FIVE, CLUBS}, {SIX, CLUBS}, {SEVEN, CLUBS}, {EIGHT, CLUBS}, {NINE, CLUBS}}},
    // Hand 2: Straight (5-6-7-8-9 of Spades)
    {{{FIVE, SPADES}, {SIX, SPADES}, {SEVEN, SPADES}, {EIGHT, SPADES}, {NINE, SPADES}}},
};

struct pokeval_need_comparing_t need_comparing2[3] = {
    {.id = 0, .hand = hands2[0]},
    {.id = 1, .hand = hands2[1]},
    {.id = 2, .hand = hands2[2]},
};

n_wins = pokeval_compare_hands(need_comparing2, 3);
fprintf(stderr, "winners: %d\n", n_wins);
assert(n_wins == 2);
assert(need_comparing2[0].won == false);
assert(need_comparing2[1].won);
assert(need_comparing2[2].won);

for (size_t i = 0; i < 3; ++i) {
  if (need_comparing2[i].won)
    fprintf(stderr, "Winner: %d\n", need_comparing2[i].id);
}

struct pokeval_hand_t hands3[2] = {
    // Hand 0: Four of a Kind (Queens)
    {{{QUEEN, HEARTS}, {QUEEN, SPADES}, {TWO, CLUBS}, {THREE, DIAMONDS}, {FOUR, HEARTS}}},
    // Hand 1: Straight (5-6-7-8-9 of Clubs)
    {{{ACE, SPADES}, {SIX, CLUBS}, {SEVEN, CLUBS}, {EIGHT, CLUBS}, {NINE, CLUBS}}},
};

struct pokeval_need_comparing_t need_comparing3[2] = {
    {.id = 0, .hand = hands3[0]},
    {.id = 1, .hand = hands3[1]},
};

n_wins = pokeval_compare_hands(need_comparing3, 2);
fprintf(stderr, "winners: %d\n", n_wins);
assert(n_wins == 1);
assert(need_comparing3[0].won);
assert(need_comparing3[1].won == false);

for (size_t i = 0; i < 2; ++i) {
  if (need_comparing3[i].won)
    fprintf(stderr, "Winner: %d\n", need_comparing2[i].id);
}

struct pokeval_hand_t hands4[2] = {
    // Hand 0: Four of a Kind (Queens)
    {{{QUEEN, HEARTS}, {TEN, SPADES}, {TWO, CLUBS}, {THREE, DIAMONDS}, {FOUR, HEARTS}}},
    // Hand 1: Straight (5-6-7-8-9 of Clubs)
    {{{ACE, SPADES}, {SIX, CLUBS}, {SEVEN, CLUBS}, {EIGHT, CLUBS}, {NINE, CLUBS}}},
};

struct pokeval_need_comparing_t need_comparing4[2] = {
    {.id = 0, .hand = hands4[0]},
    {.id = 1, .hand = hands4[1]},
};

n_wins = pokeval_compare_hands(need_comparing4, 2);
fprintf(stderr, "winners: %d\n", n_wins);
assert(n_wins == 1);
assert(need_comparing4[0].won == false);
assert(need_comparing4[1].won);

_MAIN_TAIL_
