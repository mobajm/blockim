/* main.c */
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
 * @Brief Main file of the project, puts the pieces together.
 */

#include "errors.h"
#include "shape.h"
#include "tablet.h"
#include "board.h"
#include "square.h"
#include "graphic.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>

int main(void)
{
  SDL_Surface* screen = NULL;
  int*** shapes_arr = NULL;
  board_t* board = NULL;
  tablet_t** tablets = NULL;

  /* read the shapes from the files */
  shapes_arr = shapes_read();

  /* build the tablets */
  tablets = tablets_build(shapes_arr);
  
  /* build the game board */
  board = board_init();
  if (board == NULL)
    exit(EXIT_FAILURE);

  /* Fire up SDL, draw everything and let the game begin */
  screen = init_sdl();
  if (screen == NULL)
    exit(EXIT_FAILURE);
  run_game(screen, board, tablets);

  return 0;
}
