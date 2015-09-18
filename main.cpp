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

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load stretching surface
    //gStretchedSurface = loadSurface( "images/tiger.bmp" );
    sheep = SDL_LoadBMP("images/sheep.bmp");

    if( sheep == NULL )
    {
        printf( "Failed to load stretching image!\n" );
        success = false;
    }

    return success;
}

void close()
{
    //Free loaded image
    SDL_FreeSurface( gStretchedSurface );
    gStretchedSurface = NULL;

    SDL_FreeSurface (sheep);
    sheep = NULL;

    SDL_FreeSurface (gScreenSurface);
    gScreenSurface = NULL;


    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

SDL_Surface* loadSurface( std::string path )
{
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
        //optimizedSurface = SDL_ConvertSurface( loadedSurface, sheep->format, NULL );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    return optimizedSurface;
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
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {   
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;
            SDL_Rect stretchRect;
            stretchRect.x = 300;
            stretchRect.y = 200;
            stretchRect.w = 50;
            stretchRect.h = 50;

            //While application is running
            while( !quit )
            {
                //Handle events on queue
                if( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }

                    //else if(e.type == SDL_KEYDOWN)
                    //{ 
                    if (e.key.keysym.sym == SDLK_w)
                    {
                        stretchRect.y -= 5;
                    }
                    
                    if (e.key.keysym.sym == SDLK_s)
                    {
                        stretchRect.y += 5;
                    }
                    
                    if (e.key.keysym.sym == SDLK_a)
                    {
                        stretchRect.x -= 5;
                    }
                    
                    if (e.key.keysym.sym == SDLK_d)
                    {
                        stretchRect.x += 5;
                    }
                
                
                }
                
                terr_generation();
                terr_print();
                SDL_BlitScaled( sheep, NULL, gScreenSurface, &stretchRect );
                
                SDL_UpdateWindowSurface(gWindow);
                SDL_Delay(20);
                
            }
        }
        
    }
//Free resources and close SDL
	close();
    
	return 0;
}
