#include "00_test.h"

_MAIN_HEAD_

// --- POKEVAL_score_visible_cards ---

// n=1: higher face value scores higher
DH_Card ace_h  = {DH_CARD_ACE,   DH_SUIT_HEARTS};
DH_Card king_h = {DH_CARD_KING,  DH_SUIT_HEARTS};
DH_Card nine_s = {DH_CARD_NINE,  DH_SUIT_SPADES};

assert(POKEVAL_score_visible_cards(&ace_h,  1) > POKEVAL_score_visible_cards(&king_h, 1));
assert(POKEVAL_score_visible_cards(&king_h, 1) > POKEVAL_score_visible_cards(&nine_s, 1));

// n=2 vs n=1: kicker (extra card) beats a missing card even if the kicker is low.
// Rule: "7-3 beats a 7 by itself"
DH_Card seven_three[2] = {{DH_CARD_SEVEN, DH_SUIT_CLUBS}, {DH_CARD_THREE, DH_SUIT_DIAMONDS}};

assert(POKEVAL_score_visible_cards(seven_three, 2) >
       POKEVAL_score_visible_cards(seven_three, 1));

// Pair (n=4) beats single high card (n=1) — stud-range path
DH_Card pair_nines[4] = {
    {DH_CARD_NINE,  DH_SUIT_CLUBS},
    {DH_CARD_NINE,  DH_SUIT_HEARTS},
    {DH_CARD_TWO,   DH_SUIT_DIAMONDS},
    {DH_CARD_FIVE,  DH_SUIT_SPADES},
};
assert(POKEVAL_score_visible_cards(pair_nines, 4) >
       POKEVAL_score_visible_cards(&ace_h, 1));

// n=5 (full-eval path): pair beats ace-high
DH_Card pair5[5] = {
    {DH_CARD_NINE,  DH_SUIT_CLUBS},
    {DH_CARD_NINE,  DH_SUIT_HEARTS},
    {DH_CARD_ACE,   DH_SUIT_SPADES},
    {DH_CARD_KING,  DH_SUIT_HEARTS},
    {DH_CARD_TWO,   DH_SUIT_DIAMONDS},
};
DH_Card high5[5] = {
    {DH_CARD_ACE,   DH_SUIT_SPADES},
    {DH_CARD_KING,  DH_SUIT_HEARTS},
    {DH_CARD_QUEEN, DH_SUIT_CLUBS},
    {DH_CARD_JACK,  DH_SUIT_DIAMONDS},
    {DH_CARD_NINE,  DH_SUIT_CLUBS},
};
assert(POKEVAL_score_visible_cards(pair5, 5) > POKEVAL_score_visible_cards(high5, 5));

// n=7 (best-5-from-7 path): three-of-a-kind beats two pair
DH_Card trips7[7] = {
    {DH_CARD_KING,  DH_SUIT_CLUBS},
    {DH_CARD_KING,  DH_SUIT_HEARTS},
    {DH_CARD_KING,  DH_SUIT_SPADES},
    {DH_CARD_TWO,   DH_SUIT_DIAMONDS},
    {DH_CARD_FIVE,  DH_SUIT_HEARTS},
    {DH_CARD_SEVEN, DH_SUIT_CLUBS},
    {DH_CARD_TEN,   DH_SUIT_SPADES},
};
DH_Card twopair7[7] = {
    {DH_CARD_ACE,   DH_SUIT_CLUBS},
    {DH_CARD_ACE,   DH_SUIT_HEARTS},
    {DH_CARD_QUEEN, DH_SUIT_SPADES},
    {DH_CARD_QUEEN, DH_SUIT_DIAMONDS},
    {DH_CARD_TWO,   DH_SUIT_HEARTS},
    {DH_CARD_FIVE,  DH_SUIT_CLUBS},
    {DH_CARD_SEVEN, DH_SUIT_SPADES},
};
assert(POKEVAL_score_visible_cards(trips7, 7) > POKEVAL_score_visible_cards(twopair7, 7));

// n=5 flush ranks above n=4 straight (flush needs 5 cards, straight needs 5 cards)
// A 4-card sequence is not a straight — must be scored as high card only.
// So 5-card flush > 4-card "almost straight"
DH_Card flush5[5] = {
    {DH_CARD_TWO,   DH_SUIT_HEARTS},
    {DH_CARD_FIVE,  DH_SUIT_HEARTS},
    {DH_CARD_SEVEN, DH_SUIT_HEARTS},
    {DH_CARD_NINE,  DH_SUIT_HEARTS},
    {DH_CARD_JACK,  DH_SUIT_HEARTS},
};
DH_Card almost_straight4[4] = {
    {DH_CARD_NINE,  DH_SUIT_CLUBS},
    {DH_CARD_TEN,   DH_SUIT_SPADES},
    {DH_CARD_JACK,  DH_SUIT_DIAMONDS},
    {DH_CARD_QUEEN, DH_SUIT_HEARTS},
};
assert(POKEVAL_score_visible_cards(flush5, 5) >
       POKEVAL_score_visible_cards(almost_straight4, 4));

// n=0 returns 0
assert(POKEVAL_score_visible_cards(NULL, 0) == 0);

_MAIN_TAIL_
