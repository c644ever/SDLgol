#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define QUITKEY 	SDLK_ESCAPE
#define WIDTH 		400
#define HIGHT 		400
#define BLOCKSIZE 	4
#define FELDH 		HIGHT/BLOCKSIZE
#define FELDW 		WIDTH/BLOCKSIZE

#define DEAD 		0
#define LIVE 		255

//global vars
int quit=0;

//    neigbors:        0      1     2     3     4     5     6     7     8
bool ruledeath[9] = { true  ,true ,false,false,true ,true ,true ,true ,true  } ;
bool rulebirth[9] = { false ,false,false,true ,false,false,false,false,false } ;

	
//SDL Declaration
int errorCount=0 ;
SDL_Window* screen = NULL;
SDL_Renderer* renderer;
SDL_Event event;
SDL_Rect rect, source, destination, dst;

//Declarations

unsigned char welt[FELDH][FELDW] = {0};
unsigned char buffer[FELDH][FELDW] = {0} ;

unsigned char getWelt (int x,int y)
{
	if (x<0) x=FELDW-1 ;
	if (y<0) y=FELDH-1 ;
	if (x>FELDW-1) x=0;
	if (y>FELDH-1) y=0;

	return (welt[x][y]);
}

void setWelt (int x, int y, unsigned char value)
{
	if (x<0) x=FELDW-1 ;
	if (y<0) y=FELDH-1 ;
	if (x>FELDW-1) x=0;
	if (y>FELDH-1) y=0;

	welt[x][y]=value ;
}

void initFeld () {
	int x,y ;
	
	for (x=0 ; x < FELDW ; x++ )
	{
		for ( y=0 ; y < FELDH ; y++ )
		{
			setWelt(x,y, (rand() % 2)*255 ) ;
		}
	}
}


int countNeighbors( int x, int y ) {
	
	int lebend=0;
	
	//check Nordost
	if ( getWelt(x-1,y-1) == LIVE ) { lebend++ ; }
	//check Nord
	if ( getWelt(x  ,y-1) == LIVE ) { lebend++ ; }
	//check Nordwest
	if ( getWelt(x+1,y-1) == LIVE ) { lebend++ ; }
	//check Ost
	if ( getWelt(x-1,y  ) == LIVE ) { lebend++ ; }
	//check West
	if ( getWelt(x+1,y  ) == LIVE ) { lebend++ ; }
	//check Südost
	if ( getWelt(x-1,y+1) == LIVE ) { lebend++ ; }
	//check Süd
	if ( getWelt(x  ,y+1) == LIVE ) { lebend++ ; }
	//check Südwest
	if ( getWelt(x+1,y+1) == LIVE ) { lebend++ ; }

	return (lebend);
}

//SDL Stuff
void LogError(char* msg) {
	errorCount++;
	printf( msg," %d",errorCount );
	printf("SDL Error: %s\n", SDL_GetError() );
}

void InitSetup() {
	srand((int)time(NULL));
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(WIDTH, HIGHT, SDL_WINDOW_SHOWN, &screen, &renderer);
	if (!screen) 
	{
		LogError("InitSetup failed to create window");
	}
	SDL_SetWindowTitle( screen,"Game of life" );
}

void ExitClean() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(screen);
	SDL_Quit();
}

void drawFeld( void ) {

	int x,y,r,g,b ;
	unsigned char s;
	
	for (x=0 ; x < FELDW ; x++ ) {
		for ( y=0 ; y < FELDH ; y++ ) {
			s=getWelt(x,y);
	
			switch (s) {
				case 0	: r=50 	; g=0   ; b=0   ; break ;
				case 255: r=189	; g=255 ; b=255	; break ;
			}
			rect.y=( x * BLOCKSIZE ) ; 
			rect.x=( y * BLOCKSIZE ) ; 
			rect.w= BLOCKSIZE-1 ;
			rect.h= BLOCKSIZE-1 ;
			SDL_SetRenderDrawColor(renderer,r,g,b,255);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
	SDL_RenderPresent(renderer);
}

int main( void ){
	int quit=0;
	int ticks=0 ;
	int x,y ; 
	int oldliving[10]={ 0 } , living=0; ;
	
	initFeld();

	//diffrent rule
	// ruledeath[5] = false ;
	// rulebirth[2] = true ;

	InitSetup();
	
	while(quit==0)
	{
		drawFeld();
		ticks++ ;

		for (x=0 ; x <FELDW ; x++) {
			for (y=0 ; y<FELDH ; y++) {
				int n=countNeighbors(x,y);

				if ( ruledeath[n] )
					buffer[x][y]=DEAD ;
				else if ( rulebirth[n] )
					buffer[x][y]=LIVE ;
				else
					buffer[x][y]=getWelt(x,y);
			}
		}

		for (x=0 ; x<FELDW ; x++) {
			for (y=0 ; y<FELDH ; y++) {
				setWelt(x,y,buffer[x][y]);
				if (buffer[x][y]==LIVE) living++ ;
			}
		}

		// Abbruchbedingung
		/*
		if (living==oldliving) quit=1 ;
		printf ("%d %d\n",oldliving,living );
		oldliving=living;
		living=0;
		*/
		
	}

	ExitClean();
	return (0);
}

int pressed ( void ) {
	int k=0 ;
	
	while (SDL_PollEvent(&event))
	{
		switch (event.type) {
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_LEFT:
						k=1 ;
					break;
					case SDLK_RIGHT:
						k=2 ;
					break;
					case SDLK_a:
						k=4 ;
					break;
					case SDLK_s:
						k=8 ;
					break;
					case SDLK_DOWN:
						k=16 ;
					break;
					default:
						k=0 ;
					break;
				}
			break;
				
			case SDL_KEYUP:  
				switch(event.key.keysym.sym){
					case SDLK_LEFT:
						k=0 ;
					break;
					case SDLK_RIGHT:
						k=0 ;
					break;
					case SDLK_a:
						k=0 ;
					break;
					case SDLK_s:
						k=0 ;
					break;
					case SDLK_DOWN:
						k=0 ;
					default:
					break;
				}
			break;
				
			case SDL_QUIT:
				quit = 1;
			break;
				
			default:
			break;	
		}
	}
	return (k) ;
}
