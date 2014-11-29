/* board.c */
/* Copyright 2013 */
/* Mustapha Aouimar <stephano.spirit@gmail.com> */
/* Moncef <mob> Baazet <mob.ajm@gmail.com> */

/* This file is part of blockim */

/* All rights reserved. */

/* Redistribution and use in source and binary forms, with or without */
/* modification, are permitted provided that the following conditions are */
/* met: */

/* 1. Redistributions of source code must retain the above copyright */
/*    notice, this list of conditions and the following disclaimer. */
/* 2. Redistributions in binary form must reproduce the above copyright */
/*    notice, this list of conditions and the following disclaimer in the */
/*    documentation and/or other materials provided with the */
/*    distribution. */

/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR */
/* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY */
/* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE */
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

/**
 * @Brief Implements the logic of the game's central board (the big gray
          matrix in the middle of your screen)
 */

#include "errors.h"
#include "board.h"
#include "square.h"
#include "shape.h"
#include "player.h"
#include <SDL/SDL.h>
#include <stdlib.h>

board_t* board_init(void)
{
  int i, j;
  board_t* b = malloc(sizeof *b);
  if (b == NULL) {
    ERR("Malloc error");
    return NULL;
  }

  b->w = b->h = BRD_NB_SQRS;
  for (i = 0; i < b->h; i++) {
    for (j = 0; j < b->w; j++)
      b->sqrs[i][j] = square_init();
  }

  return b;
}

struct pos_t { int x, y; };
struct pos_t corners[4] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
struct pos_t sides[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

static int valid_coord(int x, int y)
{
  return x >= 0 && x < BRD_NB_SQRS
      && y >= 0 && y < BRD_NB_SQRS;
}

static void update_playable_squares(shape_t*, player_t*, int, int);
int board_place_shape(board_t* brd, shape_t* sh, int x, int y, struct player_t** players, int p)
{
  int max_dx = shape_max_dx_gen(sh),
      max_dy = shape_max_dy(sh),
      min_dx = shape_min_dx(sh),
      min_dy = shape_min_dy(sh);
  sqrs_queue_t* curr = sh->head;
  int k;
  int curr_x, curr_y;
  struct player_t* pl = players[p];

  /* Are we placing any square outside the board? */
  if (x + max_dx >= brd->w
      || y + max_dy >= brd->h
      || x + min_dx < 0
      || y + min_dy < 0)
    return 0;

  /* Is this position possible to play for this player? */
  while (curr != NULL) {
    for (k = 0; k < 4; k++) {
      curr_x = x + curr->dx + sides[k].x;
      curr_y = y + curr->dy + sides[k].y;
      
      if (valid_coord(curr_x, curr_y))
        if (pl->can_play[curr_y][curr_x] == pl->num + 2)
          return 0;
    }
    if (pl->can_play[y+curr->dy][x+curr->dx] >= 2)
      return 0;
    curr = curr->next;
  }

  curr = sh->head;
  while (curr != NULL) {
    if (pl->can_play[y+curr->dy][x+curr->dx] == 1)
      break;
    curr = curr->next;
  }

  if (curr != NULL) {
    curr = sh->head;
    while (curr != NULL) {
      /* It is. We setup the square, make it unplayable, document
         its owner (pl->num), and make the surroundind squares
         playable. */
      brd->sqrs[y + curr->dy][x + curr->dx] = curr->sq;

      for (k = 0; k < NB_PLAYERS; k++)
        players[k]->can_play[y+curr->dy][x+curr->dx] = pl->num + 2;
      
      curr = curr->next;
    }
    update_playable_squares(sh, pl, x, y);
    pl->score += sh->nb_sqrs;
    return 1;
  }
  return 0;
}

static void update_playable_squares(shape_t* sh, player_t* pl, int x, int y)
{
  int is_head = 1;
  int dx, dy, head_x = 0, head_y = 0;
  int i, j, k, l;
  int curr_x, curr_y;

  for (i = 0; i < SHAPE_SIZE; i++) {
    for (j = 0; j < SHAPE_SIZE; j++) {
      if (sh->sh_tab[i][j] == 1) {
        if (is_head) {
          dx = dy = 0;
          head_x = j;
          head_y = i;
          is_head = 0;
        } else {
          dx = j - head_x;
          dy = i - head_y;
        }
        /* For each square at a corner of the current one */
        for (k = 0; k < 4; k++) {
          curr_x = x + dx + corners[k].x;
          curr_y = y + dy + corners[k].y;
          if (valid_coord(curr_x, curr_y)
              && pl->can_play[curr_y][curr_x] == 0) {
            /* The coordinates of the square are valid
               and it's free. Check if its sides are free. */
            for (l = 0; l < 4; l++)
              if (pl->can_play[curr_y + sides[l].y][curr_x + sides[l].x] >= 2)
                break;

            /* All's good, go on */
            if (l >= 4) {
              pl->can_play[curr_y][curr_x] = 1;
            }
          }
        }

        for (l = 0; l < 4; l++) {
          curr_x = x + dx + sides[l].x;
          curr_y = y + dy + sides[l].y;
          if (valid_coord(curr_x, curr_y)
              && pl->can_play[curr_y][curr_x] == 1)
            pl->can_play[curr_y][curr_x] = 0;
        }
      }
    }
  }
}
