#include "00_test.h"

_MAIN_HEAD_

struct pokeval_hand_t hand = {.card = {
                                  {TEN, HEARTS},
                                  {THREE, CLUBS},
                                  {ACE, DIAMONDS},
                                  {FIVE, SPADES},
                                  {KING, CLUBS},
                              }};

sort_hand(&hand);

int expected[] = {PKEV_ACE, KING, TEN, FIVE, THREE};
for (int i = 0; i < HAND_SIZE; ++i) {
  fprintf(stderr, "card: %d | ", hand.card[i].face_val);
  assert(hand.card[i].face_val == expected[i]);
}
putchar('\n');

_MAIN_TAIL_
