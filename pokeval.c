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

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "pokeval.h"

const char *POKEVAL_rank[NUM_HAND_RANKS] = {[POKEVAL_HIGH_CARD] = "High Card",
                                            [POKEVAL_PAIR] = "Pair",
                                            [POKEVAL_TWO_PAIR] = "Two Pair",
                                            [POKEVAL_THREE_OF_A_KIND] = "Three-of-a-Kind",
                                            [POKEVAL_STRAIGHT] = "Straight",
                                            [POKEVAL_FLUSH] = "Flush",
                                            [POKEVAL_FULL_HOUSE] = "Full House",
                                            [POKEVAL_FOUR_OF_A_KIND] = "Four-of-a-Kind",
                                            [POKEVAL_STRAIGHT_FLUSH] = "Straight Flush",
                                            [POKEVAL_FIVE_OF_A_KIND] = "Five-of-a-Kind",
                                            [POKEVAL_ROYAL_FLUSH] = "Royal Flush"};

static inline int face_rank(int val) { return (val == DH_CARD_ACE) ? 14 : val; }

static int count_face(const POKEVAL_Hand_5 *hand, int face_val) {
  int count = 0;
  for (int i = 0; i < POKEVAL_HAND_SIZE; ++i)
    if (hand->card[i].face_val == face_val)
      count++;
  return count;
}

void POKEVAL_sort_hand(POKEVAL_Hand_5 *hand) {
  for (int i = 0; i < POKEVAL_HAND_SIZE - 1; ++i) {
    for (int j = i + 1; j < POKEVAL_HAND_SIZE; ++j) {
      int val_i = (hand->card[i].face_val == DH_CARD_ACE) ? POKEVAL_ACE : hand->card[i].face_val;
      int val_j = (hand->card[j].face_val == DH_CARD_ACE) ? POKEVAL_ACE : hand->card[j].face_val;
      hand->card[i].face_val = val_i;
      hand->card[j].face_val = val_j;

      if (val_i < val_j) {
        DH_Card tmp = hand->card[i];
        hand->card[i] = hand->card[j];
        hand->card[j] = tmp;
      }
    }
  }
}

static bool is_straight(POKEVAL_Hand_5 *hand) {
  int faces[POKEVAL_HAND_SIZE];
  for (int i = 0; i < POKEVAL_HAND_SIZE; ++i) {
    faces[i] = hand->card[i].face_val;
    // fprintf(stderr, "face_val: %d | ", faces[i]);
  }
  // putchar('\n');

  if (faces[0] == POKEVAL_ACE && faces[1] == DH_CARD_FIVE && faces[2] == DH_CARD_FOUR &&
      faces[3] == DH_CARD_THREE && faces[4] == DH_CARD_TWO) {
    // hand->card[0].face_val = DH_CARD_ACE;
    return true;
  }

  if (faces[0] == POKEVAL_ACE && faces[1] == DH_CARD_KING && faces[2] == DH_CARD_QUEEN &&
      faces[3] == DH_CARD_JACK && faces[4] == DH_CARD_TEN)
    return true;

  for (int i = 1; i < POKEVAL_HAND_SIZE; ++i)
    if (faces[i] != faces[i - 1] - 1)
      return false;

  return true;
}

short POKEVAL_evaluate_hand(POKEVAL_Hand_5 hand) {
  POKEVAL_sort_hand(&hand);

  bool flush = true;
  for (int i = 1; i < POKEVAL_HAND_SIZE; ++i)
    if (hand.card[i].suit != hand.card[0].suit)
      flush = false;

  bool straight = is_straight(&hand);

  if (straight && flush && hand.card[0].face_val == POKEVAL_ACE)
    return POKEVAL_ROYAL_FLUSH;
  if (straight && flush)
    return POKEVAL_STRAIGHT_FLUSH;
  if ((count_face(&hand, hand.card[0].face_val) == 5) ||
      (count_face(&hand, hand.card[4].face_val) == 5))
    return POKEVAL_FIVE_OF_A_KIND;
  if ((count_face(&hand, hand.card[0].face_val) == 4) ||
      (count_face(&hand, hand.card[4].face_val) == 4))
    return POKEVAL_FOUR_OF_A_KIND;
  if ((count_face(&hand, hand.card[0].face_val) == 3 &&
       count_face(&hand, hand.card[4].face_val) == 2) ||
      (count_face(&hand, hand.card[0].face_val) == 2 &&
       count_face(&hand, hand.card[4].face_val) == 3))
    return POKEVAL_FULL_HOUSE;
  if (flush)
    return POKEVAL_FLUSH;
  if (straight)
    return POKEVAL_STRAIGHT;
  if ((count_face(&hand, hand.card[0].face_val) == 3) ||
      (count_face(&hand, hand.card[2].face_val) == 3) ||
      (count_face(&hand, hand.card[4].face_val) == 3))
    return POKEVAL_THREE_OF_A_KIND;

  int pair_count = 0;
  for (int i = 0; i < POKEVAL_HAND_SIZE; ++i)
    if (count_face(&hand, hand.card[i].face_val) == 2)
      pair_count++;

  if (pair_count == 4)
    return POKEVAL_TWO_PAIR;
  if (pair_count == 2)
    return POKEVAL_PAIR;

  return POKEVAL_HIGH_CARD;
}

static int get_triplet_value(const POKEVAL_Hand_5 *hand) {
  for (int i = 0; i <= POKEVAL_HAND_SIZE - 3; ++i)
    if (count_face(hand, hand->card[i].face_val) == 3)
      return hand->card[i].face_val;
  return -1;
}

static int get_quint_value(const POKEVAL_Hand_5 *hand) {
  for (int i = 0; i <= POKEVAL_HAND_SIZE - 5; ++i)
    if (count_face(hand, hand->card[i].face_val) == 5)
      return hand->card[i].face_val;
  return -1;
}

static int get_quad_value(const POKEVAL_Hand_5 *hand) {
  for (int i = 0; i <= POKEVAL_HAND_SIZE - 4; ++i)
    if (count_face(hand, hand->card[i].face_val) == 4)
      return hand->card[i].face_val;
  return -1;
}

static int compare_high_cards(const POKEVAL_Hand_5 *a, const POKEVAL_Hand_5 *b) {
  for (int i = 0; i < POKEVAL_HAND_SIZE; ++i) {
    if (a->card[i].face_val > b->card[i].face_val)
      return 1;
    if (a->card[i].face_val < b->card[i].face_val)
      return -1;
  }
  return 0;
}

static int compare_one_pair_tiebreak(const POKEVAL_Hand_5 *a, const POKEVAL_Hand_5 *b) {
  int a_pair = 0, b_pair = 0;
  int a_kickers[3] = {0}, b_kickers[3] = {0};
  int a_k = 0, b_k = 0;

  // Assumes hands are already sorted in descending order
  for (int i = 0; i < POKEVAL_HAND_SIZE - 1; ++i) {
    if (face_rank(a->card[i].face_val) == face_rank(a->card[i + 1].face_val)) {
      a_pair = face_rank(a->card[i].face_val);
      break;
    }
  }
  for (int i = 0; i < POKEVAL_HAND_SIZE - 1; ++i) {
    if (face_rank(b->card[i].face_val) == face_rank(b->card[i + 1].face_val)) {
      b_pair = face_rank(b->card[i].face_val);
      break;
    }
  }

  // Extract kickers
  for (int i = 0; i < POKEVAL_HAND_SIZE; ++i) {
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

static int compare_two_pair_tiebreak(const POKEVAL_Hand_5 *a, const POKEVAL_Hand_5 *b) {
  int a_high = 0, a_low = 0, a_kicker = 0;
  int b_high = 0, b_low = 0, b_kicker = 0;

  // Assumes hand is sorted high to low
  for (int i = 0; i < POKEVAL_HAND_SIZE - 1; ++i) {
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

  for (int i = 0; i < POKEVAL_HAND_SIZE; ++i) {
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

static int get_kicker_value(const POKEVAL_Hand_5 *hand, int quad_val) {
  for (int i = 0; i < POKEVAL_HAND_SIZE; ++i) {
    if (hand->card[i].face_val != quad_val)
      return hand->card[i].face_val;
  }
  return -1; // Should never happen in valid hand
}

static uint8_t compare_hands_5(POKEVAL_NeedComparing *need_comparing, uint8_t count) {
  for (size_t i = 0; i < count; ++i) {
    POKEVAL_sort_hand(&need_comparing[i].hand_5);
  }

  uint8_t num_winners = 0;
  short best_rank = -1;
  POKEVAL_Hand_5 best_hand = {0};
  assert(count > 0);
  uint8_t winner_indices[count];
  winner_indices[0] = 0;

  // Evaluate all hands and determine the best one(s)
  for (uint8_t i = 0; i < count; ++i) {
    need_comparing[i].won = false;
    POKEVAL_Hand_5 current_hand = need_comparing[i].hand_5;
    short rank = POKEVAL_evaluate_hand(current_hand);

    if (rank > best_rank) {
      best_rank = rank;
      best_hand = current_hand;
      winner_indices[0] = i;
      num_winners = 1;
    } else if (rank == best_rank) {
      POKEVAL_Hand_5 a = best_hand;
      POKEVAL_Hand_5 b = current_hand;
      // sort_hand(&a);
      // sort_hand(&b);

      bool b_wins = false;
      bool tie = false;

      switch (rank) {
      case POKEVAL_ROYAL_FLUSH:
        tie = true;
        break;
      case POKEVAL_STRAIGHT_FLUSH:
      case POKEVAL_STRAIGHT: {
        int a_high = a.card[0].face_val;
        int b_high = b.card[0].face_val;
        if (a_high == POKEVAL_ACE && a.card[1].face_val == DH_CARD_FIVE)
          a_high = 5;
        if (b_high == POKEVAL_ACE && b.card[1].face_val == DH_CARD_FIVE)
          b_high = 5;
        if (a_high == b_high)
          tie = true;
        else
          b_wins = b_high > a_high;
        break;
      }
      case POKEVAL_FIVE_OF_A_KIND: {
        int a_quint = get_quint_value(&a);
        int b_quint = get_quint_value(&b);
        if (a_quint == b_quint)
          tie = true;
        else
          b_wins = b_quint > a_quint;
        break;
      }
      case POKEVAL_FOUR_OF_A_KIND: {
        int a_quad = get_quad_value(&a);
        int b_quad = get_quad_value(&b);

        int a_kicker = get_kicker_value(&a, a_quad);
        int b_kicker = get_kicker_value(&b, b_quad);

        if (a_quad == b_quad) {
          if (a_kicker == b_kicker)
            tie = true;
          else
            b_wins = b_kicker > a_kicker;
        } else {
          b_wins = b_quad > a_quad;
        }
        break;
      }
      case POKEVAL_FULL_HOUSE: {
        int a_trip = get_triplet_value(&a);
        int b_trip = get_triplet_value(&b);
        int a_pair = -1, b_pair = -1;
        for (int j = 0; j < POKEVAL_HAND_SIZE; ++j) {
          int val = a.card[j].face_val;
          if (val != a_trip && count_face(&a, val) == 2) {
            a_pair = val;
            break;
          }
        }
        for (int j = 0; j < POKEVAL_HAND_SIZE; ++j) {
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
      case POKEVAL_FLUSH:
      case POKEVAL_HIGH_CARD: {
        int cmp = compare_high_cards(&a, &b);
        if (cmp == 0)
          tie = true;
        else if (cmp < 0)
          b_wins = true;
        break;
      }
      case POKEVAL_THREE_OF_A_KIND: {
        int a_trip = get_triplet_value(&a);
        int b_trip = get_triplet_value(&b);
        if (a_trip == b_trip)
          tie = true;
        else
          b_wins = b_trip > a_trip;
        break;
      }
      case POKEVAL_TWO_PAIR: {
        int cmp = compare_two_pair_tiebreak(&a, &b);
        if (cmp == 0)
          tie = true;
        else if (cmp > 0)
          b_wins = true;
        break;
      }
      case POKEVAL_PAIR: {
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

POKEVAL_Hand_5 POKEVAL_hand5_from_hand7(const POKEVAL_Hand_7 *src) {
  // Fast path: It's a 5-card hand (cards 5 and/or 6 are DH_card_null)
  if (DH_is_card_null(src->card[5]) || DH_is_card_null(src->card[6])) {
    POKEVAL_Hand_5 dest;
    for (size_t i = 0; i < 5; ++i) {
      dest.card[i] = src->card[i];
    }
    return dest;
  }

  // Full 7-card evaluation for stud or hold'em
  POKEVAL_Hand_5 best_hand = {0};
  short best_rank = -1;
  DH_Card temp[5];

  for (size_t i = 0; i < 7; ++i) {
    for (size_t j = i + 1; j < 7; ++j) {
      // Build candidate hand by omitting cards i and j
      size_t k = 0;
      for (size_t m = 0; m < 7; ++m) {
        if (m != i && m != j) {
          temp[k++] = src->card[m];
        }
      }

      POKEVAL_Hand_5 candidate = {0};
      memcpy(candidate.card, temp, sizeof(temp));

      short rank = POKEVAL_evaluate_hand(candidate);

      if (rank > best_rank) {
        best_rank = rank;
        best_hand = candidate;
      }
      // Optional: exact tie-breaking could be added here
    }
  }

  return best_hand;
}

static inline int lowball_value(int face) { return (face == DH_CARD_ACE) ? 1 : face; }

void POKEVAL_sort_hand_lowball(POKEVAL_Hand_5 *hand) {
  for (int i = 0; i < POKEVAL_HAND_SIZE - 1; ++i) {
    for (int j = i + 1; j < POKEVAL_HAND_SIZE; ++j) {

      int val_i = lowball_value(hand->card[i].face_val);
      int val_j = lowball_value(hand->card[j].face_val);

      // ascending (low wins)
      if (val_i > val_j) {
        DH_Card tmp = hand->card[i];
        hand->card[i] = hand->card[j];
        hand->card[j] = tmp;
      }
    }
  }
}

static int compare_lowball_5(const POKEVAL_Hand_5 *a, const POKEVAL_Hand_5 *b) {
  // assumes both hands are sorted low → high with Ace = 1

  // 1) classify by duplicates
  int a_counts[15] = {0};
  int b_counts[15] = {0};

  for (int i = 0; i < 5; ++i) {
    a_counts[a->card[i].face_val]++;
    b_counts[b->card[i].face_val]++;
  }

  int a_max = 0, b_max = 0;
  for (int v = 1; v <= 14; ++v) {
    if (a_counts[v] > a_max)
      a_max = a_counts[v];
    if (b_counts[v] > b_max)
      b_max = b_counts[v];
  }

  // lower duplicate count is better
  if (a_max != b_max)
    return (a_max < b_max) ? -1 : 1;

  // 2) same class → compare card-by-card (low wins)
  for (int i = 4; i >= 0; --i) {
    int av = a->card[i].face_val;
    int bv = b->card[i].face_val;
    if (av != bv)
      return (av < bv) ? -1 : 1;
  }

  return 0; // tie
}

static uint8_t compare_hands_5_lowball(POKEVAL_NeedComparing *need_comparing, uint8_t count) {
  for (uint8_t i = 0; i < count; ++i) {
    POKEVAL_sort_hand_lowball(&need_comparing[i].hand_5);
    need_comparing[i].won = false;
  }

  assert(count > 0);
  uint8_t winner_indices[count];
  winner_indices[0] = 0;
  uint8_t num_winners = 0;

  POKEVAL_Hand_5 best_hand = need_comparing[0].hand_5;
  winner_indices[0] = 0;
  num_winners = 1;

  for (uint8_t i = 1; i < count; ++i) {
    int cmp = compare_lowball_5(&best_hand, &need_comparing[i].hand_5);

    if (cmp == 0) {
      winner_indices[num_winners++] = i;
    } else if (cmp > 0) {
      best_hand = need_comparing[i].hand_5;
      winner_indices[0] = i;
      num_winners = 1;
    }
  }

  for (uint8_t i = 0; i < num_winners; ++i) {
    need_comparing[winner_indices[i]].won = true;
  }

  return num_winners;
}

uint8_t POKEVAL_compare_hands(POKEVAL_NeedComparing *need_comparing, uint8_t count,
                              const bool lowball) {

  for (size_t i = 0; i < count; ++i) {
    need_comparing[i].hand_5 = POKEVAL_hand5_from_hand7(&need_comparing[i].hand);
  }
  return lowball == false ? compare_hands_5(need_comparing, count)
                          : compare_hands_5_lowball(need_comparing, count);
}
