/* square.c */
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
 * @Brief Handles all the square related logic. The square is the basic
          component of the shapes and the central board.
 */

#include "errors.h"
#include "shape.h"
#include "square.h"

square_t* square_init(void)
{
  square_t* sq = malloc(sizeof *sq);
  if (sq == NULL) {
    ERR("Malloc error");
    return NULL;
  }

  sq->color = GRAY;
  sq->borders = BRDR_U | BRDR_D | BRDR_L | BRDR_R;
  sq->sh = NULL;
  return sq;
}

square_t* square_create(border_t b, color_t col, struct shape_t* shape)
{
  square_t* sq = malloc(sizeof *sq);
  if (sq == NULL) {
    ERR("Malloc error");
    return NULL;
  }

  sq->color = col;
  sq->borders = b;
  sq->sh = shape;
  return sq;
}

void square_free(square_t* sq)
{
  free(sq);
}

border_t borders_img(border_t bord, transform_t t)
{
  border_t new_bord = BRDR_NONE;

  switch (t) {
    case REFLECT_V:
      if (bord & BRDR_L) new_bord |= BRDR_R;
      if (bord & BRDR_R) new_bord |= BRDR_L;
      if (bord & BRDR_U) new_bord |= BRDR_U;
      if (bord & BRDR_D) new_bord |= BRDR_D;
      break;

    case REFLECT_H:
      if (bord & BRDR_L) new_bord |= BRDR_L;
      if (bord & BRDR_R) new_bord |= BRDR_R;
      if (bord & BRDR_U) new_bord |= BRDR_D;
      if (bord & BRDR_D) new_bord |= BRDR_U;
      break;

    default: break;
  }

  return new_bord;
}
