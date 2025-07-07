#include "00_test.h"

typedef struct {
  POKEVAL_Hand_7 hand;
  short expected_rank;
  const char *description;
} TestCase;

_MAIN_HEAD_

TestCase cases[] = {
    {
        .hand = {{
            {DH_CARD_QUEEN, DH_SUIT_HEARTS},
            {DH_CARD_QUEEN, DH_SUIT_SPADES},
            {DH_CARD_QUEEN, DH_SUIT_DIAMONDS},
            {DH_CARD_TEN, DH_SUIT_CLUBS},
            {DH_CARD_TEN, DH_SUIT_HEARTS},
            {DH_CARD_FOUR, DH_SUIT_SPADES},
            {DH_CARD_TWO, DH_SUIT_CLUBS},
        }},
        .expected_rank = POKEVAL_FULL_HOUSE,
        .description = "Full House: Queens over Tens",
    },
    {
        .hand = {{
            {DH_CARD_FIVE, DH_SUIT_CLUBS},
            {DH_CARD_SEVEN, DH_SUIT_CLUBS},
            {DH_CARD_NINE, DH_SUIT_CLUBS},
            {DH_CARD_TWO, DH_SUIT_DIAMONDS},
            {DH_CARD_SIX, DH_SUIT_CLUBS},
            {DH_CARD_KING, DH_SUIT_HEARTS},
            {DH_CARD_EIGHT, DH_SUIT_CLUBS},
        }},
        .expected_rank = POKEVAL_STRAIGHT_FLUSH,
        .description = "Straight Flush",
    },
    {
        .hand = {{
            {DH_CARD_ACE, DH_SUIT_SPADES},
            {DH_CARD_KING, DH_SUIT_HEARTS},
            {DH_CARD_QUEEN, DH_SUIT_CLUBS},
            {DH_CARD_JACK, DH_SUIT_SPADES},
            {DH_CARD_TEN, DH_SUIT_DIAMONDS},
            {DH_CARD_THREE, DH_SUIT_CLUBS},
            {DH_CARD_TWO, DH_SUIT_HEARTS},
        }},
        .expected_rank = POKEVAL_STRAIGHT,
        .description = "Ace-high Straight",
    },
    {
        .hand = {{
            {DH_CARD_THREE, DH_SUIT_SPADES},
            {DH_CARD_THREE, DH_SUIT_HEARTS},
            {DH_CARD_THREE, DH_SUIT_CLUBS},
            {DH_CARD_FIVE, DH_SUIT_DIAMONDS},
            {DH_CARD_EIGHT, DH_SUIT_CLUBS},
            {DH_CARD_QUEEN, DH_SUIT_SPADES},
            {DH_CARD_KING, DH_SUIT_HEARTS},
        }},
        .expected_rank = POKEVAL_THREE_OF_A_KIND,
        .description = "Three of a Kind: Threes",
    },
};

size_t num_cases = sizeof cases / sizeof cases[0];

for (size_t i = 0; i < num_cases; ++i) {
    POKEVAL_Hand_5 reduced = POKEVAL_hand5_from_hand7(&cases[i].hand);
    short actual_rank = POKEVAL_evaluate_hand(reduced);

    fprintf(stderr, "Test %zu: %s (Expected rank: %d, Got: %d)\n",
            i + 1, cases[i].description, cases[i].expected_rank, actual_rank);

    assert(actual_rank == cases[i].expected_rank);
}

_MAIN_TAIL_
