/* shape.h */
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

#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "square.h"

/* This is a queue of squares. Each square "nows" where its place
   relatively to the first square read when building the shape
   (left to right, top to bottom traversal). */
typedef struct sqrs_queue_s {
  square_t* sq;
  char dx, dy;
  struct sqrs_queue_s* next;
} sqrs_queue_t;

/* Each shape nows where its first square is to be placed
   on the matrix representing a tablet. */
typedef struct shape_t {
  sqrs_queue_t* head;
  sqrs_queue_t* last;
  char tbl_x, tbl_y;
  int **sh_tab;
  int nb_sqrs;
} shape_t;

/* Kinds of shapes */
typedef enum {
  BigI = 0,
  ShortU,
  Gun,
  BigL,
  OneLongLegZ,
  EvenL,
  Square,
  BigW,
  ShortI,
  OneSquare,
  LongZ,
  WeirdT,
  ShortZ,
  Plus,
  NormalL,
  TwoSquares,
  NormalT,
  ShortGun,
  ShortT,
  MediumI,
  Step,
  NB_KINDS
} shape_kind_t;

#define SHAPE_SIZE 5

shape_t** shapes_build(int***, color_t);
shape_t* shape_build_from_array(int**, int, int, color_t);
int*** shapes_read(void);
shape_t* shape_reflect(shape_t*, transform_t, color_t);
int shape_max_dx_gen(shape_t*);
int shape_max_dy(shape_t*);
int shape_min_dx(shape_t*);
int shape_min_dy(shape_t*);

#endif /* _SHAPE_H_ */

