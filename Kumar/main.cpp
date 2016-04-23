#include "header.h"

const int WINDOW_WIDTH 		= 800;
const int WINDOW_HEIGHT 	= 600;
const char* WINDOW_TITLE 	= "SDL2 Kumario";

int main(/*int argc, char *argv[]*/)
{
   SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO );
   
   SDL_Window* window = SDL_CreateWindow( WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );

	SDL_Event event;
   bool gameRunning = true;

   while (gameRunning)
   {
     	if (SDL_PollEvent(&event))
     	{
        	if (event.type == SDL_QUIT) {
           	gameRunning = false;
        	}
     	} 
   }

	SDL_Quit();

	return 0;
}
