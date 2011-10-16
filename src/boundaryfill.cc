#include "SDL/SDL.h"
#include <iostream>
using namespace std;
SDL_Surface* pSurface;

enum 
{
  SCREENWIDTH = 512,
  SCREENHEIGHT = 384,
  SCREENBPP = 0,
  SCREENFLAGS = SDL_ANYFORMAT  
} ;

class Start
{
	public:
		
	static void Draw()
	{
		int r,xc,yc;
		cout<<"Enter the values of xc and yc:\n";
		cin>>xc>>yc;	
		cout<<"Enter the Radius:\n";
		cin>>r;
		
		SDL_Color color ;
		color.r = 0 ;
		color.g = 255 ;
		color.b = 0 ;
		
		BresCircle(xc,yc,r,color);
	}
	
	static void CirclePlotPoints(int xc, int yc, int x, int y, SDL_Color color)
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

	static void BresCircle( int xc, int yc, int r, SDL_Color color) 
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
		
		// Boundary Fill Specific
		// -------------------------
		SDL_Color color1 ;
		color1.r = 255 ;
		color1.g = 0 ;
		color1.b = 0 ;
		// -------------------------
		
		BoundaryFill(xc,yc,color1,color);					
	}
	
	static void BoundaryFill(int x, int y, SDL_Color fill, SDL_Color boundary)
	{
		SDL_Color current = GetPixel(pSurface,x,y);
		
		if(!CompareColor(current,boundary) && !CompareColor(current,fill))
		{
			SetPixel(pSurface,x,y,fill);
			BoundaryFill(x+1,y,fill,boundary);
			BoundaryFill(x-1,y,fill,boundary);
			BoundaryFill(x,y+1,fill,boundary);
			BoundaryFill(x,y-1,fill,boundary);					
		}
	}
	
	static bool CompareColor(SDL_Color c1, SDL_Color c2)
	{
		if(c1.r == c2.r && c1.g == c2.g && c1.b == c2.b)
			return true;
		else
			return false;	
	}
	
	static void SetPixel ( SDL_Surface* pSurface , int x , int y , SDL_Color color ) 
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
	
	static SDL_Color GetPixel ( SDL_Surface* pSurface , int x , int y ) 
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

};

class SDLInit
{
	public:

	static void Init()
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
				Start::Draw(); 				// My coding!
				flag = 0;
			}   
		  }//end of message pump

		  //done
	}
	
	
};

int main()
{
  SDLInit::Init(); 
  return ( 0 ) ;
}
