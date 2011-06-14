#include "SDL/SDL.h"
#include <stdio.h>

struct Point 
    {			
	  int xco;
	  int yco;
    };


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




void lineMid( struct Point * p1, struct Point *p2, SDL_Color color) 
{
	int dx = abs(p1->xco - p2->xco);  
	int dy = abs(p1->yco - p2->yco);
	int D = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyDx = 2* (dy-dx);
	int x,y,xEnd;

	if(p1->xco > p2->xco)
	{
		x = p2->xco;
		y = p2->yco;
		xEnd = p1->xco;		
	}
	else
	{
		x = p1->xco;
		y = p1->yco;
		xEnd = p2->xco;
	}		
	
	SetPixel(pSurface, x, y, color);
	
	while(x < xEnd)
	{
		x++;
		if(D<0)
			D += twoDy;
		else
		{
			y++;
			D += twoDyDx;			
		}			
		
		SetPixel(pSurface, x,y, color);
	}
}


void foo()
{
	struct Point p1,p2;
	printf("Enter the values of x1 and y1:\n");
  	scanf("%d %d",&p1.xco,&p1.yco);
 	printf("Enter the values of x2 and y2:\n");
  	scanf("%d %d",&p2.xco,&p2.yco);
	
	SDL_Color color ;
    color.r = rand ( ) % 256 ;
    color.g = rand ( ) % 256 ;
    color.b = rand ( ) % 256 ;
	
	lineMid(&p1,&p2,color);
}
