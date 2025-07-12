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

#ifndef __POKEVAL_H
#define __POKEVAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

#include "deckhandler.h"

#define POKEVAL_HAND_SIZE 5

#define POKEVAL_ACE (DH_CARD_KING + 1)

typedef enum {
  POKEVAL_HIGH_CARD,
  POKEVAL_PAIR,
  POKEVAL_TWO_PAIR,
  POKEVAL_THREE_OF_A_KIND,
  POKEVAL_STRAIGHT,
  POKEVAL_FLUSH,
  POKEVAL_FULL_HOUSE,
  POKEVAL_FOUR_OF_A_KIND,
  POKEVAL_STRAIGHT_FLUSH,
  POKEVAL_ROYAL_FLUSH,
  POKEVAL_FIVE_OF_A_KIND,
  NUM_HAND_RANKS
} hand_rank_t;

extern const char *POKEVAL_rank[NUM_HAND_RANKS];

typedef struct {
  DH_Card card[5];
} POKEVAL_Hand_5;

typedef struct {
  DH_Card card[7];
} POKEVAL_Hand_7;

typedef struct {
  bool won;
  int8_t id;
  POKEVAL_Hand_7 hand;
  POKEVAL_Hand_5 hand_5;
} POKEVAL_NeedComparing;

void sort_hand(POKEVAL_Hand_5 *hand);

short POKEVAL_evaluate_hand(POKEVAL_Hand_5 hand);

// Returns the number of winners and fills `winners` with their indices.
// `winners` must have at least `count` elements allocated by the caller.
uint8_t POKEVAL_compare_hands(POKEVAL_NeedComparing *hands, const uint8_t count);

POKEVAL_Hand_5 POKEVAL_hand5_from_hand7(const POKEVAL_Hand_7 *src);

#ifdef __cplusplus
}
#endif
#endif
