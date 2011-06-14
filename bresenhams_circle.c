#include "SDL/SDL.h"
#include <stdio.h>

enum 
{
  SCREENWIDTH = 512,
  SCREENHEIGHT = 384,
  SCREENBPP = 0,
  SCREENFLAGS = SDL_ANYFORMAT
} ;

void SetPixel ( SDL_Surface* pSurface , int x , int y , SDL_Color color ) ;
SDL_Color GetPixel ( SDL_Surface* pSurface , int x , int y ) ;
void foo();
SDL_Surface* pSurface;

int main( int argc, char* argv[] )
{
  //initialize systems
  SDL_Init ( SDL_INIT_VIDEO ) ;

  //set our at exit function
  atexit ( SDL_Quit ) ;

  //create a window
  pSurface = SDL_SetVideoMode ( SCREENWIDTH , SCREENHEIGHT ,
                                             SCREENBPP , SCREENFLAGS ) ;

  //declare event variable
  SDL_Event event ;

  // flag defined to stop looping foo()
  int flag = 1;
	
  //message pump
  for ( ; ; )
  {
    //look for an event
    if ( SDL_PollEvent ( &event ) )
    {
      //an event was found
      if ( event.type == SDL_QUIT ) break ;
    }
	if(flag)
	{
		foo();
		flag = 0;
	}   
  }//end of message pump

  //done
  return ( 0 ) ;
}

void SetPixel ( SDL_Surface* pSurface , int x , int y , SDL_Color color ) 
{
  //convert color
  Uint32 col = SDL_MapRGB ( pSurface->format , color.r , color.g , color.b ) ;

  //determine position
  char* pPosition = ( char* ) pSurface->pixels ;

  //offset by y
  pPosition += ( pSurface->pitch * y ) ;

  //offset by x
  pPosition += ( pSurface->format->BytesPerPixel * x ) ;

  //copy pixel data
  memcpy ( pPosition , &col , pSurface->format->BytesPerPixel ) ;
}
SDL_Color GetPixel ( SDL_Surface* pSurface , int x , int y ) 
{
  SDL_Color color ;
  Uint32 col = 0 ;

  //determine position
  char* pPosition = ( char* ) pSurface->pixels ;

  //offset by y
  pPosition += ( pSurface->pitch * y ) ;

  //offset by x
  pPosition += ( pSurface->format->BytesPerPixel * x ) ;

  //copy pixel data
  memcpy ( &col , pPosition , pSurface->format->BytesPerPixel ) ;

  //convert color
  SDL_GetRGB ( col , pSurface->format , &color.r , &color.g , &color.b ) ;
  return ( color ) ;
}

void CirclePlotPoints(int xc, int yc, int x, int y, SDL_Color color)
{
	SetPixel(pSurface, xc+x,yc+y, color);	
	SetPixel(pSurface, xc-x,yc+y, color);
	SetPixel(pSurface, xc+x,yc-y, color);
	SetPixel(pSurface, xc-x,yc-y, color);
	SetPixel(pSurface, xc+y,yc+x, color);
	SetPixel(pSurface, xc-y,yc+x, color);
	SetPixel(pSurface, xc+y,yc-x, color);
	SetPixel(pSurface, xc-y,yc-x, color);
}

void BresCircle( int xc, int yc, int r, SDL_Color color) 
{
	int x = 0;
	int y = r;
	int d = 3 - 2*r;
	
	CirclePlotPoints(xc,yc,x,y,color);
	
	while(x<y)
	{		
		if(d<0)
			d += 4*x + 6;
		else
		{
			d += 4* (x-y) + 10;			
			y--;
		}	
		x++;		
		
		CirclePlotPoints(xc,yc,x,y,color);
	}
	
}

void foo()
{
	int r,xc,yc;
	printf("Enter the values of xc and yc:\n");
  	scanf("%d %d",&xc,&yc); 	
	printf("Enter the Radius:\n");
	scanf("%d", &r);
	
	SDL_Color color ;
	color.r = rand ( ) % 256 ;
	color.g = rand ( ) % 256 ;
	color.b = rand ( ) % 256 ;
	
	BresCircle(xc,yc,r,color);
}
