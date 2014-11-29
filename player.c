#include "errors.h"
#include "tablet.h"
#include "player.h"
#include <stdlib.h>

static tablet_spot_t initial_possible[NB_PLAYERS] = {
  {0, 0},
  {0, BRD_NB_SQRS - 1},
  {BRD_NB_SQRS - 1, BRD_NB_SQRS - 1},
  {BRD_NB_SQRS - 1, 0}
};

player_t** players_init(void)
{
  int i;
  player_t** players = malloc(NB_PLAYERS * sizeof *players);
  if (players == NULL) {
    ERR("Malloc error");
    return NULL;
  }

  for (i = 0; i < NB_PLAYERS; i++)
    players[i] = malloc(sizeof **players);
  
  for (i = 0; i < NB_PLAYERS; i++) {
    players[i]->num = i;
    players[i]->first = 1;
    players[i]->score = 0;
    players[i]->still_playing = 1;
    /* All squares are unavailable at first */
    memset(players[i]->can_play, 0,
           BRD_NB_SQRS*BRD_NB_SQRS*sizeof(unsigned char));
    /* Except the corners of the board */
    players[i]->can_play[initial_possible[i].x][initial_possible[i].y] = 1;
  }

  return players;
}
