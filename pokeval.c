/*
 pokeval.c
 https://github.com/Dealer-s-Choice/pokeval

 MIT License

 Copyright (c) 2025 Andy Alt

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>

#include "pokeval.h"

const char *ranks[NUM_HAND_RANKS] = {[NOTHING] = "Nothing",
                                     [PAIR] = "Pair",
                                     [TWO_PAIR] = "Two Pair",
                                     [THREE_OF_A_KIND] = "Three-of-a-Kind",
                                     [STRAIGHT] = "Straight",
                                     [FLUSH] = "Flush",
                                     [FULL_HOUSE] = "Full House",
                                     [FOUR_OF_A_KIND] = "Four-of-a-Kind",
                                     [STRAIGHT_FLUSH] = "Straight Flush",
                                     [ROYAL_FLUSH] = "Royal Flush"};

static int count_face(const struct hand_t *hand, int face_val) {
  int count = 0;
  for (int i = 0; i < HAND_SIZE; ++i)
    if (hand->card[i].face_val == face_val)
      count++;
  return count;
}

static void sort_hand(struct hand_t *hand) {
  for (int i = 0; i < HAND_SIZE - 1; ++i) {
    for (int j = i + 1; j < HAND_SIZE; ++j) {
      if (hand->card[i].face_val > hand->card[j].face_val) {
        struct dh_card tmp = hand->card[i];
        hand->card[i] = hand->card[j];
        hand->card[j] = tmp;
      }
    }
  }
}

static int compare_faces(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

static bool is_straight(const struct hand_t *hand) {
  int faces[HAND_SIZE];
  for (int i = 0; i < HAND_SIZE; ++i)
    faces[i] = hand->card[i].face_val;

  qsort(faces, HAND_SIZE, sizeof(int), compare_faces);

  if (faces[0] == ACE && faces[1] == TWO && faces[2] == THREE && faces[3] == FOUR &&
      faces[4] == FIVE)
    return true;

  if (faces[0] == ACE && faces[1] == TEN && faces[2] == JACK && faces[3] == QUEEN &&
      faces[4] == KING)
    return true;

  for (int i = 1; i < HAND_SIZE; ++i)
    if (faces[i] != faces[i - 1] + 1)
      return false;

  return true;
}

short evaluate_hand(struct hand_t hand) {
  sort_hand(&hand);

  bool flush = true;
  for (int i = 1; i < HAND_SIZE; ++i)
    if (hand.card[i].suit != hand.card[0].suit)
      flush = false;

  bool straight = is_straight(&hand);

  if (straight && flush && hand.card[0].face_val == ACE)
    return ROYAL_FLUSH;
  if (straight && flush)
    return STRAIGHT_FLUSH;
  if ((count_face(&hand, hand.card[0].face_val) == 4) ||
      (count_face(&hand, hand.card[4].face_val) == 4))
    return FOUR_OF_A_KIND;
  if ((count_face(&hand, hand.card[0].face_val) == 3 &&
       count_face(&hand, hand.card[4].face_val) == 2) ||
      (count_face(&hand, hand.card[0].face_val) == 2 &&
       count_face(&hand, hand.card[4].face_val) == 3))
    return FULL_HOUSE;
  if (flush)
    return FLUSH;
  if (straight)
    return STRAIGHT;
  if ((count_face(&hand, hand.card[0].face_val) == 3) ||
      (count_face(&hand, hand.card[2].face_val) == 3) ||
      (count_face(&hand, hand.card[4].face_val) == 3))
    return THREE_OF_A_KIND;

  int pair_count = 0;
  for (int i = 0; i < HAND_SIZE; ++i)
    if (count_face(&hand, hand.card[i].face_val) == 2)
      pair_count++;

  if (pair_count == 4)
    return TWO_PAIR;
  if (pair_count == 2)
    return PAIR;

  return NOTHING;
}
