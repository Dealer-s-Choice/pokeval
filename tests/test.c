#include "00_test.h"

#define TEST_HAND(expected_rank, f, s)                                                             \
  do {                                                                                             \
    int faces[HAND_SIZE];                                                                          \
    int suits[HAND_SIZE];                                                                          \
    static struct pokeval_hand_t hand;                                                             \
    memcpy(faces, f, sizeof(faces));                                                               \
    memcpy(suits, s, sizeof(suits));                                                               \
    set_hand(&hand, faces, suits);                                                                 \
    short rank = pokeval_evaluate_hand(hand);                                                      \
    fprintf(stderr, "rank: %s\n", pokeval_ranks[rank]);                                            \
    assert(rank == expected_rank);                                                                 \
  } while (0)

static void test_static_hands(void) {
  int rf_faces[] = {DH_CARD_KING, DH_CARD_QUEEN, DH_CARD_ACE, DH_CARD_JACK, DH_CARD_TEN};
  int rf_suits[] = {DH_SUIT_HEARTS, DH_SUIT_HEARTS, DH_SUIT_HEARTS, DH_SUIT_HEARTS, DH_SUIT_HEARTS};
  TEST_HAND(ROYAL_FLUSH, rf_faces, rf_suits);

  int fh_faces[] = {DH_CARD_ACE, DH_CARD_ACE, DH_CARD_ACE, DH_CARD_JACK, DH_CARD_JACK};
  int fh_suits[] = {DH_SUIT_HEARTS, DH_SUIT_CLUBS, DH_SUIT_DIAMONDS, DH_SUIT_SPADES, DH_SUIT_HEARTS};
  TEST_HAND(FULL_HOUSE, fh_faces, fh_suits);

  int fl_faces[] = {DH_CARD_ACE, DH_CARD_THREE, DH_CARD_EIGHT, DH_CARD_JACK, DH_CARD_QUEEN};
  int fl_suits[] = {DH_SUIT_HEARTS, DH_SUIT_HEARTS, DH_SUIT_HEARTS, DH_SUIT_HEARTS, DH_SUIT_HEARTS};
  TEST_HAND(FLUSH, fl_faces, fl_suits);

  int st_faces[] = {DH_CARD_TWO, DH_CARD_FOUR, DH_CARD_THREE, DH_CARD_ACE, DH_CARD_FIVE};
  int st_suits[] = {DH_SUIT_HEARTS, DH_SUIT_HEARTS, DH_SUIT_CLUBS, DH_SUIT_HEARTS, DH_SUIT_SPADES};
  TEST_HAND(STRAIGHT, st_faces, st_suits);
}

_MAIN_HEAD_

int cases[] = {THREE_OF_A_KIND, NOTHING, PAIR, NOTHING, PAIR, PAIR, TWO_PAIR, PAIR};

DH_Deck deck = DH_get_new_deck();
DH_pcg_srand(1, 1);

int i;
for (size_t t = 0; t < sizeof cases / sizeof cases[0]; t++) {
  i = 0;
  DH_shuffle_deck(&deck);

  struct pokeval_hand_t hand;
  int k = 0;
  do {
    hand.card[k].suit = deck.card[i].suit;
    hand.card[k].face_val = deck.card[i].face_val;

    i++;
  } while (++k < HAND_SIZE);

  short rank = pokeval_evaluate_hand(hand);
  fprintf(stderr, "rank: %s\n", pokeval_ranks[rank]);
  assert(rank == cases[t]);
}

test_static_hands();

_MAIN_TAIL_
