#include <stdio.h>
#include "square.h"
#include <SDL.h>
#include <SDL_image.h>


static void shape_impl(char* nom ,shape_t* shape,sqr_e color);
static void border_init(shape_t* shape);


static void shape_impl(char* nom ,shape_t* shape,sqr_e color)
{
    FILE* fichier=fopen(nom,"r");

    int i=0,j=0;
    int c=0;
    for(j=0;j<5;j++){
        for(i=0;i<5;i++){
        fscanf(fichier,"%i",&c);
        if(c==0) shape->sqr[j*5+i].clr=VOID;
        else if(c==1) shape->sqr[j*5+i].clr=color;
        else shape->sqr[j*5+i].clr=VOID;
         }
         c=fgetc(fichier);
    }

    fclose(fichier);
}

static void border_init(shape_t* shape)
{
    int i=0;
    for(i=0;i<SIZE_SQR;i++){
    shape->sqr[i].L=0;
    shape->sqr[i].R=0;
    shape->sqr[i].D=0;
    shape->sqr[i].U=0;
    shape->pl=0;
    }
}

void shape_border(shape_t* shape)
{
    int i=0,j=0;

    /* initialise square border from 0 to 4 (first line) */
    for(i=0;i<5;i++){

        if(shape->sqr[i].clr != VOID){
           shape->sqr[i].U=1;
          if(shape->sqr[i+1].clr == VOID) shape->sqr[i].R=1;
          if(shape->sqr[i-1].clr == VOID) shape->sqr[i].L=1;
          if(shape->sqr[5+i].clr == VOID) shape->sqr[i].D=1;

        if(i==0) shape->sqr[i].L=1;
        else if(i==4) shape->sqr[i].R=1;
        }
        else
        {
            shape->sqr[i].R=0;
            shape->sqr[i].L=0;
            shape->sqr[i].D=0;
            shape->sqr[i].U=0;
        }
        }

    /* initialise square border from 5 to 19 (body-line) */
    for(i=1;i<4;i++)
    {
        for(j=0;j<5;j++)
        {
            if(shape->sqr[j+i*5].clr != VOID){
            if(shape->sqr[j+1+i*5].clr == VOID) shape->sqr[j+i*5].R=1;
            if(shape->sqr[j-1+i*5].clr == VOID) shape->sqr[j+i*5].L=1;
            if(shape->sqr[j+(i+1)*5].clr == VOID) shape->sqr[j+i*5].D=1;
            if(shape->sqr[j+(i-1)*5].clr == VOID) shape->sqr[j+i*5].U=1;
            }
        }
    }
    /*initialise square border from 20 to 24 (last-line)*/
    for(i=20;i<25;i++)
    {
        if(shape->sqr[i].clr != VOID){
          if(shape->sqr[i+1].clr == VOID) shape->sqr[i].R=1;
          if(shape->sqr[i-1].clr == VOID) shape->sqr[i].L=1;
          if(shape->sqr[i-5].clr == VOID) shape->sqr[i].U=1;
        shape->sqr[i].D=1;
        if(i==20) shape->sqr[i].L=1;
        else if(i==24) shape->sqr[i].R=1;
        }
    }
}
void shape_init(shape_t* shape,int typ,sqr_e color)
{

    border_init(shape);

    switch(typ){
        case 0:{
            shape_impl("shape/shape0.txt",shape,color);
        }break;
        case 1:{
            shape_impl("shape/shape1.txt",shape,color);
        }break;
        case 2:{
            shape_impl("shape/shape2.txt",shape,color);
        }break;
        case 3:{
            shape_impl("shape/shape3.txt",shape,color);
        }break;
        case 4:{
            shape_impl("shape/shape4.txt",shape,color);
        }break;
        case 5:{
            shape_impl("shape/shape5.txt",shape,color);
        }break;
        case 6:{
            shape_impl("shape/shape6.txt",shape,color);
        }break;
        case 7:{
            shape_impl("shape/shape7.txt",shape,color);
        }break;
        case 8:{
            shape_impl("shape/shape8.txt",shape,color);
        }break;
        case 9:{
            shape_impl("shape/shape9.txt",shape,color);
        }break;
        case 10:{
            shape_impl("shape/shape10.txt",shape,color);
        }break;
        case 11:{
            shape_impl("shape/shape11.txt",shape,color);
        }break;
        case 12:{
            shape_impl("shape/shape12.txt",shape,color);
        }break;
        case 13:{
            shape_impl("shape/shape13.txt",shape,color);
        }break;
        case 14:{
            shape_impl("shape/shape14.txt",shape,color);
        }break;
        case 15:{
            shape_impl("shape/shape15.txt",shape,color);
        }break;
        case 16:{
            shape_impl("shape/shape16.txt",shape,color);
        }break;
        case 17:{
            shape_impl("shape/shape17.txt",shape,color);
        }break;
        case 18:{
            shape_impl("shape/shape18.txt",shape,color);
        }break;
        case 19:{
            shape_impl("shape/shape19.txt",shape,color);
        }break;
        case 20:{
            shape_impl("shape/shape20.txt",shape,color);
        }break;
        case 21:
        {
             shape_impl("shape/shape21.txt",shape,color);
        }break;
        default: break;
    }


}

void choose_clr(sqr_e color, char* name)
{
    switch(color)
    {
        case YELOW:
        {
            name="yel.png";
        }break;
        case BLUE:
        {
            name="blu.png";
        }break;
        case RED:
        {
            name="red.png";
        }break;
        case GREEN:
        {
            name="gre.png";
        }break;
        case VOID:
        {
            name="voi.png";
        }break;
        default: break;
    }
}

void SDL_create_shape(SDL_Surface* screen,shape_t *shape,int typ,sqr_e color,int refle)
{
  SDL_Surface *image[25]={NULL};

  char* name;

  int i=0,j=0;
  shape_init_posi(shape);
  shape_init(shape,typ,color);
  switch(refle)
  {
      case HORIZONTAL:
      {
          reflexion(HORIZONTAL,shape);
      }break;
      case VERTICAL:
      {
          reflexion(VERTICAL,shape);
      }break;
      case HORI_VERTI:
      {
          reflexion(HORI_VERTI,shape);
      }break;
      default: break;
  }

  shape_border(shape);

  for(i=0;i<5;i++)
  {

    for(j=0;j<5;j++)
    {
              if(shape->sqr[i*5+j].clr == YELOW)
              {
                  name="yel.png";
              }
              else if(shape->sqr[i*5+j].clr == BLUE)
              {
                  name="blu.png";
              }
              else if(shape->sqr[i*5+j].clr == RED)
              {
                  name="red.png";
              }
              else if(shape->sqr[i*5+j].clr == GREEN)
              {
                  name="gre.png";
              }
              else if (shape->sqr[i*5+j].clr == GRIS)
              {
                  name="gris.png";
              }
              else if(shape->sqr[i*5+j].clr == VOID)
              {
                  name="voi.png";
              }
              image[i*5+j]=IMG_Load(name);
              SDL_BlitSurface(image[i*5+j],NULL,screen,&shape->sqr[i*5+j].posi);
    }
  }
  shape_frame(screen,*shape);
  SDL_Flip(screen);
    for(i=0;i<25;i++)
  {
      SDL_FreeSurface(image[i]);
  }
}

void type_frame(SDL_Surface* screen,SDL_Rect posi,int type,direct_e direction,sqr_e color)
{
    SDL_Surface *square_i=NULL;

    square_i = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, 2, 32, 0, 0,0, 0);

    int redd=0,greenn=0,bluee=0,i=0;

    switch(color)
    {
        case RED:
        {
            redd=194;greenn=13;bluee=3;
        }break;
        case BLUE:
        {
            redd=28;greenn=18;bluee=139;
        }break;
        case YELOW:
        {
            redd=207;greenn=202;bluee=10;
        }break;
        case GREEN:
        {
            redd=29;greenn=168;bluee=9;
        }break;
        case GRIS:
        {
            redd=126;greenn=126;bluee=126;
        }break;
        default: break;
    }

    SDL_FillRect(square_i, NULL, SDL_MapRGB(screen->format, redd,greenn, bluee));

    if(type==HORIZONTAL)
    {
        if(direction==UP)
        {
            posi.y-=1;
            posi.x+=1;
        }
        else if(direction==DOWN)
        {
            posi.y+=25;
            posi.x+=1;
        }
        for(i=0;i<25;i++)
        {
            SDL_BlitSurface(square_i,NULL,screen,&posi);
            posi.x+=1;
        }
    }
    else if(type==VERTICAL)
    {
        if(direction==RIGHT)
        {
            posi.y-=1;
            posi.x+=25;
        }
        else if(direction==LEFT)
        {
            posi.y-=1;
            posi.x-=1;
        }
        for(i=0;i<27;i++)
        {
            SDL_BlitSurface(square_i,NULL,screen,&posi);
            posi.y+=1;
        }
    }

    SDL_FreeSurface(square_i);
}

void square_frame(SDL_Surface* screen ,square_t sqre)
{
   if(sqre.L==1)
   {
       type_frame(screen,sqre.posi,VERTICAL,LEFT,sqre.clr);
   }
   if(sqre.R==1)
   {
       type_frame(screen,sqre.posi,VERTICAL,RIGHT,sqre.clr);
   }
   if(sqre.D==1)
   {
       type_frame(screen,sqre.posi,HORIZONTAL,DOWN,sqre.clr);
   }
   if(sqre.U==1)
   {
       type_frame(screen,sqre.posi,HORIZONTAL,UP,sqre.clr);
   }
}

void shape_frame(SDL_Surface* screen ,shape_t shape)
{
    int i=0;
    for(i=0;i<25;i++)
    {
        square_frame(screen,shape.sqr[i]);
    }
}

void shape_init_posi(shape_t *shape)
{
    int i=0,j=0,y=0,x=0;

    y=shape->sqr[0].posi.y;
    x=shape->sqr[0].posi.x;

    for(i=0;i<5;i++)
    {
        for(j=0;j<5;j++)
        {
            shape->sqr[i*5+j].posi.x=x+25*j;
            shape->sqr[i*5+j].posi.y=y+25*i;
        }
    }
}

void initiate_tablet(tablet_t* shape_green,tablet_t *shape_red ,tablet_t *shape_blue,tablet_t *shape_yellow)
{
    int i=0;
    /* BLUE*/
    shape_blue->ta[0].sqr[0].posi.x=100;
    shape_blue->ta[0].sqr[0].posi.y=250;
    shape_blue->ta[1].sqr[0].posi.x=49;
    shape_blue->ta[1].sqr[0].posi.y=300;
    shape_blue->ta[2].sqr[0].posi.x=49;
    shape_blue->ta[2].sqr[0].posi.y=300-51;
    shape_blue->ta[3].sqr[0].posi.x=49-26;
    shape_blue->ta[3].sqr[0].posi.y=300-76;
    shape_blue->ta[4].sqr[0].posi.x=49+50;
    shape_blue->ta[4].sqr[0].posi.y=300-101;
    shape_blue->ta[5].sqr[0].posi.x=49-26;
    shape_blue->ta[5].sqr[0].posi.y=300-152;
    shape_blue->ta[6].sqr[0].posi.x=49+24;
    shape_blue->ta[6].sqr[0].posi.y=300-151-26;
    shape_blue->ta[7].sqr[0].posi.x=49+50+25;
    shape_blue->ta[7].sqr[0].posi.y=300-151-51;
    shape_blue->ta[8].sqr[0].posi.x=49-26;
    shape_blue->ta[8].sqr[0].posi.y=300-151-102;
    shape_blue->ta[9].sqr[0].posi.x=49-27+25+1;
    shape_blue->ta[9].sqr[0].posi.y=300-151-103;
    shape_blue->ta[10].sqr[0].posi.x=49-27+51;
    shape_blue->ta[10].sqr[0].posi.y=300-151-102;
    shape_blue->ta[11].sqr[0].posi.x=49+50+25;
    shape_blue->ta[11].sqr[0].posi.y=300-151-51-51;
    shape_blue->ta[12].sqr[0].posi.x=49+50+25+51+25;
    shape_blue->ta[12].sqr[0].posi.y=300-151-51-25;
    shape_blue->ta[13].sqr[0].posi.x=49+50+25+51;
    shape_blue->ta[13].sqr[0].posi.y=300-151-25-102;
    shape_blue->ta[14].sqr[0].posi.x=49+50+25+127;
    shape_blue->ta[14].sqr[0].posi.y=300-151-51-50;
    shape_blue->ta[15].sqr[0].posi.x=49+50+25+178;
    shape_blue->ta[15].sqr[0].posi.y=300-151-51-25;
    shape_blue->ta[16].sqr[0].posi.x=49+50+25+178;
    shape_blue->ta[16].sqr[0].posi.y=300-151-101;
    shape_blue->ta[17].sqr[0].posi.x=49+50+25+178;
    shape_blue->ta[17].sqr[0].posi.y=300-151-127;
    shape_blue->ta[18].sqr[0].posi.x=49+50+25+102;
    shape_blue->ta[18].sqr[0].posi.y=300-151-127;
    shape_blue->ta[19].sqr[0].posi.x=49+50+25;
    shape_blue->ta[19].sqr[0].posi.y=300-151-51-101;
    shape_blue->ta[20].sqr[0].posi.x=48;
    shape_blue->ta[20].sqr[0].posi.y=50-28;
    for(i=0;i<21;i++)
    {
        shape_blue->ta[i].sqr[0].posi.x-=40;
        shape_blue->ta[i].sqr[0].posi.y-=30;
    }
    /*GREEN*/

    shape_green->ta[0].sqr[0].posi.x=100;
    shape_green->ta[0].sqr[0].posi.y=250;
    shape_green->ta[1].sqr[0].posi.x=49;
    shape_green->ta[1].sqr[0].posi.y=200;
    shape_green->ta[2].sqr[0].posi.x=49;
    shape_green->ta[2].sqr[0].posi.y=251;
    shape_green->ta[3].sqr[0].posi.x=49-26;
    shape_green->ta[3].sqr[0].posi.y=251+76;
    shape_green->ta[4].sqr[0].posi.x=49+50;
    shape_green->ta[4].sqr[0].posi.y=251+101;
    shape_green->ta[5].sqr[0].posi.x=49-26;
    shape_green->ta[5].sqr[0].posi.y=251+152;
    shape_green->ta[6].sqr[0].posi.x=49+24;
    shape_green->ta[6].sqr[0].posi.y=251+151+26;
    shape_green->ta[7].sqr[0].posi.x=49+50+25;
    shape_green->ta[7].sqr[0].posi.y=251+151+51;
    shape_green->ta[8].sqr[0].posi.x=49-26;
    shape_green->ta[8].sqr[0].posi.y=251+151+102;
    shape_green->ta[9].sqr[0].posi.x=49-27+25+1;
    shape_green->ta[9].sqr[0].posi.y=251+151+103;
    shape_green->ta[10].sqr[0].posi.x=49-27+51;
    shape_green->ta[10].sqr[0].posi.y=251+151+102;
    shape_green->ta[11].sqr[0].posi.x=49+50+25;
    shape_green->ta[11].sqr[0].posi.y=251+151+51+51;
    shape_green->ta[12].sqr[0].posi.x=49+50+25+51+25;
    shape_green->ta[12].sqr[0].posi.y=251+151+51+25;
    shape_green->ta[13].sqr[0].posi.x=49+50+25+51;
    shape_green->ta[13].sqr[0].posi.y=251+151+25+102;
    shape_green->ta[14].sqr[0].posi.x=49+50+25+127;
    shape_green->ta[14].sqr[0].posi.y=251+151+51+50;
    shape_green->ta[15].sqr[0].posi.x=49+50+25+178;
    shape_green->ta[15].sqr[0].posi.y=251+151+51+25;
    shape_green->ta[16].sqr[0].posi.x=49+50+25+178;
    shape_green->ta[16].sqr[0].posi.y=251+151+101;
    shape_green->ta[17].sqr[0].posi.x=49+50+25+178;
    shape_green->ta[17].sqr[0].posi.y=251+151+127;
    shape_green->ta[18].sqr[0].posi.x=49+50+25+102;
    shape_green->ta[18].sqr[0].posi.y=251+151+127;
    shape_green->ta[19].sqr[0].posi.x=49+50+25;
    shape_green->ta[19].sqr[0].posi.y=251+151+51+101;
    shape_green->ta[20].sqr[0].posi.x=48;
    shape_green->ta[20].sqr[0].posi.y=251+151+26+101;

    for(i=0;i<21;i++)
    {
        shape_green->ta[i].sqr[0].posi.x-=40;
        shape_green->ta[i].sqr[0].posi.y+=200;
    }
    for(i=3;i<21;i++)
    {
        shape_green->ta[i].sqr[0].posi.y-=51;
    }
    /*yellow*/

    shape_yellow->ta[0].sqr[0].posi.x=100;
    shape_yellow->ta[0].sqr[0].posi.y=250;
    shape_yellow->ta[1].sqr[0].posi.x=100+51;
    shape_yellow->ta[1].sqr[0].posi.y=300;
    shape_yellow->ta[2].sqr[0].posi.x=100+51;
    shape_yellow->ta[2].sqr[0].posi.y=300-51;
    shape_yellow->ta[3].sqr[0].posi.x=100+51+26;
    shape_yellow->ta[3].sqr[0].posi.y=300-76;
    shape_yellow->ta[4].sqr[0].posi.x=100+51-50;
    shape_yellow->ta[4].sqr[0].posi.y=300-101;
    shape_yellow->ta[5].sqr[0].posi.x=100+51+26;
    shape_yellow->ta[5].sqr[0].posi.y=300-152;
    shape_yellow->ta[6].sqr[0].posi.x=100+51-24;
    shape_yellow->ta[6].sqr[0].posi.y=300-151-26;
    shape_yellow->ta[7].sqr[0].posi.x=100+51-50-25;
    shape_yellow->ta[7].sqr[0].posi.y=300-151-51;
    shape_yellow->ta[8].sqr[0].posi.x=100+51+26;
    shape_yellow->ta[8].sqr[0].posi.y=300-151-102;
    shape_yellow->ta[9].sqr[0].posi.x=100+51+27-26;
    shape_yellow->ta[9].sqr[0].posi.y=300-151-103;
    shape_yellow->ta[10].sqr[0].posi.x=100+51+27-51;
    shape_yellow->ta[10].sqr[0].posi.y=300-151-102;
    shape_yellow->ta[11].sqr[0].posi.x=100+51-50-25;
    shape_yellow->ta[11].sqr[0].posi.y=300-151-51-51;
    shape_yellow->ta[12].sqr[0].posi.x=100+51-50-25-51-25;
    shape_yellow->ta[12].sqr[0].posi.y=300-151-51-25;
    shape_yellow->ta[13].sqr[0].posi.x=100+51-50-25-51;
    shape_yellow->ta[13].sqr[0].posi.y=300-151-25-102;
    shape_yellow->ta[14].sqr[0].posi.x=100+51-50-25-127;
    shape_yellow->ta[14].sqr[0].posi.y=300-151-51-50;
    shape_yellow->ta[15].sqr[0].posi.x=100+51-50-25-178;
    shape_yellow->ta[15].sqr[0].posi.y=300-151-51-25;
    shape_yellow->ta[16].sqr[0].posi.x=100+51-50-25-178;
    shape_yellow->ta[16].sqr[0].posi.y=300-151-101;
    shape_yellow->ta[17].sqr[0].posi.x=100+51-50-25-178;
    shape_yellow->ta[17].sqr[0].posi.y=300-151-127;
    shape_yellow->ta[18].sqr[0].posi.x=100+51-50-25-102;
    shape_yellow->ta[18].sqr[0].posi.y=300-151-127;
    shape_yellow->ta[19].sqr[0].posi.x=100+51-50-25;
    shape_yellow->ta[19].sqr[0].posi.y=300-151-51-101;
    shape_yellow->ta[20].sqr[0].posi.x=100+52;
    shape_yellow->ta[20].sqr[0].posi.y=50-28;
    for(i=0;i<21;i++)
    {
        shape_yellow->ta[i].sqr[0].posi.x+=550;
        shape_yellow->ta[i].sqr[0].posi.y-=30;
    }
    /*RED*/
    shape_red->ta[0].sqr[0].posi.x=100;
    shape_red->ta[0].sqr[0].posi.y=250;
    shape_red->ta[1].sqr[0].posi.x=100+51;
    shape_red->ta[1].sqr[0].posi.y=200;
    shape_red->ta[2].sqr[0].posi.x=100+51;
    shape_red->ta[2].sqr[0].posi.y=200+51;
    shape_red->ta[3].sqr[0].posi.x=100+51+26;
    shape_red->ta[3].sqr[0].posi.y=200+76;
    shape_red->ta[4].sqr[0].posi.x=100+51-50;
    shape_red->ta[4].sqr[0].posi.y=200+101;
    shape_red->ta[5].sqr[0].posi.x=100+51+26;
    shape_red->ta[5].sqr[0].posi.y=200+152;
    shape_red->ta[6].sqr[0].posi.x=100+51-24;
    shape_red->ta[6].sqr[0].posi.y=200+151+26;
    shape_red->ta[7].sqr[0].posi.x=100+51-50-25;
    shape_red->ta[7].sqr[0].posi.y=200+151+51;
    shape_red->ta[8].sqr[0].posi.x=100+51+26;
    shape_red->ta[8].sqr[0].posi.y=200+151+102;
    shape_red->ta[9].sqr[0].posi.x=100+51+27-26;
    shape_red->ta[9].sqr[0].posi.y=200+151+103;
    shape_red->ta[10].sqr[0].posi.x=100+51+27-51;
    shape_red->ta[10].sqr[0].posi.y=200+151+102;
    shape_red->ta[11].sqr[0].posi.x=100+51-50-25;
    shape_red->ta[11].sqr[0].posi.y=200+151+51+51;
    shape_red->ta[12].sqr[0].posi.x=100+51-50-25-51-25;
    shape_red->ta[12].sqr[0].posi.y=200+151+51+25;
    shape_red->ta[13].sqr[0].posi.x=100+51-50-25-51;
    shape_red->ta[13].sqr[0].posi.y=200+151+25+102;
    shape_red->ta[14].sqr[0].posi.x=100+51-50-25-127;
    shape_red->ta[14].sqr[0].posi.y=200+151+51+50;
    shape_red->ta[15].sqr[0].posi.x=100+51-50-25-178;
    shape_red->ta[15].sqr[0].posi.y=200+151+51+25;
    shape_red->ta[16].sqr[0].posi.x=100+51-50-25-178;
    shape_red->ta[16].sqr[0].posi.y=200+151+101;
    shape_red->ta[17].sqr[0].posi.x=100+51-50-25-178;
    shape_red->ta[17].sqr[0].posi.y=200+151+127;
    shape_red->ta[18].sqr[0].posi.x=100+51-50-25-102;
    shape_red->ta[18].sqr[0].posi.y=200+151+127;
    shape_red->ta[19].sqr[0].posi.x=100+51-50-25;
    shape_red->ta[19].sqr[0].posi.y=200+151+51+101;
    shape_red->ta[20].sqr[0].posi.x=100+52;
    shape_red->ta[20].sqr[0].posi.y=200+151+26+101;
    for(i=0;i<21;i++)
    {
        shape_red->ta[i].sqr[0].posi.x+=550;
        shape_red->ta[i].sqr[0].posi.y+=(149+51);
    }


}

void reflexion(int type,shape_t *shape)
{
    sqr_e color;

    int i=0,j=0;

    if(type==HORIZONTAL)
    {
        for(i=0;i<5;i++)
        {
            for(j=0;j<2;j++)
            {
                color=shape->sqr[i*5+j].clr;
                shape->sqr[i*5+j].clr=shape->sqr[i*5+4-j].clr;
                shape->sqr[i*5+4-j].clr=color;
            }
        }
    }
    else if(type==VERTICAL)
    {
       for(i=0;i<2;i++)
        {
            for(j=0;j<5;j++)
            {
                color=shape->sqr[i*5+j].clr;
                shape->sqr[i*5+j].clr=shape->sqr[(4-i)*5+j].clr;
                shape->sqr[(4-i)*5+j].clr=color;
            }
        }
    }
    else if(type==HORI_VERTI)
    {
       for(i=0;i<5;i++)
        {
            for(j=0;j<2;j++)
            {
                color=shape->sqr[i*5+j].clr;
                shape->sqr[i*5+j].clr=shape->sqr[i*5+4-j].clr;
                shape->sqr[i*5+4-j].clr=color;
            }
        }

       for(i=0;i<2;i++)
        {
            for(j=0;j<5;j++)
            {
                color=shape->sqr[i*5+j].clr;
                shape->sqr[i*5+j].clr=shape->sqr[(4-i)*5+j].clr;
                shape->sqr[(4-i)*5+j].clr=color;
            }
        }
    }
}

void create_table(SDL_Surface* screen)
{
        int i=0,j=0;
        SDL_Surface* table=NULL;
        SDL_Rect posi;

        table=IMG_Load("tablet.png");
        posi.x=168;
        posi.y=147;
        SDL_BlitSurface(table,NULL,screen,&posi);

        SDL_FreeSurface(table);
}

color_square_t detect_shape(SDL_Event even,tablet_t shape_green,tablet_t shape_red ,tablet_t shape_blue,tablet_t shape_yellow)
{
     color_square_t cs;
     cs.cl=VOID;
     cs.numb=0;
     cs.typ=21;
     int i=0,j=0;


         for(i=0;i<21;i++)
         {
             for(j=0;j<25;j++)
             {
                 if(shape_blue.ta[i].sqr[j].posi.x<even.button.x && shape_blue.ta[i].sqr[j].posi.x + 25 > even.button.x && shape_blue.ta[i].sqr[j].posi.y<even.button.y && shape_blue.ta[i].sqr[j].posi.y+25>even.button.y && shape_blue.ta[i].sqr[j].clr!=VOID && shape_blue.ta[i].pl!=1)
                 {
                     cs.cl=BLUE; cs.typ=i; cs.numb=j; return cs;
                 }
                 else if(shape_green.ta[i].sqr[j].posi.x<even.button.x && shape_green.ta[i].sqr[j].posi.x + 25 > even.button.x && shape_green.ta[i].sqr[j].posi.y<even.button.y && shape_green.ta[i].sqr[j].posi.y+25>even.button.y && shape_green.ta[i].sqr[j].clr!=VOID && shape_green.ta[i].pl!=1)
                 {
                     cs.cl=GREEN; cs.typ=i; cs.numb=j; return cs;
                 }
                 else if(shape_red.ta[i].sqr[j].posi.x<even.button.x && shape_red.ta[i].sqr[j].posi.x + 25 > even.button.x && shape_red.ta[i].sqr[j].posi.y<even.button.y && shape_red.ta[i].sqr[j].posi.y+25>even.button.y && shape_red.ta[i].sqr[j].clr!=VOID && shape_red.ta[i].pl!=1)
                 {
                     cs.cl=RED; cs.typ=i; cs.numb=j; return cs;
                 }
                 else if(shape_yellow.ta[i].sqr[j].posi.x<even.button.x && shape_yellow.ta[i].sqr[j].posi.x + 25 > even.button.x && shape_yellow.ta[i].sqr[j].posi.y<even.button.y && shape_yellow.ta[i].sqr[j].posi.y+25>even.button.y && shape_yellow.ta[i].sqr[j].clr!=VOID && shape_yellow.ta[i].pl!=1)
                 {
                     cs.cl=YELOW; cs.typ=i; cs.numb=j; return cs;
                 }
             }
         }
         return cs;

}

int detect_station_void(SDL_Event even,tablet_t shape_green,tablet_t shape_red ,tablet_t shape_blue,tablet_t shape_yellow)
{
    int i=0,j=0;
    for(i=0;i<21;i++)
    {
        for(j=0;j<25;j++)
        {
            if(shape_blue.ta[i].sqr[j].posi.x<even.button.x && shape_blue.ta[i].sqr[j].posi.x + 25 > even.button.x && shape_blue.ta[i].sqr[j].posi.y<even.button.y && shape_blue.ta[i].sqr[j].posi.y+25>even.button.y && shape_blue.ta[i].sqr[j].clr!=VOID)
            {
                return 0;
            }
        }
    }

    for(i=0;i<21;i++)
    {
        for(j=0;j<25;j++)
        {
            if(shape_green.ta[i].sqr[j].posi.x<even.button.x && shape_green.ta[i].sqr[j].posi.x + 25 > even.button.x && shape_green.ta[i].sqr[j].posi.y<even.button.y && shape_green.ta[i].sqr[j].posi.y+25>even.button.y && shape_green.ta[i].sqr[j].clr!=VOID)
            {
                return 0;
            }
        }
    }

    for(i=0;i<21;i++)
    {
        for(j=0;j<25;j++)
        {
            if(shape_red.ta[i].sqr[j].posi.x<even.button.x && shape_red.ta[i].sqr[j].posi.x + 25 > even.button.x && shape_red.ta[i].sqr[j].posi.y<even.button.y && shape_red.ta[i].sqr[j].posi.y+25>even.button.y && shape_red.ta[i].sqr[j].clr!=VOID)
            {
                return 0;
            }
        }
    }

    for(i=0;i<21;i++)
    {
        for(j=0;j<25;j++)
        {
             if(shape_yellow.ta[i].sqr[j].posi.x<even.button.x && shape_yellow.ta[i].sqr[j].posi.x + 25 > even.button.x && shape_yellow.ta[i].sqr[j].posi.y<even.button.y && shape_yellow.ta[i].sqr[j].posi.y+25>even.button.y && shape_yellow.ta[i].sqr[j].clr!=VOID)
            {
                return 0;
            }
        }
    }

    return 1;
}

void shape_place(tablet_t *shape,int typ)
{
    shape->ta[typ].pl=1;
}

void table_init(tables_t *tabl)
{
    int i=0,j=0;
    for(i=0;i<20;i++)
    {
        for(j=0;j<20;j++)
        {
            tabl->tab[i*20+j].au=VOI;
            tabl->tab[i*20+j].pos.x=168+j*25;
            tabl->tab[i*20+j].pos.y=147+i*25;
            tabl->tab[i*20+j].blu_p=1;
            tabl->tab[i*20+j].gre_p=1;
            tabl->tab[i*20+j].red_p=1;
            tabl->tab[i*20+j].yel_p=1;
        }

    }

    tabl->tab[0].au=P_BLUE;
}
