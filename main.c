/*
	My simple Interpretation of Conways Game of Life

	* playfield is wrap around
	* Rules are implemented
	* Avioding pointers
	* Using SDL to draw the playfield

	Have fun!
*/

// include needed stuff

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define QUITKEY 	SDLK_ESCAPE

// Constants
#define WIDTH 		800
#define HIGHT 		800
#define BLOCKSIZE 	8
#define FELDH 		HIGHT/BLOCKSIZE
#define FELDW 		WIDTH/BLOCKSIZE

#define DEAD 		0
#define LIVE 		255

//global vars
int quit=0;
	
//SDL Declaration
int errorCount=0 ;
SDL_Window* screen = NULL;
SDL_Renderer* renderer;
SDL_Event event;
SDL_Rect rect, source, destination, dst;

//Declarations for Life

unsigned char welt[FELDH][FELDW] = {0};
unsigned char buffer[FELDH][FELDW] = {0} ;

// The Conway Rules
//    neigbors:        0      1     2     3     4     5     6     7     8
bool ruledeath[9] = { true  ,true ,false,false,true ,true ,true ,true ,true  } ;
bool rulebirth[9] = { false ,false,false,true ,false,false,false,false,false } ;


// Reading out of the World
unsigned char getWelt (int x,int y )
{
	if ( x < 0 ) x = FELDW-1 ;
	if ( y < 0 ) y = FELDH-1 ;
	if ( x == FELDW ) x = 0 ;
	if ( y == FELDH ) y = 0 ;

	return (welt[x][y]);
}

// init with World Random
void initWorld (void) {
	int x,y ;
	
	for (x=0 ; x < FELDW ; x++ )
	{
		for ( y=0 ; y < FELDH ; y++ )
		{
			welt[x][y] = ( rand() % 2 ) * 255 ;
		}
	}
}

// Counting the Neigbors of a cell
int countNeighbors( int x, int y ) {
	
	int lebend=0;

	// Hier kann es zu array out of bounds kommen - daher getWet funktion für den Zugriff
	// Brute Force! possible optimation - because some cells are read out twice
	
	// check Nordost
	if ( getWelt(x-1,y-1) ) { lebend++ ; }
	// check Nord
	if ( getWelt(x  ,y-1) ) { lebend++ ; }
	// check Nordwest
	if ( getWelt(x+1,y-1) ) { lebend++ ; }
	// check Ost
	if ( getWelt(x-1,y  ) ) { lebend++ ; }
	// check West
	if ( getWelt(x+1,y  ) ) { lebend++ ; }
	// check Südost
	if ( getWelt(x-1,y+1) ) { lebend++ ; }
	// check Süd
	if ( getWelt(x  ,y+1) ) { lebend++ ; }
	// check Südwest
	if ( getWelt(x+1,y+1) ) { lebend++ ; }

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

void drawWorld( void ) {

	int x,y,r,g,b ;
	unsigned char s;
	
	for (x=0 ; x < FELDW ; x++ ) {
		for ( y=0 ; y < FELDH ; y++ ) {
			s=welt[x][y];
	
			switch (s) {
				case 0	: r=50 	; g=0   ; b=0   ; break ;
				case 255: r=189	; g=255 ; b=255	; break ;
			}
			
			rect.y= x * BLOCKSIZE ; 
			rect.x= y * BLOCKSIZE ; 
			rect.w= BLOCKSIZE-1 ;
			rect.h= BLOCKSIZE-1 ;
			SDL_SetRenderDrawColor(renderer,r,g,b,255);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
	SDL_RenderPresent(renderer);
}

void pressed( void ) 
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type) 
		{				
			case SDL_QUIT:
				quit = 1;
			break;
				
			default:
			break;	
		}
	}
}


/*
	init SDL

	Fill World with random numbers

	Count Neighbors
	Decide if dead or alive
	set new World
	
*/
int main( void )
{
	int ticks=0 ;
	int x,y ; 
	int living=0 ;	
	
	initWorld();

	// put different rules here
	/*
	ruledeath[5] = false ;
	rulebirth[2] = true ;
	*/
	
	InitSetup();
	
	while(quit==0)
	{
		drawWorld();
		ticks++ ;

		for (x=0 ; x <FELDW ; x++) {
			for (y=0 ; y<FELDH ; y++) {
				int n=countNeighbors(x,y);

				if ( ruledeath[n] )
					buffer[x][y]=DEAD ;
				else if ( rulebirth[n] )
					buffer[x][y]=LIVE ;
				else
					buffer[x][y]=welt[x][y];
			}
		}

		for (x=0 ; x<FELDW ; x++) {
			for (y=0 ; y<FELDH ; y++) {
				unsigned char tmp ;
				 
				tmp = buffer[x][y];
				buffer[x][y]=0;

				welt[x][y]=tmp ;
				if ( tmp==LIVE) living++ ;
				
			}
		}

		pressed();

		if (ticks>10000) quit=1;
	}

	ExitClean();
	return (0);
}
