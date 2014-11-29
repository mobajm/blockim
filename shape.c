/* shape.c */
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
 * @Brief Shapes are the major components of the game. They can be moved,
          reflected, translated, etc.
 */

#include "errors.h"
#include "shape.h"
#include "square.h"
#include "tablet.h"
#include <stdlib.h>
#include <stdio.h>

/* Static functions prototypes */
static int** read_shape(shape_kind_t k);

/* map: kind -> place on tablet */
tablet_spot_t shapes_spots[NB_KINDS] = {
  {3, 8}, {0, 11}, {1, 8}, {0, 7},
  {3, 6}, {0, 4}, {1, 4}, {4, 3},
  {0, 1}, {1, 2}, {2, 1}, {4, 1},
  {7, 2}, {6, 0}, {9, 1}, {11, 3},
  {10, 1}, {10, 0}, {7, 0}, {2, 0},
  {0, 0}
};
    
static shape_t* shape_init(char x, char y)
{
  shape_t* shp = malloc(sizeof *shp);
  if (shp == NULL) {
    ERR("Malloc error");
    return NULL;
  }
  
  shp->head = shp->last = NULL;
  shp->tbl_x = x;
  shp->tbl_y = y;
  shp->nb_sqrs = 0;
  return shp;
}

static void shape_add_sqr(shape_t* shp, square_t* sq,
                          char dx, char dy)
{
  sqrs_queue_t* new_sq = malloc(sizeof *new_sq);
  if (new_sq == NULL) {
    ERR("Malloc error");
    return;
  }

  new_sq->sq = sq;
  new_sq->dx = dx;
  new_sq->dy = dy;
  new_sq->next = NULL;

  if (shp->head == NULL && shp->last == NULL) {
    shp->head = new_sq;
    shp->last = new_sq;
  }
  else
    shp->last->next = new_sq;
  shp->last = new_sq;
  shp->nb_sqrs++;
}

shape_t** shapes_build(int*** shapes_arrs, color_t col)
{
  shape_t** shapes = NULL;
  int i;

  shapes = malloc(NB_KINDS * sizeof *shapes);
  if (shapes == NULL) {
    ERR("Malloc error");
    return NULL;
  }
  
  for (i = BigI; i < NB_KINDS; i++)
    shapes[i] = shape_build_from_array(shapes_arrs[i], shapes_spots[i].x,
                                       shapes_spots[i].y, col);;

  return shapes;
}

int*** shapes_read(void)
{
  int i;

  int*** shps_arrs = malloc(NB_KINDS * sizeof *shps_arrs);
  for (i = 0; i < NB_KINDS; i++)
    shps_arrs[i] = read_shape(i);

  return shps_arrs;
}

static int** read_shape(shape_kind_t k)
{
#define SHAPES_PATT "shape/shape%d.txt"
  FILE* sh_f = NULL;
  int c, i, j;
  char path[30];
  int** shape_arr = NULL;

  shape_arr = calloc(SHAPE_SIZE, sizeof *shape_arr);
  if (shape_arr == NULL) {
    ERR("Malloc error");
    return NULL;
  }
  for (i = 0; i < SHAPE_SIZE; i++)
    shape_arr[i] = calloc(SHAPE_SIZE, sizeof **shape_arr);

  /* read in the squares */
  sprintf(path, SHAPES_PATT, k);
  sh_f = fopen(path, "r");
  if (sh_f == NULL) {
    ERR("Unable to open shape file");
    return NULL;
  }

  i = j = 0;
  while ((c = fgetc(sh_f)) != EOF
         && i < SHAPE_SIZE && j <= SHAPE_SIZE) {
    if (c == '\n') { i++; j = 0; }
    else if (c == '0' || c == '1') {
      shape_arr[i][j] = c - '0';
      j++;
    }
  }
  fclose(sh_f);

  return shape_arr;
}

shape_t* shape_build_from_array(int** arr, int spot_x, int spot_y, color_t col)
{
  shape_t* sh = NULL;
  int is_head, head_x, head_y;
  int dx, dy;
  border_t bord;
  square_t* sq = NULL;
  int i, j;

  /* init the shape. where will it be placed on the tablet? */
  sh = shape_init(spot_x, spot_y);
  sh->sh_tab = arr;
    
  is_head = 1;
  head_x = head_y = 0;
  for (i = 0; i < SHAPE_SIZE; i++) {
    for (j = 0; j < SHAPE_SIZE; j++) {
      if (arr[i][j] == 1) {
        /* First square encountered? store its coords */
        if (is_head) {
          is_head = 0;
          head_x = j;
          head_y = i;
          dx = dy = 0;
        } else {
          dx = j - head_x;
          dy = i - head_y;
        }
        /* Does this square have any borders? */
        bord = BRDR_NONE;
        if (i == 0 || arr[i-1][j] == 0)
          bord |= BRDR_U;
        if (i == SHAPE_SIZE-1 || arr[i+1][j] == 0)
          bord |= BRDR_D;
        if (j == 0 || arr[i][j-1] == 0)
          bord |= BRDR_L;
        if (j == SHAPE_SIZE-1 || arr[i][j+1] == 0)
          bord |= BRDR_R;
       
        sq = square_create(bord, col, sh);
        shape_add_sqr(sh, sq, dx, dy);
      }
    }
  }
  return sh;
}

static int shape_max_dx(shape_t* sh)
{
  int max = sh->head->dx;
  sqrs_queue_t* curr = sh->head->next;
  while (curr != NULL) {
    if (curr->dy == 0 && curr->dx > max)
      max = curr->dx;
    curr = curr->next;
  }
  return max;
}

int shape_min_dx(shape_t* sh)
{
  int min = sh->head->dx;
  sqrs_queue_t* curr = sh->head->next;
  while (curr != NULL) {
    if (curr->dx < min)
      min = curr->dx;
    curr = curr->next;
  }
  return min;
}

int shape_max_dx_gen(shape_t* sh)
{
  int max = sh->head->dx;
  sqrs_queue_t* curr = sh->head->next;
  while (curr != NULL) {
    if (curr->dx > max)
      max = curr->dx;
    curr = curr->next;
  }
  return max;
}

int shape_max_dy(shape_t* sh)
{
  int max = sh->head->dy;
  sqrs_queue_t* curr = sh->head->next;
  while (curr != NULL) {
    if (curr->dy > max)
      max = curr->dy;
    curr = curr->next;
  }
  return max;
}

int shape_min_dy(shape_t* sh)
{
  int min = sh->head->dx;
  sqrs_queue_t* curr = sh->head->next;
  while (curr != NULL) {
    if (curr->dy < min)
      min = curr->dy;
    curr = curr->next;
  }
  return min;
}

/**
 * This function returns the lowest x-coordinate
 * of the squares that are the farthest from the
 * head of the shape. It's used in the reflection
 * by the horizontal axis.
 */
static int min_x_max_y(shape_t* sh)
{
  sqrs_queue_t* curr = NULL;
  int max_y = shape_max_dy(sh);
  int min = SHAPE_SIZE;

  if (max_y == 0)
    return 0;

  curr = sh->head->next;
  while (curr != NULL) {
    if (curr->dy == max_y && curr->dx < min)
      min = curr->dx;
    curr = curr->next;
  }
  return min;
}

shape_t* shape_reflect(shape_t* sh, transform_t t, color_t col)
{
  int i, j, k;
  int** new_arr = NULL;
  int** arr;
  int spot_x, spot_y;
  
  new_arr = calloc(SHAPE_SIZE, sizeof *new_arr);
  if (new_arr == NULL) {
    ERR("Malloc error");
    return NULL;
  }
  for (i = 0; i < SHAPE_SIZE; i++)
    new_arr[i] = calloc(SHAPE_SIZE, sizeof **new_arr);

  spot_x = spot_y = 0;
  arr = sh->sh_tab;
  if (t == REFLECT_V) {
    spot_x = (TBLT_SIZE-1) - sh->tbl_x - shape_max_dx(sh);
    spot_y = sh->tbl_y;
    for (k = 0; k < SHAPE_SIZE; k++) {
      for (i = 0, j = SHAPE_SIZE-1; i <= j; i++, j--) {
        new_arr[k][i] = arr[k][j];
        new_arr[k][j] = arr[k][i];
      }
    }
  } else if (t == REFLECT_H) {
    spot_y = (TBLT_SIZE-1) - sh->tbl_y - shape_max_dy(sh);
    spot_x = sh->tbl_x + min_x_max_y(sh);
    for (i = 0, j = SHAPE_SIZE-1; i <= j; i++, j--) {
      for (k = 0; k < SHAPE_SIZE; k++) {
        new_arr[i][k] = arr[j][k];
        new_arr[j][k] = arr[i][k];
      }
    }
  }

  return shape_build_from_array(new_arr, spot_x, spot_y, col);
}
  
