#include "00_test.h"

void set_hand(struct pokeval_hand_t *hand, int faces[HAND_SIZE], int suits[HAND_SIZE]) {
  for (int i = 0; i < HAND_SIZE; ++i) {
    hand->card[i].face_val = faces[i];
    hand->card[i].suit = suits[i];
  }
}
