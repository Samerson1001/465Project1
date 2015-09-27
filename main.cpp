/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, standard IO, and strings
#include <SDL.h>
#include <stdio.h>
#include <string>
#include "constants.h"
#include <SDL/SDL_image.h>
#include "asteroid.h"

Asteroid asteroid[100];
SDL_Rect zone1;
SDL_Rect zone2;
SDL_Rect zone3;
SDL_Rect zone4;
SDL_Rect zone5;
SDL_Rect zone6;
SDL_Rect zone7;
SDL_Rect zone8;


void terr_generation()
{
    for (int i = 0; i < 100; ++i)
    {
        if (rand() % 250 == 0)
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
    //gStretchedSurface = loadSurface( "images/sheep.bmp" );
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
        bool flag = true;
        if (asteroid[i].screen)
        {
            for (int l = 0; l < 100; ++l)
            {
                if (asteroid[l].screen)
                {
                    if (asteroid[i].overlap_check(asteroid[l]))
                    {
                        flag = false;
                        asteroid[i].screen = false;
                        asteroid[i].ast.x = 680;
                        asteroid[i].ast.y = rand() % 480;
                    }
                }
            }
            if (flag == true)
            {
                asteroid[i].print();
            }
            
        }
    }
}

void safe_zone(const int & safe)
{
    
}

void zone()
{
     zone1.x = 600;
     zone1.y = 0;
     zone1.w = 60;
     zone1.h = 60;
     zone2.x = 600;
     zone2.y = 60;
     zone2.w = 60;
     zone2.h = 60;
     zone3.x = 600;
     zone3.y = 120;
     zone3.w = 60;
     zone3.h = 60;
     zone4.x = 600;
     zone4.y = 180;
     zone4.w = 60;
     zone4.h = 60;
     zone5.x = 600;
     zone5.y = 240;
     zone5.w = 60;
     zone5.h = 60;
     zone6.x = 600;
     zone6.y = 300;
     zone6.w = 60;
     zone6.h = 60;
     zone7.x = 600;
     zone7.y = 360;
     zone7.w = 60;
     zone7.h = 60;
     zone8.x = 600;
     zone8.y = 420;
     zone8.w = 60;
     zone8.h = 60;
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
            SDL_Surface *back;
            back = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);
            SDL_Rect background;
            background.x = 0;
            background.y = 0;
            background.w = 640;
            background.h = 480;
            SDL_FillRect(back, NULL, SDL_MapRGB(back->format, 0, 0, 0));

            stretchRect.x = 300;
            stretchRect.y = 200;
            stretchRect.w = 50;
            stretchRect.h = 50;

            zone();
            
            int safe = rand() % 8 + 1;
            int sheepSpeed = 3;

            //While application is running
            while( !quit )
            {
                const Uint8 *state = SDL_GetKeyboardState(NULL);

                if (state[SDL_SCANCODE_W])
                {
                    if (stretchRect.y <= 0);

                    else
                        stretchRect.y -= sheepSpeed;
                }

                if (state[SDL_SCANCODE_S])
                {
                    if (stretchRect.y >= 430);

                    else
                        stretchRect.y += sheepSpeed;
                }
                    
                if (state[SDL_SCANCODE_A])
                {
                    if (stretchRect.x <= 0);

                    else
                        stretchRect.x -= sheepSpeed;
                }
                    
                if (state[SDL_SCANCODE_D])
                {
                    if (stretchRect.x >= 590);

                    else
                        stretchRect.x += sheepSpeed;
                }

                //Handle events on queue
                if( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }

                    //int SDL_EnablelKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);


                    //else if(e.type == SDL_KEYDOWN)
                    //{ 
                    
                    

                
                
                }
                
                SDL_BlitScaled(back, NULL, gScreenSurface, &background);
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
