#include "jde.h"
#include "pioneer.h"
#include "forms.h"
#include "introrob.h"

void yourcode_iteration(void)
{
  v=0;
  w=40;
  /*  printf("laser[90]=%.2f, X=%.2f, Y=%.2f, Theta=%.2f\n",laser[90],robot[0],robot[1],robot[2]*RADTODEG);*/
  /*  printf("pixel central, fila inferior: R=%d G=%d B=%d\n",
	 (int)(unsigned char)imagenRGB[(239*SIFNTSC_COLUMNS+159)*3+2],
	 (int)(unsigned char)imagenRGB[(239*SIFNTSC_COLUMNS+159)*3+1],
	 (int)(unsigned char)imagenRGB[(239*SIFNTSC_COLUMNS+159)*3]);*/
}

void visualizacion(void)     
{ 
  Tvoxel aa;
  static Tvoxel a,b;
  static Tvoxel c,d;

  /* example of relative segment */
  /* borra la el segmento pintado en la ultima iteracion */
  pintaSegmento(a,b,FL_WHITE);

  /* calcula nueva posicion absoluta del segmento */
  aa.x=500.; aa.y=500.; /* en mm */
  relativas2absolutas(aa,&a);
  aa.x=0.; aa.y=0.;
  relativas2absolutas(aa,&b);
  /* pinta la nueva posicion del segmento */
  pintaSegmento(a,b,FL_RED);

  /* ejemplo segmento absoluto */
  /* borra la el segmento pintado en la ultima iteracion */
  pintaSegmento(c,d,FL_WHITE); 
  c.x=0.; c.y=0.; /* en mm */
  d.x=robot[0]; d.y=robot[1]; /* en mm */
  pintaSegmento(c,d,FL_BLUE);
}