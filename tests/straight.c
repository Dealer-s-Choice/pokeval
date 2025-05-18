#include "00_test.h"

_MAIN_HEAD_

struct pokeval_hand_t ranked[] = {
    {{{QUEEN, HEARTS}, {TEN, SPADES}, {NINE, CLUBS}, {KING, DIAMONDS}, {JACK, HEARTS}}},
    {{{TEN, SPADES}, {QUEEN, CLUBS}, {JACK, CLUBS}, {KING, DIAMONDS}, {ACE, CLUBS}}},
    {{{FIVE, SPADES}, {TWO, CLUBS}, {THREE, CLUBS}, {FOUR, DIAMONDS}, {ACE, CLUBS}}},
};

for (size_t i = 0; i < sizeof ranked / sizeof ranked[0]; i++) {
    int rank = pokeval_evaluate_hand(ranked[i]);
    fprintf(stderr, "%d:%s\n", i, pokeval_ranks[rank]);
    assert(rank == STRAIGHT);
  }

struct pokeval_need_comparing_t need_comparing[3] = {
  {.id = 0, .hand = ranked[0]},
  {.id = 1, .hand = ranked[1]},
  {.id = 2, .hand = ranked[2]},
};

int n_wins = pokeval_compare_hands(need_comparing, 3);
assert(n_wins == 1);
fprintf(stderr, "winners: %d\n", n_wins);
assert(!need_comparing[0].won);
assert(need_comparing[1].won);
assert(!need_comparing[2].won);

_MAIN_TAIL_
