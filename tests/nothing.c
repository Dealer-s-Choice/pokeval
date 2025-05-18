#include "00_test.h"

_MAIN_HEAD_

struct pokeval_hand_t ranked[] = {
    {{{QUEEN, HEARTS}, {TEN, SPADES}, {ACE, CLUBS}, {THREE, DIAMONDS}, {FOUR, HEARTS}}},
    {{{ACE, SPADES}, {KING, CLUBS}, {SEVEN, CLUBS}, {FIVE, DIAMONDS}, {NINE, CLUBS}}},
    {{{TWO, SPADES}, {ACE, CLUBS}, {SEVEN, CLUBS}, {KING, DIAMONDS}, {NINE, CLUBS}}},
};

for (size_t i = 0; i < sizeof ranked / sizeof ranked[0]; i++)
    assert(pokeval_evaluate_hand(ranked[i]) == NOTHING);

struct pokeval_need_comparing_t need_comparing[3] = {
  {.id = 0, .hand = ranked[0]},
  {.id = 1, .hand = ranked[1]},
  {.id = 2, .hand = ranked[2]},
};

int n_wins = pokeval_compare_hands(need_comparing, 3);
assert(n_wins == 1);
fprintf(stderr, "winners: %d\n", n_wins);
assert(need_comparing[1].won);

//memcpy(&need_comparing[0].hand, &hands[i], sizeof hands[i]);
//memcpy(&need_comparing[1].hand, &hands[i + 1], sizeof hands[i + 1]);
//memcpy(&need_comparing[2].hand, &hands[i + 2], sizeof hands[i + 2]);

_MAIN_TAIL_
