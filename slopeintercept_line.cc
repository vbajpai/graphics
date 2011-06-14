#include "SDL/SDL.h"
#include <iostream>
#define round(x) (int)((x)+0.5)
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
	private:
		
	struct Point 
	{			
	  int xco;
	  int yco;
    };
    
	public:
		
	static void Draw()
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
		
		Simpleline(&p1,&p2,color);
	}
	
	static void Simpleline( struct Point * p1, struct Point *p2, SDL_Color color) 
	{

	  int dx=p2->xco - p1->xco;
	  int dy=p2->yco - p1->yco;
	  
	  int x= p1->xco;
	  int y= p1->yco;	
		
	  SetPixel(pSurface,round(x),round(y),color);
			
	  if (dx != 0) 
	  {
				float m = (float) dy / (float) dx;
				float b = y - m*x;
		  
				dx = (p2->xco > x) ? 1 : -1;
		  
				while (x != p2->xco) 
				{
					x += dx;
					y  = round(m*x + b);
					SetPixel(pSurface,round(x),round(y),color);
				}
	   }
	  
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

