/*******************************************************************/
/* vtemp      Compute local max operation on a single byte image   */
/*******************************************************************/

#include "VisXV4.h"           /* VisionX structure include file    */
#include "Vutil.h"            /* VisionX utility header files      */

VXparam_t par[] =             /* command line structure            */
{ /* prefix, value,   description                         */   
{    "if=",    0,   " input file  vtemp: local max filter "},
{    "of=",    0,   " output file "},
{     0,       0,   0}  /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
/*void lmax(int, int); */
/* Blanca add the declaration of setlable function */
void setlable(int, int, int);

Vfstruct (im);                      /* i/o image structure          */
Vfstruct (tm);                      /* temp image structure         */

main(argc, argv)
int argc;
char *argv[];
{
int        y,x;                     /* index counters               */
  VXparse(&argc, &argv, par);       /* parse the command line       */

  Vfread(&im, IVAL);                /* read image file              */
  Vfembed(&tm, &im, 1,1,1,1);       /* image structure with border  */
  if ( im.type != VX_PBYTE ) {      /* check image format           */
     fprintf(stderr, "vtemp: no byte image data in input file\n");
     exit(-1);
  }
/*  for (y = im.ylo ; y <= im.yhi ; y++) {
     for (x = im.xlo ; x <= im.xhi; x++)  {
           lmax(x, y);
     }
   }
 */ 
   
   // <-- Blanca's code
   /* preset all pixels in im to 0 implies unlabled */
   for (y = im.ylo ; y <= im.yhi ; y++){
     for (x = im.xlo ; x <= im.xhi ; x++){
       im.u[y][x] = 0;
     }
   }
   
   /* when meet object and check isunlabled in im, then use lable function to lable it    */
   int i = 1;
   for (y = tm.ylo ; y <= tm.yhi ; y++){
     for(x = tm.ylo ; x <= tm.xhi ; x++){
       if(tm.u[y][x] != 0 && im.u[y + 1][x + 1] == 0){
         setlable(x, y, i);
         i++;
       }
     }
   }
   //Blanca's code -->

   Vfwrite(&im, OVAL);             /* write image file                */
   exit(0);
}

/* function to compute the local maximum */
/*
 * void lmax(int x, int y) 
{						
    im.u[y][x] = MAX(tm.u[y][x],
                  MAX(tm.u[y][x+1],
                   MAX(tm.u[y+1][x],
                    MAX(tm.u[y][x-1],
                            tm.u[y-1][x]))));
}
*/

//<-- Blanca's code
/* setlable(x, y, l) function here   */
void setlable(int x, int y, int l){
  im.u[x][y] = l;
  if(im.u[x][y + 1] == 0 && tm.u[x][y + 1] != 0){
    setlable(x, y + 1, l);
  }
  else if(im.u[x][y - 1] == 0 && tm.u[x][y - 1] != 0){
    setlable(x, y - 1, l);
  }
  else if(im.u[x - 1][y] == 0 && tm.u[x - 1][y] != 0){
    setlable(x - 1, y, l);
  }
  else if(im.u[x + 1][y] == 0 && tm.u[x + 1][y] != 0){
    setlable(x + 1, y, l);
  }
}

//Blanca's code -->
