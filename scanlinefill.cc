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

class Data
{
	public:
		
	struct Point 
	{			
	  int x;
	  int y;
    };
    
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

class ScanFill: public Data
{
	private:
	
	struct Edge
	{
		int yUpper;
		float xIntersect, dxPerScan;
		struct Edge *next;	
	};
	
	public:
	
	static void resortActiveList(Edge * active)
	{
		Edge *q, *p = active->next;
		
		active->next = NULL;
		
		while(p!=NULL)
		{
			q = p->next;
			insertEdge(active,p);
			p = q;	
		}			
	}
	
	static void deleteAfter(Edge *q)
	{
		Edge *p = q->next;
		
		q->next = p->next;
		delete(p);	
	}	
	
	static void updateActiveList(int scan, Edge * active)
	{
		Edge *q = active, *p = active->next;
		
		while(p!=NULL)
			if(scan >= p->yUpper)
			{
				p = p->next;
				deleteAfter(q);
			}
			else
			{
				p->xIntersect = p->xIntersect + p->dxPerScan;
				q = p;
				p = p->next;
			}		
	}
	
	static void fillScan(int scan, Edge *active, SDL_Color color)
	{
		Edge *p1, *p2;
		
		p1 = active->next;
		
		while(p1!= NULL)
		{
			p2 = p1->next;
			
			for(int i=(int)p1->xIntersect; i< p2->xIntersect; i++)	
					SetPixel(pSurface, (int)i, scan, color);	
					
			p1 = p2->next;
			
		}	
	}
	
	static void buildActiveList(int scan, Edge *active, Edge * edges[])
	{
		Edge *p, *q;
		
		p = edges[scan] ->next;
		
		while(p)
		{
			q = p->next;
			insertEdge(active,p);
			p = q;	
		}
	}
	
	static void insertEdge(Edge *list, Edge *edge)
	{
		Edge *p, *q = list;
		
		p = q->next;
		
		while(p!=NULL)
		{
			if(edge->xIntersect < p->xIntersect)
				p = NULL;
			else
			{
				q = p;
				p = p->next;	
			}	
		}	
		
		edge->next = q->next;
		q->next = edge;
	}
	
	static int yNext(int k, int cnt, Point *p)
	{
		int j;
		
		if((k+1) > (cnt-1))	
			j=0;
		else
			j = k+1;
			
		while(p[k].y == p[j].y)
			if((j+1) > (cnt-1))
				j=0;
			else
				j++;
		return (p[j].y);
	}
	
	static void makeEdgeRec(Point lower, Point upper, int yComp, Edge *edge, Edge *edges[])
	{
		edge->dxPerScan = (float) (upper.x - lower.x) / (upper.y - lower.y);
		edge->xIntersect = lower.x;
		
		if(upper.y <yComp)
			edge->yUpper = upper.y - 1;
		else
			edge->yUpper = upper.y;
			
		insertEdge(edges[lower.y], edge);
	}
	
	static void buildEdgeList(int cnt, Point *p, Edge * edges[])
	{
		Point v1,v2;
		
		int yPrev = p[cnt-2].y;
		
		v1.x = p[cnt-1].x;
		v1.y = p[cnt-1].y;
		
		for(int i=0;i<cnt;i++)
		{
			v2 = p[i];
			if(v1.y != v2.y)
			{
				Edge *edge = new Edge;
				if(v1.y < v2.y)
					makeEdgeRec(v1,v2,yNext(i,cnt,p),edge,edges);
				else
					makeEdgeRec(v2,v1,yPrev,edge,edges);	
			}
			
			yPrev = v1.y;
			v1 = v2;
		}
	}
	
	static void PolygonFill(int cnt, Point* p, SDL_Color color)
	{
		Edge *edges[480], *active;
		
		for(int i=0; i<480;i++)
		{
			edges[i] = new Edge;
			edges[i]->next = NULL;	
		}
		
		buildEdgeList(cnt, p, edges);
		
		active = new Edge;
		active->next = NULL;		
		
		for(int scan=0;scan<480;scan++)
		{
			buildActiveList(scan,active,edges);
			if(active->next!=NULL)
			{
				fillScan(scan,active,color);
				updateActiveList(scan,active);
				resortActiveList(active);	
			}	
		}
	}	
};

class Start: public Data
{
	public:
		
	static void Draw()
	{
	   int num=10;
	   
	   Point P[10] = {
	   					{220,340} ,
	   				    {220,220} , 
	   				    {250,170} , 
	   				    {270,200} ,
						{300,140} , 
						{320,240} , 
						{320,290} , 
						{420,220} ,
						{420,340} ,
						 {220,340}
					  };
	   					      
		
	   SDL_Color color ;
	   color.r = rand ( ) % 256 ;
	   color.g = rand ( ) % 256 ;
	   color.b = rand ( ) % 256 ;
	   
       PolygonDraw(num,P,color);		
       ScanFill::PolygonFill(num,P,color);
	}
	
	static void PolygonDraw(int n, Point points[],SDL_Color color)
    {
      if(n>=2)
	  {
	     for(int count=0;count<n-1;count++)
			lineDDA(&points[count],&points[count+1],color);	 	
	  }
    }
    
    static void lineDDA( struct Point * p1, struct Point *p2, SDL_Color color) 
	{

	  int dx,dy,steps,k;
	  float xinc,yinc,x,y;
		
	  dx=p2->x - p1->x;
	  dy=p2->y - p1->y;
		
	  if(abs(dx) > abs(dy))
		steps=abs(dx);
	  else
		steps=abs(dy);
	  
	  xinc=(float)dx/(float)steps;
	  yinc=(float)dy/(float)steps;
	  x=p1->x;
	  y=p1->y;
	  
	  SetPixel(pSurface,round(x),round(y),color);
	  
	  for(k=0;k<steps;k++) 
	  {
		x+=xinc;
		y+=yinc;
		SetPixel(pSurface,round(x),round(y),color);
	  }
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
