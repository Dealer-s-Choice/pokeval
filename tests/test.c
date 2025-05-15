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
  int rf_faces[] = {KING, QUEEN, ACE, JACK, TEN};
  int rf_suits[] = {HEARTS, HEARTS, HEARTS, HEARTS, HEARTS};
  TEST_HAND(ROYAL_FLUSH, rf_faces, rf_suits);

  int fh_faces[] = {ACE, ACE, ACE, JACK, JACK};
  int fh_suits[] = {HEARTS, CLUBS, DIAMONDS, SPADES, HEARTS};
  TEST_HAND(FULL_HOUSE, fh_faces, fh_suits);

  int fl_faces[] = {ACE, THREE, EIGHT, JACK, QUEEN};
  int fl_suits[] = {HEARTS, HEARTS, HEARTS, HEARTS, HEARTS};
  TEST_HAND(FLUSH, fl_faces, fl_suits);

  int st_faces[] = {TWO, FOUR, THREE, ACE, FIVE};
  int st_suits[] = {HEARTS, HEARTS, CLUBS, HEARTS, SPADES};
  TEST_HAND(STRAIGHT, st_faces, st_suits);
}

_MAIN_HEAD_

int cases[] = {THREE_OF_A_KIND, NOTHING, PAIR, NOTHING, PAIR, PAIR, TWO_PAIR, PAIR};

struct dh_deck deck;
dh_init_deck(&deck);
dh_pcg_srand(1, 1);

int i;
for (size_t t = 0; t < sizeof cases / sizeof cases[0]; t++) {
  i = 0;
  dh_shuffle_deck(&deck);

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
