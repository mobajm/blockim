#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "board.h"

/* A player has a score. We also need to know if it's
   his first turn, and if he's still playing. */
typedef struct player_t {
  char num;
  unsigned char first;
  unsigned char score;
  unsigned char still_playing;
  unsigned char can_play[BRD_NB_SQRS][BRD_NB_SQRS];
} player_t;

#define NB_PLAYERS 4

player_t** players_init(void);

#endif /* _PLAYER_H_ */
