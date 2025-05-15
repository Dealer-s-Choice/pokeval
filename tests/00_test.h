#include <deckhandler.h>
#include <pokeval.h>
#include <stdio.h>
#include <string.h>

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <assert.h>

#define _MAIN_HEAD_                                                                                \
  int main(int argc, char *argv[]) {                                                               \
    (void)argc;                                                                                    \
    (void)argv;

#define _MAIN_TAIL_                                                                                \
  return 0;                                                                                        \
  }

void set_hand(struct pokeval_hand_t *hand, int faces[HAND_SIZE], int suits[HAND_SIZE]);
