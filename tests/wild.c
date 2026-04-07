#include "00_test.h"

#define TEST_WILD(expected_rank, f, s)                                                             \
  do {                                                                                             \
    int tw_faces[POKEVAL_HAND_SIZE];                                                               \
    int tw_suits[POKEVAL_HAND_SIZE];                                                               \
    static POKEVAL_Hand_5 hand;                                                                    \
    memcpy(tw_faces, f, POKEVAL_HAND_SIZE * sizeof(int));                                          \
    memcpy(tw_suits, s, POKEVAL_HAND_SIZE * sizeof(int));                                          \
    set_hand(&hand, tw_faces, tw_suits);                                                           \
    short rank = POKEVAL_evaluate_hand_wild(hand, DH_CARD_TWO);                                   \
    fprintf(stderr, "rank: %s\n", POKEVAL_rank[rank]);                                             \
    assert(rank == expected_rank);                                                                 \
  } while (0)

_MAIN_HEAD_

/* --- No wilds: should match regular evaluate_hand --- */
{
  int fh_f[] = {DH_CARD_ACE, DH_CARD_ACE, DH_CARD_KING, DH_CARD_KING, DH_CARD_KING};
  int fh_s[] = {DH_SUIT_HEARTS, DH_SUIT_CLUBS, DH_SUIT_HEARTS, DH_SUIT_CLUBS, DH_SUIT_DIAMONDS};
  TEST_WILD(POKEVAL_FULL_HOUSE, fh_f, fh_s);
}

/* --- 1 wild: pair + wild = three-of-a-kind --- */
{
  int f[] = {DH_CARD_KING, DH_CARD_KING, DH_CARD_TWO, DH_CARD_NINE, DH_CARD_THREE};
  int s[] = {DH_SUIT_HEARTS, DH_SUIT_CLUBS, DH_SUIT_SPADES, DH_SUIT_DIAMONDS, DH_SUIT_HEARTS};
  TEST_WILD(POKEVAL_THREE_OF_A_KIND, f, s);
}

/* --- 1 wild: three-of-a-kind + wild = four-of-a-kind --- */
{
  int f[] = {DH_CARD_QUEEN, DH_CARD_QUEEN, DH_CARD_QUEEN, DH_CARD_TWO, DH_CARD_FIVE};
  int s[] = {DH_SUIT_HEARTS, DH_SUIT_CLUBS, DH_SUIT_DIAMONDS, DH_SUIT_SPADES, DH_SUIT_HEARTS};
  TEST_WILD(POKEVAL_FOUR_OF_A_KIND, f, s);
}

/* --- 1 wild: four-of-a-kind + wild = five-of-a-kind --- */
{
  int f[] = {DH_CARD_JACK, DH_CARD_JACK, DH_CARD_JACK, DH_CARD_JACK, DH_CARD_TWO};
  int s[] = {DH_SUIT_HEARTS, DH_SUIT_CLUBS, DH_SUIT_DIAMONDS, DH_SUIT_SPADES, DH_SUIT_HEARTS};
  TEST_WILD(POKEVAL_FIVE_OF_A_KIND, f, s);
}

/* --- 1 wild: 4 suited consecutive + wild = straight flush --- */
{
  int f[] = {DH_CARD_FIVE, DH_CARD_SIX, DH_CARD_SEVEN, DH_CARD_EIGHT, DH_CARD_TWO};
  int s[] = {DH_SUIT_HEARTS, DH_SUIT_HEARTS, DH_SUIT_HEARTS, DH_SUIT_HEARTS, DH_SUIT_SPADES};
  TEST_WILD(POKEVAL_STRAIGHT_FLUSH, f, s);
}

/* --- 1 wild: A,K,Q,J same suit + wild = royal flush --- */
{
  int f[] = {DH_CARD_ACE, DH_CARD_KING, DH_CARD_QUEEN, DH_CARD_JACK, DH_CARD_TWO};
  int s[] = {DH_SUIT_SPADES, DH_SUIT_SPADES, DH_SUIT_SPADES, DH_SUIT_SPADES, DH_SUIT_HEARTS};
  TEST_WILD(POKEVAL_ROYAL_FLUSH, f, s);
}

/* --- 1 wild: 4 consecutive (off-suit) + wild = straight --- */
{
  int f[] = {DH_CARD_NINE, DH_CARD_TEN, DH_CARD_JACK, DH_CARD_QUEEN, DH_CARD_TWO};
  int s[] = {DH_SUIT_HEARTS, DH_SUIT_CLUBS, DH_SUIT_DIAMONDS, DH_SUIT_SPADES, DH_SUIT_HEARTS};
  TEST_WILD(POKEVAL_STRAIGHT, f, s);
}

/* --- 1 wild: 4 same suit (non-sequential) + wild = flush --- */
{
  int f[] = {DH_CARD_THREE, DH_CARD_SIX, DH_CARD_NINE, DH_CARD_QUEEN, DH_CARD_TWO};
  int s[] = {DH_SUIT_CLUBS, DH_SUIT_CLUBS, DH_SUIT_CLUBS, DH_SUIT_CLUBS, DH_SUIT_HEARTS};
  TEST_WILD(POKEVAL_FLUSH, f, s);
}

/* --- 2 wilds: pair + 2 wilds = four-of-a-kind --- */
{
  int f[] = {DH_CARD_TEN, DH_CARD_TEN, DH_CARD_TWO, DH_CARD_TWO, DH_CARD_FIVE};
  int s[] = {DH_SUIT_HEARTS, DH_SUIT_CLUBS, DH_SUIT_SPADES, DH_SUIT_DIAMONDS, DH_SUIT_HEARTS};
  TEST_WILD(POKEVAL_FOUR_OF_A_KIND, f, s);
}

/* --- 2 wilds: three-of-a-kind + 2 wilds = five-of-a-kind --- */
{
  int f[] = {DH_CARD_ACE, DH_CARD_ACE, DH_CARD_ACE, DH_CARD_TWO, DH_CARD_TWO};
  int s[] = {DH_SUIT_HEARTS, DH_SUIT_CLUBS, DH_SUIT_DIAMONDS, DH_SUIT_SPADES, DH_SUIT_HEARTS};
  TEST_WILD(POKEVAL_FIVE_OF_A_KIND, f, s);
}

/* --- 2 wilds: 3 suited consecutive + 2 wilds = straight flush --- */
{
  int f[] = {DH_CARD_THREE, DH_CARD_FOUR, DH_CARD_FIVE, DH_CARD_TWO, DH_CARD_TWO};
  int s[] = {DH_SUIT_DIAMONDS, DH_SUIT_DIAMONDS, DH_SUIT_DIAMONDS, DH_SUIT_HEARTS, DH_SUIT_HEARTS};
  TEST_WILD(POKEVAL_STRAIGHT_FLUSH, f, s);
}

/* --- 3 wilds: two different-value real cards + 3 wilds → four-of-a-kind --- */
{
  /* Real: K♥ and 7♥ are different, so can't be five-of-a-kind; best is four kings */
  int f[] = {DH_CARD_KING, DH_CARD_TWO, DH_CARD_TWO, DH_CARD_TWO, DH_CARD_SEVEN};
  int s[] = {DH_SUIT_HEARTS, DH_SUIT_SPADES, DH_SUIT_CLUBS, DH_SUIT_DIAMONDS, DH_SUIT_HEARTS};
  TEST_WILD(POKEVAL_FOUR_OF_A_KIND, f, s);
}

/* --- 3 wilds: two same-value real cards + 3 wilds = five-of-a-kind --- */
{
  int f[] = {DH_CARD_EIGHT, DH_CARD_EIGHT, DH_CARD_TWO, DH_CARD_TWO, DH_CARD_TWO};
  int s[] = {DH_SUIT_HEARTS, DH_SUIT_CLUBS, DH_SUIT_SPADES, DH_SUIT_DIAMONDS, DH_SUIT_HEARTS};
  TEST_WILD(POKEVAL_FIVE_OF_A_KIND, f, s);
}

/* --- 4 wilds + 1 real card = five-of-a-kind --- */
{
  int f[] = {DH_CARD_KING, DH_CARD_TWO, DH_CARD_TWO, DH_CARD_TWO, DH_CARD_TWO};
  int s[] = {DH_SUIT_HEARTS, DH_SUIT_HEARTS, DH_SUIT_CLUBS, DH_SUIT_DIAMONDS, DH_SUIT_SPADES};
  TEST_WILD(POKEVAL_FIVE_OF_A_KIND, f, s);
}

/* --- 7-card hand evaluator with wilds --- */
{
  /* 3 kings + 2 wilds (among 7 cards) → best 5-card hand is five-of-a-kind of kings */
  POKEVAL_Hand_7 h7 = {{{DH_CARD_KING, DH_SUIT_HEARTS},
                         {DH_CARD_KING, DH_SUIT_CLUBS},
                         {DH_CARD_KING, DH_SUIT_DIAMONDS},
                         {DH_CARD_TWO, DH_SUIT_SPADES},
                         {DH_CARD_TWO, DH_SUIT_HEARTS},
                         {DH_CARD_NINE, DH_SUIT_CLUBS},
                         {DH_CARD_THREE, DH_SUIT_DIAMONDS}}};
  POKEVAL_Hand_5 best = POKEVAL_hand5_from_hand7_wild(&h7, DH_CARD_TWO);
  short rank = POKEVAL_evaluate_hand_wild(best, DH_CARD_TWO);
  fprintf(stderr, "7-card wild best: %s\n", POKEVAL_rank[rank]);
  assert(rank == POKEVAL_FIVE_OF_A_KIND);
}

{
  /* 4 suited consecutive + 1 wild in 7-card hand → should find straight flush */
  POKEVAL_Hand_7 h7 = {{{DH_CARD_FIVE, DH_SUIT_HEARTS},
                         {DH_CARD_SIX, DH_SUIT_HEARTS},
                         {DH_CARD_SEVEN, DH_SUIT_HEARTS},
                         {DH_CARD_EIGHT, DH_SUIT_HEARTS},
                         {DH_CARD_TWO, DH_SUIT_SPADES},
                         {DH_CARD_QUEEN, DH_SUIT_CLUBS},
                         {DH_CARD_THREE, DH_SUIT_DIAMONDS}}};
  POKEVAL_Hand_5 best = POKEVAL_hand5_from_hand7_wild(&h7, DH_CARD_TWO);
  short rank = POKEVAL_evaluate_hand_wild(best, DH_CARD_TWO);
  fprintf(stderr, "7-card straight flush wild: %s\n", POKEVAL_rank[rank]);
  assert(rank == POKEVAL_STRAIGHT_FLUSH);
}

_MAIN_TAIL_
