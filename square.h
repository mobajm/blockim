
#ifndef _H_square_
#endif

#define SIZE_SQR 25

#include <SDL.h>
#define HORIZONTAL 0
#define VERTICAL 1
#define HORI_VERTI 2
#define NORMAL 3



typedef enum  {BLUE,YELOW,RED,GREEN,GRIS,VOID} sqr_e;

typedef enum  {RIGHT,LEFT,UP,DOWN} direct_e;

typedef enum  {VOI,P_BLUE,P_GREEN,P_RED,P_YELLOW} autority_e;

typedef struct square_s
{
    SDL_Rect posi;
    sqr_e clr;
    int L;
    int R;
    int U;
    int D;
}square_t;

typedef struct squares_s
{
    square_t sqr[SIZE_SQR];
    int pl;
}shape_t;

typedef struct tablet_s
{
    shape_t ta[21];
}tablet_t;

typedef struct table_s
{
    SDL_Rect pos;
    int blu_p;
    int red_p;
    int gre_p;
    int yel_p;
    autority_e au;
}table_t;

typedef struct tables_s
{
    table_t tab[400];
}tables_t;

typedef struct color_square_s
{
    sqr_e cl;
    int typ;
    int numb;
}color_square_t;

void shape_border(shape_t* shape);
void shape_init(shape_t* shape,int typ,sqr_e color);
void choose_clr(sqr_e color, char* name);
void SDL_create_shape(SDL_Surface* screen,shape_t *shape,int typ,sqr_e color,int);
void type_frame(SDL_Surface* screen,SDL_Rect posi,int type,direct_e direction,sqr_e color);
void square_frame(SDL_Surface* screen ,square_t sqre);
void shape_frame(SDL_Surface* screen ,shape_t shape);
void shape_init_posi(shape_t *shape);
void initiate_tablet(tablet_t* shape_green,tablet_t *shape_red ,tablet_t *shape_blue,tablet_t *shape_yellow);
void reflexion(int type,shape_t *shape);
void create_table(SDL_Surface* screen);
color_square_t detect_shape(SDL_Event even,tablet_t shape_green,tablet_t shape_red ,tablet_t shape_blue,tablet_t shape_yellow);
int detect_station_void(SDL_Event even,tablet_t shape_green,tablet_t shape_red ,tablet_t shape_blue,tablet_t shape_yellow);
void shape_place(tablet_t *shape,int typ);
void table_init(tables_t *tabl);
