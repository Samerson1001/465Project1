/******************************************************************************
// Class: Software Engineering
******************************************************************************/
/******************************************************************************
// Lazy Foo web tutorials were used as a rough outline to help code the
// functions. Nothing was directly copied, only used as visual reference to
// help set up the initial program
******************************************************************************/
#include <SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 1024;

bool init();
bool media();
void close();

// Game Screen Window
SDL_Window* gWindow = NULL;
// Surface of the Window
SDL_Surface* gScreenSurface = NULL;
//Image loaded onto screen
SDL_Surface* gXOut = NULL;

bool init()
{
    // Flag
	bool success = true;

	//Initialize
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}

bool media()
{
    // Flag
    bool success = true;

    // Image
    gXOut = SDL_LoadBMP("465Project1/hello_world.bmp");
    if (gXOut == NULL)
    {
        success = false;
    }

    return success;
}

void close()
{
	//Deallocate surface
	SDL_FreeSurface(gXOut);
	gXOut = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL 
	SDL_Quit();
}

int main(int argc, char* args[])
{
    if(!init())
    {
        printf("Init failed\n");
    }
    else
    {
        if (!media())
        {
              printf("Media failed\n");
        }
        else
        {
            // Loop flag
            bool quit = false;

            // Event
            SDL_Event e;

            // Application x run
            while (!quit)
            {
                // Event queue
                while (SDL_PollEvent(&e) != 0)
                {
                    // User quit
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                }

                // Paint image onto screen
                SDL_BlitSurface(gXOut, NULL, gScreenSurface, NULL);

                // Update the game screen
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }

    // Free memory and exit out of SDL
    close();

    return 0;
}
