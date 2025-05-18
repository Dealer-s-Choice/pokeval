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

#include "pokeval.h"

const char *pokeval_ranks[NUM_HAND_RANKS] = {[NOTHING] = "Nothing",
                                             [PAIR] = "Pair",
                                             [TWO_PAIR] = "Two Pair",
                                             [THREE_OF_A_KIND] = "Three-of-a-Kind",
                                             [STRAIGHT] = "Straight",
                                             [FLUSH] = "Flush",
                                             [FULL_HOUSE] = "Full House",
                                             [FOUR_OF_A_KIND] = "Four-of-a-Kind",
                                             [STRAIGHT_FLUSH] = "Straight Flush",
                                             [ROYAL_FLUSH] = "Royal Flush"};

static inline int face_rank(int val) { return (val == ACE) ? 14 : val; }

static int count_face(const struct pokeval_hand_t *hand, int face_val) {
  int count = 0;
  for (int i = 0; i < HAND_SIZE; ++i)
    if (hand->card[i].face_val == face_val)
      count++;
  return count;
}

void sort_hand(struct pokeval_hand_t *hand) {
  for (int i = 0; i < HAND_SIZE - 1; ++i) {
    for (int j = i + 1; j < HAND_SIZE; ++j) {
      int val_i = (hand->card[i].face_val == ACE) ? PKEV_ACE : hand->card[i].face_val;
      int val_j = (hand->card[j].face_val == ACE) ? PKEV_ACE : hand->card[j].face_val;
      hand->card[i].face_val = val_i;
      hand->card[j].face_val = val_j;

      if (val_i < val_j) {
        struct dh_card tmp = hand->card[i];
        hand->card[i] = hand->card[j];
        hand->card[j] = tmp;
      }
    }
  }
}

static bool is_straight(struct pokeval_hand_t *hand) {
  int faces[HAND_SIZE];
  for (int i = 0; i < HAND_SIZE; ++i) {
    faces[i] = hand->card[i].face_val;
    //fprintf(stderr, "face_val: %d | ", faces[i]);
  }
  putchar('\n');

  if (faces[0] == PKEV_ACE && faces[1] == FIVE && faces[2] == FOUR &&
      faces[3] == THREE && faces[4] == TWO) {
    // hand->card[0].face_val = ACE;
    return true;
  }

  if (faces[0] == PKEV_ACE && faces[1] == KING && faces[2] == QUEEN &&
      faces[3] == JACK && faces[4] == TEN)
    return true;

  for (int i = 1; i < HAND_SIZE; ++i)
    if (faces[i] != faces[i - 1] - 1)
      return false;

  return true;
}

short pokeval_evaluate_hand(struct pokeval_hand_t hand) {
  sort_hand(&hand);

  bool flush = true;
  for (int i = 1; i < HAND_SIZE; ++i)
    if (hand.card[i].suit != hand.card[0].suit)
      flush = false;

  bool straight = is_straight(&hand);

  if (straight && flush && hand.card[0].face_val == PKEV_ACE)
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

static int get_triplet_value(const struct pokeval_hand_t *hand) {
  for (int i = 0; i <= HAND_SIZE - 3; ++i)
    if (count_face(hand, hand->card[i].face_val) == 3)
      return hand->card[i].face_val;
  return -1;
}

static int get_quad_value(const struct pokeval_hand_t *hand) {
  for (int i = 0; i <= HAND_SIZE - 4; ++i)
    if (count_face(hand, hand->card[i].face_val) == 4)
      return hand->card[i].face_val;
  return -1;
}

static int compare_high_cards(const struct pokeval_hand_t *a, const struct pokeval_hand_t *b) {
  for (int i = 0; i < HAND_SIZE; ++i) {
    if (a->card[i].face_val > b->card[i].face_val)
      return 1;
    if (a->card[i].face_val < b->card[i].face_val)
      return -1;
  }
  return 0;
}

static int compare_one_pair_tiebreak(const struct pokeval_hand_t *a,
                                     const struct pokeval_hand_t *b) {
  int a_pair = 0, b_pair = 0;
  int a_kickers[3] = {0}, b_kickers[3] = {0};
  int a_k = 0, b_k = 0;

  // Assumes hands are already sorted in descending order
  for (int i = 0; i < HAND_SIZE - 1; ++i) {
    if (face_rank(a->card[i].face_val) == face_rank(a->card[i + 1].face_val)) {
      a_pair = face_rank(a->card[i].face_val);
      break;
    }
  }
  for (int i = 0; i < HAND_SIZE - 1; ++i) {
    if (face_rank(b->card[i].face_val) == face_rank(b->card[i + 1].face_val)) {
      b_pair = face_rank(b->card[i].face_val);
      break;
    }
  }

  // Extract kickers
  for (int i = 0; i < HAND_SIZE; ++i) {
    int val_a = face_rank(a->card[i].face_val);
    int val_b = face_rank(b->card[i].face_val);

    if (val_a != a_pair && a_k < 3) {
      a_kickers[a_k++] = val_a;
    }
    if (val_b != b_pair && b_k < 3) {
      b_kickers[b_k++] = val_b;
    }
  }

  // Sort kickers descending (simple selection sort for 3 elements)
  for (int i = 0; i < 2; ++i) {
    for (int j = i + 1; j < 3; ++j) {
      if (a_kickers[j] > a_kickers[i]) {
        int tmp = a_kickers[i];
        a_kickers[i] = a_kickers[j];
        a_kickers[j] = tmp;
      }
      if (b_kickers[j] > b_kickers[i]) {
        int tmp = b_kickers[i];
        b_kickers[i] = b_kickers[j];
        b_kickers[j] = tmp;
      }
    }
  }

  // fprintf(stderr, "a_pair=%d b_pair=%d\n", a_pair, b_pair);
  // fprintf(stderr, "a_kickers = %d %d %d\n", a_kickers[0], a_kickers[1], a_kickers[2]);
  // fprintf(stderr, "b_kickers = %d %d %d\n", b_kickers[0], b_kickers[1], b_kickers[2]);

  // Compare pair
  if (a_pair > b_pair)
    return 1;
  if (a_pair < b_pair)
    return -1;

  // Compare kickers
  for (int i = 0; i < 3; ++i) {
    if (a_kickers[i] > b_kickers[i])
      return 1;
    if (a_kickers[i] < b_kickers[i])
      return -1;
  }

  return 0; // hands are tied
}

static int compare_two_pair_tiebreak(const struct pokeval_hand_t *a,
                                     const struct pokeval_hand_t *b) {
  int a_high = 0, a_low = 0, a_kicker = 0;
  int b_high = 0, b_low = 0, b_kicker = 0;

  // Assumes hand is sorted high to low
  for (int i = 0; i < HAND_SIZE - 1; ++i) {
    if (a->card[i].face_val == a->card[i + 1].face_val) {
      if (a_high == 0)
        a_high = a->card[i].face_val;
      else if (a_low == 0 && a->card[i].face_val != a_high)
        a_low = a->card[i].face_val;
    }
    if (b->card[i].face_val == b->card[i + 1].face_val) {
      if (b_high == 0)
        b_high = b->card[i].face_val;
      else if (b_low == 0 && b->card[i].face_val != b_high)
        b_low = b->card[i].face_val;
    }
  }

  if (a_high < a_low) {
    int tmp = a_high;
    a_high = a_low;
    a_low = tmp;
  }
  if (b_high < b_low) {
    int tmp = b_high;
    b_high = b_low;
    b_low = tmp;
  }

  for (int i = 0; i < HAND_SIZE; ++i) {
    int val = a->card[i].face_val;
    if (val != a_high && val != a_low)
      a_kicker = val;
    val = b->card[i].face_val;
    if (val != b_high && val != b_low)
      b_kicker = val;
  }

  if (a_high > b_high)
    return -1;
  if (a_high < b_high)
    return 1;
  if (a_low > b_low)
    return -1;
  if (a_low < b_low)
    return 1;
  if (a_kicker > b_kicker)
    return -1;
  if (a_kicker < b_kicker)
    return 1;
  return 0;
}

uint8_t pokeval_compare_hands(struct pokeval_need_comparing_t *need_comparing, uint8_t count) {
  for (size_t i = 0; i < count; ++i) {
    sort_hand(&need_comparing[i].hand);
  }

  uint8_t num_winners = 0;
  short best_rank = -1;
  struct pokeval_hand_t best_hand = {0};
  uint8_t winner_indices[count];

  // Evaluate all hands and determine the best one(s)
  for (uint8_t i = 0; i < count; ++i) {
    need_comparing[i].won = false;
    struct pokeval_hand_t current_hand = need_comparing[i].hand;
    short rank = pokeval_evaluate_hand(current_hand);

    if (rank > best_rank) {
      best_rank = rank;
      best_hand = current_hand;
      winner_indices[0] = i;
      num_winners = 1;
    } else if (rank == best_rank) {
      struct pokeval_hand_t a = best_hand;
      struct pokeval_hand_t b = current_hand;
      // sort_hand(&a);
      // sort_hand(&b);

      bool b_wins = false;
      bool tie = false;

      switch (rank) {
      case ROYAL_FLUSH:
        tie = true;
        break;
      case STRAIGHT_FLUSH:
      case STRAIGHT: {
        int a_high = a.card[0].face_val;
        int b_high = b.card[0].face_val;
        if (a_high == PKEV_ACE && a.card[1].face_val == FIVE)
          a_high = 5;
        if (b_high == PKEV_ACE && b.card[1].face_val == FIVE)
          b_high = 5;
        if (a_high == b_high)
          tie = true;
        else
          b_wins = b_high > a_high;
        break;
      }
      case FOUR_OF_A_KIND: {
        int a_quad = get_quad_value(&a);
        int b_quad = get_quad_value(&b);
        if (a_quad == b_quad)
          tie = true;
        else
          b_wins = b_quad > a_quad;
        break;
      }
      case FULL_HOUSE: {
        int a_trip = get_triplet_value(&a);
        int b_trip = get_triplet_value(&b);
        int a_pair = -1, b_pair = -1;
        for (int j = 0; j < HAND_SIZE; ++j) {
          int val = a.card[j].face_val;
          if (val != a_trip && count_face(&a, val) == 2) {
            a_pair = val;
            break;
          }
        }
        for (int j = 0; j < HAND_SIZE; ++j) {
          int val = b.card[j].face_val;
          if (val != b_trip && count_face(&b, val) == 2) {
            b_pair = val;
            break;
          }
        }
        if (a_trip == b_trip && a_pair == b_pair)
          tie = true;
        else if (b_trip > a_trip || (b_trip == a_trip && b_pair > a_pair))
          b_wins = true;
        break;
      }
      case FLUSH:
      case NOTHING: {
        int cmp = compare_high_cards(&a, &b);
        if (cmp == 0)
          tie = true;
        else if (cmp < 0)
          b_wins = true;
        break;
      }
      case THREE_OF_A_KIND: {
        int a_trip = get_triplet_value(&a);
        int b_trip = get_triplet_value(&b);
        if (a_trip == b_trip)
          tie = true;
        else
          b_wins = b_trip > a_trip;
        break;
      }
      case TWO_PAIR: {
        int cmp = compare_two_pair_tiebreak(&a, &b);
        if (cmp == 0)
          tie = true;
        else if (cmp > 0)
          b_wins = true;
        break;
      }
      case PAIR: {
        int cmp = compare_one_pair_tiebreak(&a, &b);
        if (cmp == 0)
          tie = true;
        else if (cmp < 0)
          b_wins = true;
        break;
      }
      }

      if (tie) {
        winner_indices[num_winners++] = i;
      } else if (b_wins) {
        best_hand = b;
        winner_indices[0] = i;
        num_winners = 1;
      }
    }
  }

  // Mark only the winners
  for (uint8_t i = 0; i < num_winners; ++i) {
    need_comparing[winner_indices[i]].won = true;
  }

  return num_winners;
}
