#include "00_test.h"

_MAIN_HEAD_

POKEVAL_Hand_7 hands1[3] = {
    // Hand 0: Full House (Aces over Kings)
    {{{DH_CARD_ACE, DH_SUIT_HEARTS}, {DH_CARD_ACE, DH_SUIT_SPADES}, {DH_CARD_KING, DH_SUIT_CLUBS}, {DH_CARD_KING, DH_SUIT_DIAMONDS}, {DH_CARD_ACE, DH_SUIT_CLUBS}}},
    // Hand 1: Straight Flush (5-6-7-8-9 of Spades)
    {{{DH_CARD_FIVE, DH_SUIT_SPADES}, {DH_CARD_SIX, DH_SUIT_SPADES}, {DH_CARD_SEVEN, DH_SUIT_SPADES}, {DH_CARD_EIGHT, DH_SUIT_SPADES}, {DH_CARD_NINE, DH_SUIT_SPADES}}},
    // Hand 2: Four of a Kind (Queens)
    {{{DH_CARD_QUEEN, DH_SUIT_HEARTS}, {DH_CARD_QUEEN, DH_SUIT_SPADES}, {DH_CARD_QUEEN, DH_SUIT_CLUBS}, {DH_CARD_QUEEN, DH_SUIT_DIAMONDS}, {DH_CARD_TWO, DH_SUIT_HEARTS}}},
};

POKEVAL_NeedComparing need_comparing1[3] = {
    {.id = 0, .hand = hands1[0]},
    {.id = 1, .hand = hands1[1]},
    {.id = 2, .hand = hands1[2]},
};

uint8_t n_wins = POKEVAL_compare_hands(need_comparing1, 3);
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

POKEVAL_Hand_7 hands2[3] = {
    // Hand 0: Four of a Kind (Queens)
    {{{DH_CARD_QUEEN, DH_SUIT_HEARTS}, {DH_CARD_QUEEN, DH_SUIT_SPADES}, {DH_CARD_QUEEN, DH_SUIT_CLUBS}, {DH_CARD_QUEEN, DH_SUIT_DIAMONDS}, {DH_CARD_TWO, DH_SUIT_HEARTS}}},
    // Hand 1: Straight (5-6-7-8-9 of Clubs)
    {{{DH_CARD_FIVE, DH_SUIT_CLUBS}, {DH_CARD_SIX, DH_SUIT_CLUBS}, {DH_CARD_SEVEN, DH_SUIT_CLUBS}, {DH_CARD_EIGHT, DH_SUIT_CLUBS}, {DH_CARD_NINE, DH_SUIT_CLUBS}}},
    // Hand 2: Straight (5-6-7-8-9 of Spades)
    {{{DH_CARD_FIVE, DH_SUIT_SPADES}, {DH_CARD_SEVEN, DH_SUIT_SPADES}, {DH_CARD_EIGHT, DH_SUIT_SPADES}, {DH_CARD_NINE, DH_SUIT_SPADES}, {DH_CARD_SIX, DH_SUIT_SPADES}}},
};

POKEVAL_NeedComparing need_comparing2[3] = {
    {.id = 0, .hand = hands2[0]},
    {.id = 1, .hand = hands2[1]},
    {.id = 2, .hand = hands2[2]},
};

n_wins = POKEVAL_compare_hands(need_comparing2, 3);
fprintf(stderr, "winners: %d\n", n_wins);
assert(n_wins == 2);
assert(need_comparing2[0].won == false);
assert(need_comparing2[1].won);
assert(need_comparing2[2].won);

for (size_t i = 0; i < 3; ++i) {
  if (need_comparing2[i].won)
    fprintf(stderr, "Winner: %d\n", need_comparing2[i].id);
}

_MAIN_TAIL_
