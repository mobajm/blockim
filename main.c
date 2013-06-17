
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "graphic.h"
#include "square.h"


int main ( int argc, char** argv )
{
    SDL_Surface* screen=NULL;
    SDL_Event even;
    int k=0,i=0,j=0;
    int blue=0,green=0,red=0,yellow=0;
    /*table_t tab;*/
    tablet_t shape_green,shape_red ,shape_blue,shape_yellow,shape_void;
    tablet_t shape_green1,shape_red1 ,shape_blue1,shape_yellow1;
    shape_t shap;
    tables_t tabl;

    SDL_Init(SDL_INIT_VIDEO);
    screen=SDL_SetVideoMode(0,0,32,SDL_FULLSCREEN | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("BLOCKIM","icon.ico");


    table_init(&tabl);
    /* */
    initiate_tablet(&shape_green,&shape_red ,&shape_blue,&shape_yellow);
    SDL_Screen(screen,&shape_green,&shape_red ,&shape_blue,&shape_yellow);




        /*shape_green.ta[0].sqr[0].posi.x=400;
        shape_green.ta[0].sqr[0].posi.y=300;
        shape_red.ta[0].sqr[0].posi.x=100;
        shape_red.ta[0].sqr[0].posi.y=250;

        for(i=0;i<21;i++)
        {
        shape_init_posi(&shape_green.ta[i]);
        SDL_create_shape(screen,&shape_green.ta[i],i,GREEN,VERTICAL);
        }*/

      /*for(i=0;i<21;i++)
     {
         shape_init_posi(&shape[i]);
         SDL_create_shape(screen,&shape[i],i,BLUE);
     }*/




     do{
        SDL_PollEvent(&even);
        color_square_t cs;
        switch(even.type){

        case SDL_QUIT:{
            k=1;
        }break;
        case SDL_KEYDOWN:
        {
            if(even.key.keysym.sym==SDLK_ESCAPE)
            {
                k=1;
            }
        }break;
        case SDL_MOUSEBUTTONDOWN:
        {
            if(blue==0 && green==0 && yellow==0 && red==0)
            cs=detect_shape(even,shape_green,shape_red,shape_blue,shape_yellow);

            if(cs.cl==BLUE)
            {
                blue=1;yellow=0;green=0;red=0;/*SDL_ShowCursor(SDL_DISABLE);*/
                 /*SDL_WarpMouse(shape_blue.ta[cs.typ].sqr[17].posi.x,shape_blue.ta[cs.typ].sqr[17].posi.y);*/
            }
            else if(cs.cl==GREEN)
            {
                green=1;blue=0;yellow=0;red=0;/*SDL_ShowCursor(SDL_DISABLE);*/
                /*SDL_WarpMouse(shape_green.ta[cs.typ].sqr[13].posi.x,shape_green.ta[cs.typ].sqr[11].posi.x);*/
            }
            else if(cs.cl==RED)
            {
                red=1;green=0;blue=0;yellow=0;/*SDL_ShowCursor(SDL_DISABLE);*/
                /*SDL_WarpMouse(shape_red.ta[cs.typ].sqr[11].posi.x,shape_red.ta[cs.typ].sqr[11].posi.x);*/
            }
            else if(cs.cl==YELOW)
            {
                yellow=1;red=0;blue=0;green=0;/*SDL_ShowCursor(SDL_DISABLE);*/
               /* SDL_WarpMouse(shape_yellow.ta[cs.typ].sqr[11].posi.x,shape_yellow.ta[cs.typ].sqr[11].posi.x);*/
            }
            if(even.button.x >168 && even.button.x <168+502 && even.button.y >147 && even.button.y< 147+502 )
            {
                if(blue==1)
                {
                    /*if(!detect_station_void(even,shape_green,shape_red,shape_blue,shape_yellow))*/
                    {
                    shape_blue.ta[cs.typ].sqr[0].posi.x=(int)(even.button.x/25) * 25 - 55;
                    shape_blue.ta[cs.typ].sqr[0].posi.y=(int)(even.button.y/25) * 25 - 51;
                    shape_init_posi(&shape_blue.ta[cs.typ]);
                    SDL_Upload_Screen(screen,&shape_green,&shape_red,&shape_blue,&shape_yellow);
                    red=0; yellow=0;green=0;blue=0;
                    shape_place(&shape_blue,cs.typ);
                    cs.cl=VOID;
                    cs.numb=0;
                    cs.typ=21;

                    }

                }
                 else if(green==1)
                {
                   /* if(!detect_station_void(even,shape_green,shape_red,shape_blue,shape_yellow))*/
                    {
                    shape_green.ta[cs.typ].sqr[0].posi.x=(int)(even.button.x/25) * 25 -55;
                    shape_green.ta[cs.typ].sqr[0].posi.y=(int)(even.button.y/25) * 25 -51;
                    shape_init_posi(&shape_green.ta[cs.typ]);
                    SDL_Upload_Screen(screen,&shape_green,&shape_red,&shape_blue,&shape_yellow);
                    red=0; yellow=0;green=0;blue=0;
                    shape_place(&shape_green,cs.typ);
                    cs.cl=VOID;
                    cs.numb=0;
                    cs.typ=21;
                    }

                }
                 else if(red==1)
                {
                    /*if(detect_station_void(even,shape_green,shape_red,shape_blue,shape_yellow))*/
                    {
                    shape_red.ta[cs.typ].sqr[0].posi.x=(int)(even.button.x/25) * 25 -55;
                    shape_red.ta[cs.typ].sqr[0].posi.y=(int)(even.button.y/25) * 25 -51;
                    shape_init_posi(&shape_red.ta[cs.typ]);
                    SDL_Upload_Screen(screen,&shape_green,&shape_red,&shape_blue,&shape_yellow);
                    red=0; yellow=0;green=0;blue=0;
                    shape_place(&shape_red,cs.typ);
                    cs.cl=VOID;
                    cs.numb=0;
                    cs.typ=21;
                    }

                }
                 else if(yellow==1)
                {
                    /*if(detect_station_void(even,shape_green,shape_red,shape_blue,shape_yellow))*/
                    {
                    shape_yellow.ta[cs.typ].sqr[0].posi.x=(int)(even.button.x/25) * 25 -55;
                    shape_yellow.ta[cs.typ].sqr[0].posi.y=(int)(even.button.y/25) * 25 -51;
                    shape_init_posi(&shape_yellow.ta[cs.typ]);
                    SDL_Upload_Screen(screen,&shape_green,&shape_red,&shape_blue,&shape_yellow);
                    red=0; yellow=0;green=0;blue=0;
                    shape_place(&shape_yellow,cs.typ);
                    cs.cl=VOID;
                    cs.numb=21;
                    cs.typ=21;
                    }

                }

            }

        }break;
        case SDL_MOUSEMOTION:
        {

                if(blue==1)
                {
                shape_blue.ta[cs.typ].sqr[0].posi.x=even.motion.x-65;
                shape_blue.ta[cs.typ].sqr[0].posi.y=even.motion.y-65;
                shape_init_posi(&shape_blue.ta[cs.typ]);


                SDL_Upload_Screen(screen,&shape_green,&shape_red,&shape_blue,&shape_yellow);

                SDL_create_shape(screen,&shape_blue.ta[cs.typ],cs.typ,BLUE,NORMAL);
                }
                else if(green==1)
                {
                shape_green.ta[cs.typ].sqr[0].posi.x=even.motion.x-65;
                shape_green.ta[cs.typ].sqr[0].posi.y=even.motion.y-65;
                shape_init_posi(&shape_green.ta[cs.typ]);


                SDL_Upload_Screen(screen,&shape_green,&shape_red,&shape_blue,&shape_yellow);

                SDL_create_shape(screen,&shape_green.ta[cs.typ],cs.typ,GREEN,VERTICAL);
                }
                if(red==1)
                {
                shape_red.ta[cs.typ].sqr[0].posi.x=even.motion.x-65;
                shape_red.ta[cs.typ].sqr[0].posi.y=even.motion.y-65;
                shape_init_posi(&shape_red.ta[cs.typ]);


                SDL_Upload_Screen(screen,&shape_green,&shape_red,&shape_blue,&shape_yellow);

                SDL_create_shape(screen,&shape_red.ta[cs.typ],cs.typ,RED,HORI_VERTI);
                }
                if(yellow==1)
                {
                shape_yellow.ta[cs.typ].sqr[0].posi.x=even.motion.x-65;
                shape_yellow.ta[cs.typ].sqr[0].posi.y=even.motion.y-65;
                shape_init_posi(&shape_yellow.ta[cs.typ]);


                SDL_Upload_Screen(screen,&shape_green,&shape_red,&shape_blue,&shape_yellow);

                SDL_create_shape(screen,&shape_yellow.ta[cs.typ],cs.typ,YELOW,HORIZONTAL);
                }
                else
                {
                   /* SDL_Upload_Screen(screen,&shape_green,&shape_red,&shape_blue,&shape_yellow);*/

                }


        }break;
        default: break;

        }

      SDL_Flip(screen);
    }while(k!=1);


       SDL_FreeSurface(screen);

    SDL_Quit();


    return 0;
}
