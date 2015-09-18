/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, standard IO, and strings
#include <SDL.h>
#include <stdio.h>
#include <string>
#include "constants.h"
#include <SDL_image.h>
#include "asteroid.h"

Asteroid asteroid[100];

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}

	return success;
}

void close()
{
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void print(Asteroid & Asteroid)
{
    SDL_BlitScaled(Asteroid.asteroid, NULL,
                   gScreenSurface, &Asteroid.ast);
                
}

void terr_generation()
{
    for (int i = 0; i < 100; ++i)
    {
        if (rand() % 500 == 0)
        {
            asteroid[i].screen = true;
        }
    }
}

void terr_print()
{
    for (int i = 0; i < 100; ++i)
    {
        if (asteroid[i].screen)
        {
            asteroid[i].left();
            SDL_BlitSurface(asteroid[i].asteroid, NULL,
                            gScreenSurface, &asteroid[i].ast);
        }
        
    }
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{	
        //Main loop flag
        bool quit = false;
        //Event handler
        SDL_Event e;
        int counter = 0;
        //While application is running
        while( !quit )
        {
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
            }
            counter++;
            terr_generation();
            terr_print();
            SDL_UpdateWindowSurface(gWindow);
            std::cout << counter << std::endl;
            SDL_Delay(20);
        }
    }
    
    
    
	//Free resources and close SDL
	close();
    
	return 0;
}
