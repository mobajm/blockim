#include <SDL.h>
#include <SDL_image.h>
#include "graphic.h"
#include "square.h"


void SDL_Screen(SDL_Surface* screen,tablet_t *shape_green,tablet_t *shape_red ,tablet_t *shape_blue,tablet_t *shape_yellow);
void SDL_Upload_Screen(SDL_Surface* screen,tablet_t *shape_green,tablet_t *shape_red ,tablet_t *shape_blue,tablet_t *shape_yellow);

void SDL_Screen(SDL_Surface* screen,tablet_t *shape_green,tablet_t *shape_red ,tablet_t *shape_blue,tablet_t *shape_yellow)
{
    SDL_Surface *surface=NULL,*table=NULL;
    SDL_Rect posi;
    int i=0,j=0;

    surface=IMG_Load("arriere2.png");


    for(i=0;i<13;i++){
        for(j=0;j<10;j++){
          posi.x=i*100;
          posi.y=j*100;
          SDL_BlitSurface(surface,NULL,screen,&posi);
        }
    }

    create_table(screen);

    for(i=0;i<21;i++)
        {
            shape_init_posi(&shape_blue->ta[i]);
            SDL_create_shape(screen,&shape_blue->ta[i],i,BLUE,NORMAL);
        }
    for(i=0;i<21;i++)
        {
            shape_init_posi(&shape_green->ta[i]);
            SDL_create_shape(screen,&shape_green->ta[i],i,GREEN,VERTICAL);
        }
    for(i=0;i<21;i++)
        {
            shape_init_posi(&shape_yellow->ta[i]);
            SDL_create_shape(screen,&shape_yellow->ta[i],i,YELOW,HORIZONTAL);
        }
    for(i=0;i<21;i++)
        {
            shape_init_posi(&shape_red->ta[i]);
            SDL_create_shape(screen,&shape_red->ta[i],i,RED,HORI_VERTI);
        }

    SDL_FreeSurface(table);
    SDL_Flip(screen);

}
void SDL_Upload_Screen(SDL_Surface* screen,tablet_t *shape_green,tablet_t *shape_red ,tablet_t *shape_blue,tablet_t *shape_yellow)
{
    SDL_Surface *image[5]={NULL},*surface=NULL;
    SDL_Rect posi;
    int i=0,j=0;

    image[0]=IMG_Load("blu.png");
    image[1]=IMG_Load("voi.png");
    image[2]=IMG_Load("yel.png");
    image[3]=IMG_Load("red.png");
    image[4]=IMG_Load("gre.png");


    surface=IMG_Load("arriere2.png");


    for(i=0;i<13;i++){
        for(j=0;j<10;j++){
          posi.x=i*100;
          posi.y=j*100;
          SDL_BlitSurface(surface,NULL,screen,&posi);
        }
    }


    create_table(screen);

   for(i=0;i<21;i++)
    {
        for(j=0;j<25;j++)
    {
        if(shape_blue->ta[i].sqr[j].clr==BLUE)
        {
           SDL_BlitSurface(image[0],NULL,screen,&shape_blue->ta[i].sqr[j].posi);
        }
        else
        {
            SDL_BlitSurface(image[1],NULL,screen,&shape_blue->ta[i].sqr[j].posi);
        }
    }
         shape_frame(screen,shape_blue->ta[i]);
    }

    for(i=0;i<21;i++)
    {
        for(j=0;j<25;j++)
    {
        if(shape_green->ta[i].sqr[j].clr==GREEN)
        {
           SDL_BlitSurface(image[4],NULL,screen,&shape_green->ta[i].sqr[j].posi);
        }
        else
        {
            SDL_BlitSurface(image[1],NULL,screen,&shape_green->ta[i].sqr[j].posi);
        }
    }
         shape_frame(screen,shape_green->ta[i]);
    }

    for(i=0;i<21;i++)
    {
        for(j=0;j<25;j++)
    {
        if(shape_red->ta[i].sqr[j].clr==RED)
        {
           SDL_BlitSurface(image[3],NULL,screen,&shape_red->ta[i].sqr[j].posi);
        }
        else
        {
            SDL_BlitSurface(image[1],NULL,screen,&shape_red->ta[i].sqr[j].posi);
        }
    }
         shape_frame(screen,shape_red->ta[i]);
    }

    for(i=0;i<21;i++)
    {
        for(j=0;j<25;j++)
    {
        if(shape_yellow->ta[i].sqr[j].clr==YELOW)
        {
           SDL_BlitSurface(image[2],NULL,screen,&shape_yellow->ta[i].sqr[j].posi);
        }
        else
        {
            SDL_BlitSurface(image[1],NULL,screen,&shape_yellow->ta[i].sqr[j].posi);
        }
    }
         shape_frame(screen,shape_yellow->ta[i]);
    }


   for(i=0;i<5;i++)
   {
       SDL_FreeSurface(image[i]);
   }

}

