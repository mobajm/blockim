/* square.h */
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

#ifndef _SQUARE_TMP_H_
#define _SQUARE_TMP_H_

/* #include "shape.h" */

typedef enum {
  BRDR_NONE = 0,
  BRDR_U = 1,
  BRDR_D = 2,
  BRDR_L = 4,
  BRDR_R = 8
} border_t;

typedef enum { VOID = 0, BLUE, YELLOW, RED, GREEN, GRAY, NB_COLORS } color_t;

/* Possible transformations on squares and shapes */
typedef enum transform_e {
  REFLECT_V,
  REFLECT_H,
  NB_TRANSFORMS
} transform_t;

struct shape_t;
typedef struct square_s {
  /* A square can be linked to the shape
     he is part of. */
  struct shape_t* sh;
  color_t color;
  border_t borders;
} square_t;

square_t* square_init(void);
square_t* square_create(border_t, color_t, struct shape_t*);
void square_free(square_t*);

border_t borders_img(border_t, transform_t);

#endif /* _SQUARE_TMP_H_ */

