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
	   int num=10;
	   
	   Point P[10] = {{220,340} , {220,220} , {250,170} , {270,200} ,
					{300,140} , {320,240} , {320,290} , {420,220} ,
					{420,340} , {220,340}};
	   					      
		
	   SDL_Color color ;
	   color.r = rand ( ) % 256 ;
	   color.g = rand ( ) % 256 ;
	   color.b = rand ( ) % 256 ;
	   
       PolygonDraw(num,P,color);		
       PolygonFill(230,220,color);
	}
	
	static void PolygonDraw(int n, Point points[],SDL_Color color)
    {
      if(n>=2)
	  {
	     for(int count=0;count<n-1;count++)
			lineDDA(&points[count],&points[count+1],color);	 	
	  }
    }
    
    static void PolygonFill(int xc,int yc, SDL_Color color)
    {
    	
		SDL_Color color1 ;		// Black
		color1.r = 0 ;
		color1.g = 0 ;
		color1.b = 0 ;	
		
		FloodFill(xc,yc,color,color1);	
    }

	static void lineDDA( struct Point * p1, struct Point *p2, SDL_Color color) 
	{

	  int dx,dy,steps,k;
	  float xinc,yinc,x,y;
		
	  dx=p2->xco - p1->xco;
	  dy=p2->yco - p1->yco;
		
	  if(abs(dx) > abs(dy))
		steps=abs(dx);
	  else
		steps=abs(dy);
	  
	  xinc=(float)dx/(float)steps;
	  yinc=(float)dy/(float)steps;
	  x=p1->xco;
	  y=p1->yco;
	  
	  SetPixel(pSurface,round(x),round(y),color);
	  
	  for(k=0;k<steps;k++) 
	  {
		x+=xinc;
		y+=yinc;
		SetPixel(pSurface,round(x),round(y),color);
	  }
	}
	
	static void FloodFill(int x, int y, SDL_Color fill, SDL_Color oldcolor)
	{
		SDL_Color current = GetPixel(pSurface,x,y);
		
		if(CompareColor(current,oldcolor))
		{
			SetPixel(pSurface,x,y,fill);
			FloodFill(x+1,y,fill,oldcolor);
			FloodFill(x-1,y,fill,oldcolor);
			FloodFill(x,y+1,fill,oldcolor);
			FloodFill(x,y-1,fill,oldcolor);					
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
