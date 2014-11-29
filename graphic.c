/* graphic.c */
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
 * @Brief Home of the graphic and drawing primitives
 */

#include "errors.h"
#include "graphic.h"
#include "board.h"
#include "shape.h"
#include "tablet.h"
#include "player.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <math.h>

/**
 * Macros of general use
 */
/* Size of a single square (pixels) */
#define SQR_SIZE 25
/* The tablets are drawn at this margin from the window */
#define TBLT_SMALL_MARGIN 20
#define TBLT_BIG_MARGIN BRD_MARGIN + SQR_SIZE * 12 + 10
/* The board is drawn at a certain top and left margin. */
/* #define BRD_MARGIN(size) (((size) - (SQR_SIZE) * (BRD_NB_SQRS)) / 2) */
#define BRD_MARGIN ((TBLT_SMALL_MARGIN) + 5 * (SQR_SIZE) + 10)
/* END macros of general use */

static struct tablets_pos_t {
  int orig_x;
  int orig_y;
} tablets_pos[NB_TABLETS] = {
  {TBLT_SMALL_MARGIN, TBLT_SMALL_MARGIN},
  {TBLT_BIG_MARGIN, TBLT_SMALL_MARGIN},
  {TBLT_BIG_MARGIN, TBLT_BIG_MARGIN},
  {TBLT_SMALL_MARGIN, TBLT_BIG_MARGIN}
};

/* map: color -> file */
const char* sqrs_tiles[NB_COLORS] = {"imgs/void.png", "imgs/blue.png", "imgs/yellow.png", "imgs/red.png",
                                     "imgs/green.png", "imgs/gray.png"};

const char* numbers_tiles[10] = {"imgs/0.png", "imgs/1.png", "imgs/2.png", "imgs/3.png", "imgs/4.png",
                                 "imgs/5.png", "imgs/6.png", "imgs/7.png", "imgs/8.png", "imgs/9.png"};

/* When a shape is clicked, it's convenient to know
   in which tablet it's located and its position */
typedef struct moving_shape_t {
  shape_t* orig_sh;                     /* In case of a reflection */
  shape_t* sh;
  int tblt_nb;
  int tblt_x, tblt_y;
} moving_shape_t;

/* is a shape under the cursor and what is it */
typedef struct mouse_state_s {
  char has_shape;
  moving_shape_t* msh;
} mouse_state_t;

/* Static functions prototypes */
static void handle_keydown(int, int*, mouse_state_t*);
static void setup_background(SDL_Surface*);
static void draw_board(SDL_Surface*, board_t*);
static void draw_square(SDL_Surface*, square_t*, SDL_Rect*);
static void draw_border(SDL_Surface*, square_t*, SDL_Rect*);
static void draw_tablet(SDL_Surface*, tablet_t*, int, int);
static void draw_tablets(SDL_Surface*, tablet_t**);
static void draw_shape(SDL_Surface*, shape_t*, int, int);
static mouse_state_t* init_mouse_state(void);
static void handle_button_down(SDL_Surface*, SDL_MouseButtonEvent*, tablet_t**, mouse_state_t*, board_t*, int*);
static moving_shape_t* get_clicked_shape(int, int, tablet_t*, int);
static moving_shape_t* new_moving_shape(shape_t*, int, int, int);
static void draw_moving_shape(SDL_Surface*, mouse_state_t*);
static int is_click_inboard(int, int);
static void draw_scores(SDL_Surface*);


SDL_Surface* init_sdl(void)
{
  SDL_Surface* screen = NULL;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_ERR("Error initializing SDL");
    return NULL;
  }
  atexit(SDL_Quit);

#define WINDOW_WIDTH 0
#define WINDOW_HEIGHT 0
  screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 0,
                            SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
  if (screen == NULL) {
    SDL_ERR("Unable to set video mode");
    return NULL;
  }

  SDL_WM_SetCaption("Blockim", "./icon.ico");

  return screen;
}

/* Keep track of the mouse position */
static int mouse_x, mouse_y;
/* Who's playing right now */
player_t** players;
int player = 0;
void run_game(SDL_Surface* screen, board_t* brd, tablet_t** tbl)
{
  int running;
  SDL_Event event;
  mouse_state_t* mouse_st = init_mouse_state();

  players = players_init();

  running = 1;
  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: running = 0; break;
          
        case SDL_KEYDOWN:
          handle_keydown(event.key.keysym.sym, &running, mouse_st);
          break;
          
        case SDL_MOUSEBUTTONDOWN:
          handle_button_down(screen, &event.button, tbl, mouse_st, brd, &running);
          break;
          
        case SDL_MOUSEMOTION:
          mouse_x = event.motion.x;
          mouse_y = event.motion.y;
          break;

        default: break;
      }
    }
    setup_background(screen);
    draw_board(screen, brd);
    draw_tablets(screen, tbl);
    draw_scores(screen);
    draw_moving_shape(screen, mouse_st);
    
    SDL_Flip(screen);
    SDL_Delay(100);
  }
}
                     
static void setup_background(SDL_Surface* screen)
{
  /* These macros compute the number of times the background
     tile has to be blitted to fill the width and height
     of the screen. */
#define BG_SIZE 100
#define NB_TILES_W ((int) ceil(screen->w / BG_SIZE))
#define NB_TILES_H ((int) ceil(screen->h / BG_SIZE))

  SDL_Surface* bg = NULL,
     *term_button = NULL,
     *quit_button = NULL;
  SDL_Rect bg_pos;
  int i, j;

  /* Load the background image */
  bg = IMG_Load("./arriere.png");
  if (bg == NULL) {
    SDL_ERR("Unable to load image");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < NB_TILES_H; i++) {
    for (j = 0; j <= NB_TILES_W; j++) {
      bg_pos.x = j * BG_SIZE;
      bg_pos.y = i * BG_SIZE;
      SDL_BlitSurface(bg, NULL, screen, &bg_pos);
    }
  }
  SDL_FreeSurface(bg);

  term_button = IMG_Load("imgs/terminer.png");
  if (term_button == NULL)
    SDL_ERR("Unable to load image");
  
  quit_button = IMG_Load("imgs/terminer.png");
  if (quit_button == NULL)
    SDL_ERR("Unable to load image");

  bg_pos.x = TBLT_BIG_MARGIN + (13 * SQR_SIZE) + 150;
  bg_pos.y = screen->h / 2 - 75;
  SDL_BlitSurface(term_button, NULL, screen, &bg_pos);

  bg_pos.y += 80;
  SDL_BlitSurface(quit_button, NULL, screen, &bg_pos);
  SDL_FreeSurface(quit_button);
}

/** n
 * Draws the central board of the game.
 * 
 * @param screen The screen on which to draw
 * @param brd The board to be drawn
 * 
 * @return void
 */
static void draw_board(SDL_Surface* screen, board_t* brd)
{
  SDL_Rect pos;
  int i, j;
  SDL_Surface* img_play = NULL;
  
  for (i = 0; i < brd->h; i++) {
    for (j = 0; j < brd->w; j++) {
      pos.x = j * SQR_SIZE + BRD_MARGIN;
      pos.y = i * SQR_SIZE + BRD_MARGIN;
      draw_square(screen, brd->sqrs[i][j], &pos);
      
      if (players[player]->can_play[i][j] == 1) {
        img_play = IMG_Load("imgs/playable.png");
        if (img_play == NULL)
          SDL_ERR("Unable to load image");
        SDL_BlitSurface(img_play, NULL, screen, &pos);
        SDL_FreeSurface(img_play);
      }
    }
  }
}

/** 
 * Draws a square on the screen at a specified place.
 * 
 * @param screen The screen on which to draw
 * @param sq The square to draw
 * @param pos The position at which the square will be blitted
 */
static void draw_square(SDL_Surface* screen, square_t* sq, SDL_Rect* pos)
{
  SDL_Surface* sq_tile = NULL;
  sq_tile = IMG_Load(sqrs_tiles[sq->color]);
  if (sq_tile == NULL)
    SDL_ERR("Unable to load image");

  SDL_BlitSurface(sq_tile, NULL, screen, pos);
  SDL_FreeSurface(sq_tile);
  draw_border(screen, sq, pos);
}

/** 
 * Draws the borders around a square. Any square might have a upper,
 * lower, left or right border.
 * 
 * @param screen 
 * @param sq 
 * @param pos 
 */
static void draw_border(SDL_Surface* screen, square_t* sq, SDL_Rect* pos)
{
#define RGB_SURFACE(flags, w, h) \
  SDL_CreateRGBSurface((flags), (w), (h), 32, 0, 0, 0, 0)
  
#define SDL_COLOR(r, g, b) \
  SDL_MapRGB(screen->format, (r), (g), (b))

  SDL_Rect brd_pos;
  SDL_Surface* vert_bord = RGB_SURFACE(SDL_HWSURFACE, 1, SQR_SIZE);
  SDL_Surface* horiz_bord = RGB_SURFACE(SDL_HWSURFACE, SQR_SIZE, 1);

  /* Set border color */
  unsigned char r = 0, g = 0, b = 0;
  switch (sq->color) {
    case BLUE: b = 100; g = 100; break;
    case RED: r = 180; break;
    case GREEN: g = 100; b = 8; r = 7; break;
    case YELLOW:
      r = 207; g = 202; b = 10;
      break;

    default: break;
  }
  SDL_FillRect(vert_bord, NULL, SDL_COLOR(r, g, b));
  SDL_FillRect(horiz_bord, NULL, SDL_COLOR(r, g, b));
  
  if (sq->borders & BRDR_U)
    SDL_BlitSurface(horiz_bord, NULL, screen, pos);

  if (sq->borders & BRDR_L)
    SDL_BlitSurface(vert_bord, NULL, screen, pos);

  if (sq->borders & BRDR_D) {
    brd_pos.x = pos->x;
    brd_pos.y = pos->y + SQR_SIZE;
    SDL_BlitSurface(horiz_bord, NULL, screen, &brd_pos);
  }
  
  if (sq->borders & BRDR_R) {
    brd_pos.x = pos->x + SQR_SIZE;
    brd_pos.y = pos->y;
    SDL_BlitSurface(vert_bord, NULL, screen, &brd_pos);
  }

  SDL_FreeSurface(horiz_bord);
  SDL_FreeSurface(vert_bord);
}

static void draw_tablets(SDL_Surface* screen, tablet_t** tbl)
{
  int i;
  for (i = 0; i < NB_TABLETS; i++)
    draw_tablet(screen, tbl[i], tablets_pos[i].orig_x, tablets_pos[i].orig_y);
}

static void draw_tablet(SDL_Surface* screen, tablet_t* tbl,
                        int orig_x, int orig_y)
{
  int i, j;
  SDL_Rect pos;

  for (i = 0; i < TBLT_SIZE; i++) {
    for (j = 0; j < TBLT_SIZE; j++) {
      if (tbl->sqrs[i][j] != NULL) {
        pos.x = j * SQR_SIZE + orig_x;
        pos.y = i * SQR_SIZE + orig_y;
        draw_square(screen, tbl->sqrs[i][j], &pos);
      }
    }
  }
}

static void draw_scores(SDL_Surface* screen)
{
  int sc;
  SDL_Rect pos;
  SDL_Surface* img;
  int x;
  const char* path;
  int pl;

  for (pl = 0; pl < NB_PLAYERS; pl++) {
    if (pl == 0) {
      pos.x = TBLT_SMALL_MARGIN + (TBLT_SIZE * SQR_SIZE) + 2;
      pos.y = TBLT_SMALL_MARGIN;
    } else if (pl == 1) {
      pos.x = TBLT_BIG_MARGIN - 30;
      pos.y = TBLT_SMALL_MARGIN;
    } else if (pl == 2) {
      pos.x = TBLT_BIG_MARGIN - 30;
      pos.y = TBLT_BIG_MARGIN + 9 * SQR_SIZE;
    } else if (pl == 3) {
      pos.x = TBLT_SMALL_MARGIN + (TBLT_SIZE * SQR_SIZE) + 2;
      pos.y = TBLT_BIG_MARGIN + 9 * SQR_SIZE;
    }

    pos.y += 60;
    sc = players[pl]->score;
    do {
      x = sc % 10;
      path = numbers_tiles[x];
      img = IMG_Load(path);
      if (img == NULL)
        SDL_ERR("Unable to load image");

      SDL_BlitSurface(img, NULL, screen, &pos);
      SDL_FreeSurface(img);
      pos.y -= 30;
      sc /= 10;
    } while (sc != 0);
    
    if (players[pl]->score <= 9) {
      img = IMG_Load("imgs/0.png");
      SDL_BlitSurface(img, NULL, screen, &pos);
      SDL_FreeSurface(img);
      pos.y -= 30;
    }
    if (players[pl]->score <= 99) {
      img = IMG_Load("imgs/0.png");
      SDL_BlitSurface(img, NULL, screen, &pos);
      SDL_FreeSurface(img);
    }
  }
}

static void draw_shape(SDL_Surface* screen, shape_t* sh, int x, int y)
{
  sqrs_queue_t* curr = sh->head;
  SDL_Rect pos;

  while (curr != NULL) {
    pos.x = x + curr->dx * SQR_SIZE;
    pos.y = y + curr->dy * SQR_SIZE;
    draw_square(screen, curr->sq, &pos);
    curr = curr->next;
  }
}

static void draw_moving_shape(SDL_Surface* screen, mouse_state_t* mouse_st)
{
  if (mouse_st->has_shape == 1) {
    draw_shape(screen, mouse_st->msh->sh, mouse_x, mouse_y);
  } 
}

static void erase_shape(tablet_t* tblt, shape_t* sh)
{
  int ind_x, ind_y;
  sqrs_queue_t* curr = sh->head;
  while (curr != NULL) {
    ind_x = sh->tbl_x + curr->dx;
    ind_y = sh->tbl_y + curr->dy;
    tblt->sqrs[ind_y][ind_x] = square_create(curr->sq->borders,
                                             VOID, NULL);
    curr = curr->next;
  }
}

static void handle_keydown(int key, int* run, mouse_state_t* mouse_st)
{
  switch (key) {
    case SDLK_ESCAPE:
    case SDLK_q:
      *run = 0;
      break;

    case SDLK_UP:
    case SDLK_DOWN:
      if (mouse_st->has_shape == 1) {
        mouse_st->msh->sh = shape_reflect(mouse_st->msh->sh, REFLECT_H,
                                          mouse_st->msh->sh->head->sq->color);
      }
      break;

    case SDLK_LEFT:
    case SDLK_RIGHT:
      if (mouse_st->has_shape == 1) {
        mouse_st->msh->sh = shape_reflect(mouse_st->msh->sh, REFLECT_V,
                                          mouse_st->msh->sh->head->sq->color);
      }
      break;

    default: break;
  }
}

static mouse_state_t* init_mouse_state(void)
{
  mouse_state_t* st = malloc(sizeof *st);
  if (st == NULL) {
    ERR("Malloc error");
    return NULL;
  }

  st->has_shape = 0;
  st->msh = NULL;
  return st;
}

static void advance_player(int* run)
{
  int nb_switches = 0;
  do {
    player = (player + 1) % NB_PLAYERS;
    nb_switches++;
  } while (players[player]->still_playing == 0 && nb_switches < 5);

  if (nb_switches >= 5)
    *run = 0;
}

static void handle_button_down(SDL_Surface* screen, SDL_MouseButtonEvent* btn,
                               tablet_t** tblts, mouse_state_t* mouse_st, board_t* brd, int* run)
{
#define BRD_CLICK_IND(X) ((int)((X) - BRD_MARGIN) / SQR_SIZE) 

  moving_shape_t* msh = NULL;
  
  if (mouse_st->has_shape == 0 && btn->button == SDL_BUTTON_LEFT) {
    if ((msh = get_clicked_shape(btn->x, btn->y, tblts[player], player)) != NULL) {
      erase_shape(tblts[msh->tblt_nb], msh->sh);
      draw_shape(screen, msh->sh, btn->x, btn->y);
      
      mouse_st->has_shape = 1;
      mouse_st->msh = msh;
    }
  } else if (mouse_st->has_shape == 1 && btn->button == SDL_BUTTON_RIGHT) {
    tablet_place_shape(tblts[mouse_st->msh->tblt_nb], mouse_st->msh->orig_sh);
    mouse_st->has_shape = 0;
  } else if (mouse_st->has_shape == 1 && btn->button == SDL_BUTTON_LEFT) {
    if (is_click_inboard(btn->x, btn->y)) {
      if (board_place_shape(brd, mouse_st->msh->sh,
                            BRD_CLICK_IND(btn->x),
                            BRD_CLICK_IND(btn->y),
                            players, player)) {
        advance_player(run);
        mouse_st->has_shape = 0;
      }
    }
  }

  /* bouton terminer */
  if (btn->x >= TBLT_BIG_MARGIN + (13 * SQR_SIZE) + 150
      && btn->x <= TBLT_BIG_MARGIN + (13 * SQR_SIZE) + 350
      && btn->y >= screen->h / 2 - 75
      && btn->y <= screen->h / 2 - 5) {
    players[player]->still_playing = 0;
    advance_player(run);
  }
  /* bouton quitter */
  if (btn->x >= TBLT_BIG_MARGIN + (13 * SQR_SIZE) + 150
      && btn->x <= TBLT_BIG_MARGIN + (13 * SQR_SIZE) + 350
      && btn->y >= screen->h / 2 + 5
      && btn->y <= screen->h / 2 + 75)
    *run = 0;
}

static moving_shape_t* get_clicked_shape(int x, int y, tablet_t* tblt, int player)
{
  int ind_x, ind_y;

  ind_x = (x - tablets_pos[player].orig_x) / SQR_SIZE;
  ind_y = (y - tablets_pos[player].orig_y) / SQR_SIZE;
  if (ind_x >= 0 && ind_x < TBLT_SIZE
      && ind_y >= 0 && ind_y < TBLT_SIZE
      && tblt->sqrs[ind_y][ind_x]->color != VOID) {
    return new_moving_shape(tblt->sqrs[ind_y][ind_x]->sh,
                            player, ind_x, ind_y);
  }
  return NULL;
}

static moving_shape_t* new_moving_shape(shape_t* sh, int tblt_nb, int x, int y)
{
  moving_shape_t* msh = malloc(sizeof *msh);
  if (msh == NULL) {
    ERR("Malloc error");
    return NULL;
  }

  msh->orig_sh = sh;
  msh->sh = sh;
  msh->tblt_nb = tblt_nb;
  msh->tblt_x = x;
  msh->tblt_y = y;
  return msh;
}

static int is_click_inboard(int click_x, int click_y)
{                                       /* TODO magic constant, bouh */
  return click_x >= BRD_MARGIN && click_x < BRD_MARGIN + SQR_SIZE * 20
      && click_y >= BRD_MARGIN && click_y < BRD_MARGIN + SQR_SIZE * 20;
}
