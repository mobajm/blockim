/* tablet.c */
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

#include "errors.h"
#include "tablet.h"
#include "square.h"
#include "shape.h"
#include <stdlib.h>

static tablet_t* tablet_init(void);
static tablet_t* tablet_build(shape_t**);
static tablet_t* tablet_img_by(shape_t***, color_t, transform_t);

tablet_t* tablet_init(void)
{
  int i;
  
  tablet_t* tblt = malloc(sizeof *tblt);
  if (tblt == NULL) {
    ERR("Malloc error");
    return NULL;
  }

  tblt->sqrs = calloc(TBLT_SIZE, sizeof *(tblt->sqrs));
  for (i = 0; i < TBLT_SIZE; i++)
    tblt->sqrs[i] = calloc(TBLT_SIZE, sizeof **(tblt->sqrs));

  return tblt;
}

tablet_t** tablets_build(int*** shapes_arr)
{
  int i;
  shape_t** shapes;
  tablet_t** tablets = NULL;
  transform_t t;
  color_t tablets_col[NB_TABLETS] = {BLUE, YELLOW,
                                     RED, GREEN};

  tablets = malloc(NB_TABLETS * sizeof *tablets);
  if (tablets == NULL) {
    ERR("Malloc error");
    return NULL;
  }

  shapes = shapes_build(shapes_arr, tablets_col[0]);
  tablets[0] = tablet_build(shapes);

  t = REFLECT_V;
  for (i = 1; i < NB_TABLETS; i++) {
    tablets[i] = tablet_img_by(&shapes, tablets_col[i], t);
    t = t == REFLECT_V ? REFLECT_H : REFLECT_V;
  }

  return tablets;
}   

static tablet_t* tablet_build(shape_t** shapes)
{
  int i, j;
  
  tablet_t* tblt = tablet_init();

  for (i = 0; i < NB_KINDS; i++)
    tablet_place_shape(tblt, shapes[i]);

  for (i = 0; i < TBLT_SIZE; i++) {
    for (j = 0; j < TBLT_SIZE; j++) {
      if (tblt->sqrs[i][j] == NULL)
        tblt->sqrs[i][j] = square_create(BRDR_NONE, VOID, NULL);
    }
  }

  return tblt;
}

void tablet_place_shape(tablet_t* tbl, shape_t* sh)
{
  int x, y;
  int orig_x, orig_y;
  sqrs_queue_t* curr = NULL;
  
  orig_x = sh->tbl_x;
  orig_y = sh->tbl_y;
  curr = sh->head;
  while (curr != NULL) {
    x = orig_x + curr->dx;
    y = orig_y + curr->dy;
    tbl->sqrs[y][x] = curr->sq;
    curr = curr->next;
  }
}

static tablet_t* tablet_img_by(shape_t*** shapes, color_t col, transform_t t)
{
  int i;
  tablet_t* new_tbl = tablet_init();
  shape_t** shps = NULL;

  shps = malloc(NB_KINDS * sizeof *shps);
  if (shps == NULL) {
    ERR("Malloc error");
    return NULL;
  }

  for (i = 0; i < NB_KINDS; i++)
    shps[i] = shape_reflect((*shapes)[i], t, col);

  new_tbl = tablet_build(shps);
  *shapes = shps;

  return new_tbl;
}
