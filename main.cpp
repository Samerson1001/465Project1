/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, standard IO, and strings
#include <SDL.h>
#include <stdio.h>
#include <string>
#include "constants.h"
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "asteroid.h"
#include "LTimer.h"

Asteroid asteroid[100];
Asteroid fill[100];
SDL_Rect zone1;
SDL_Rect zone2;
SDL_Rect zone3;
SDL_Rect zone4;
SDL_Rect zone5;
SDL_Rect zone6;
SDL_Rect zone7;
SDL_Rect zone8;

int safe = rand() % 8 + 1;
LTimer timer;


// General layout of menu was referenced from Lazy Foo Tutorials
int menu(SDL_Surface* screen, TTF_Font* font)
{
	Uint32 time;
	int x, y;
	
	// Number of Menu Items to choose from
	const int MENU_ITEMS=2;
	// Array of menu titles
	const char* menu_titles[MENU_ITEMS] = {"Start Game","Exit"};
	SDL_Surface* menus[MENU_ITEMS];
	bool selected[MENU_ITEMS] = {0,0};

	// Non-Selected Color = White; Selected Color = Red
	SDL_Color color[2] = {{255,255,255},{255,0,0}};

	// Set menu order
	menus[0]=TTF_RenderText_Solid(font, menu_titles[0], color[0]);
	menus[1]=TTF_RenderText_Solid(font, menu_titles[1], color[0]);

	// Attempt to center the menu items...?
	SDL_Rect pos[MENU_ITEMS];
	pos[0].x=screen->clip_rect.w/2 - menus[0]->clip_rect.w/2;
	pos[0].y=screen->clip_rect.h/2 - menus[0]->clip_rect.h;
	pos[1].x=screen->clip_rect.w/2 - menus[0]->clip_rect.w/2;
	pos[1].y=screen->clip_rect.h/2 + menus[0]->clip_rect.h;
	
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format,0x00,0x00,0x00));

	SDL_Event event;
	while(1)
	{
		time=SDL_GetTicks();
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				// If user quits the game, free the menu surfaces
				case SDL_QUIT:
					for(int i = 0; i < MENU_ITEMS; i++)
					{
						SDL_FreeSurface(menus[i]);
					}
					return 1;
				
				// Track position of the mouse and change colors of the menu items if they are hovered over
				case SDL_MOUSEMOTION:
					x=event.motion.x;
					y=event.motion.y;
					for(int i = 0; i < MENU_ITEMS; i++)
					{
						if(x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h)
						{
							if(!selected[i])
							{
								selected[i] = 1;
								SDL_FreeSurface(menus[i]);
								menus[i] = TTF_RenderText_Solid(font, menu_titles[i], color[1]);
							}
						}
						else
						{
							if(selected[i])
							{
								selected[i] = 0;
								SDL_FreeSurface(menus[i]);
								menus[i] = TTF_RenderText_Solid(font, menu_titles[i], color[0]);
							}
						}
					}
					break;

				// Free surfaces once the menu items have been clicked
				case SDL_MOUSEBUTTONDOWN:
					x=event.button.x;
					y=event.button.y;
					for(int i = 0; i < MENU_ITEMS; i++)
					{
						if(x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y<=pos[i].y + pos[i].h)
						{
							for(int j = 0; j < MENU_ITEMS; j++)
							{
								SDL_FreeSurface(menus[j]);
							}
							return i;
						}
					}
					break;

				// If escape key is pressed, free surfaces
				case SDL_KEYDOWN:
					if(event.key.keysym.sym==SDLK_ESCAPE)
					{
						for(int i = 0; i < MENU_ITEMS; i++)
						{
							SDL_FreeSurface(menus[i]);
						}

						return 0;
					}
			}
		}

		// Blit the menu to the screen
		for (int i = 0;i < MENU_ITEMS; i++)
			SDL_BlitSurface(menus[i], NULL, gScreenSurface, &pos[i]);

		SDL_UpdateWindowSurface(gWindow);

		if(1000 / 30 > (SDL_GetTicks()-time))
			SDL_Delay(1000 / 30 -(SDL_GetTicks()-time));
	}
}

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
        if (asteroid[i].screen)
        {
            for (int l = 0; l < 100; ++l)
            {
                if (asteroid[l].screen)
                {
                    if (asteroid[i].overlap_check(asteroid[l]))
                    {
                        asteroid[i].screen = false;
                        asteroid[i].ast.x = 680;
                        asteroid[i].ast.y = rand() % 480;
                    } 
                }
            }
        }
    }

    // THIS IS WHERE THE OVERLAP CHECK HAPPENS FOR THE FILL IN FUNCTION
    // PRETTY MUCH NEED TO FIGURE OUT HOW TO MODIFY THIS SO INSTEAD OF
    // JUST TAKING IT OFF SCREEN(WHICH SCRAPS IT'S X/Y VALUES IN THE
    // FILL IN FUNCTION. NEED TO FIGURE OUT WAY TO COMBINE BOTH SO
    // IT GENERATES A CLEAN ASTEROID NO OVERLAP THEN JUST PRINTS
    // **********************************************************
    // THIS IS WHERE I NEED THE HELP WITH FILL IN FUNCTION
    //***********************************************************
      if (zone1_check() == false)
      {
          for (int l = 0; l < 100; ++l)
          {
              if (fill[l].screen == false)
              {
                  fill[l].screen = true;
                  fill[l].ast.x = 640;
                  fill[l].ast.y = rand() % 60;
                  break;
              }
              
          }
      }
    for (int i = 0; i < 100; ++i)
    {
        if (fill[i].screen)
        {
            for (int l = 0; l < 100; ++l)
            {
                if (asteroid[l].screen)
                {
                    if (fill[i].overlap_check(asteroid[l]))
                    {
                        fill[i].screen = false;
                    }
                }
                if (fill[l].screen)
                {
                    if (fill[i].overlap_check(fill[l]))
                    {
                        fill[i].screen = false;
                    }
                }
            }
        }
    }

    
    for (int i = 0; i < 100; ++i)
    {
       
        if (asteroid[i].screen)
        {
            asteroid[i].print();
        }
        if (fill[i].screen)
        {
            fill[i].print();
        }
    }
}



void safe_zone(int & safe)
{
    // Goal of this function takes in the safe zone, then scans through both
    // arrays and does hit detection, if true, it removes the asteroid
    // off the screen.
    if (safe == 1)
    {
        int left1 = zone1.x;
        int right1 = zone1.x + zone1.w;
        int top1 = zone1.y;
        int bottom1 = zone1.y + zone2.h;
       
        // Find edges of rect2
        for (int i = 0; i < 100; ++i)
        {
            bool flag = true;
            
            int left2 = asteroid[i].ast.x;
            int right2 = asteroid[i].ast.x + asteroid[i].ast.w;
            int top2 = asteroid[i].ast.y;
            int bottom2 = asteroid[i].ast.y + asteroid[i].ast.h;
            if (left1 == left2 && right1 == right2 &&
                top1 == top2 && bottom1 == bottom2)
            {
                flag = false;
            }
            // Check edges
            if ( left1 > right2 )// Left 1 is right of right 2
                flag = false; // No collision
            
            if ( right1 < left2 ) // Right 1 is left of left 2
                flag = false; // No collision
            
            if ( top1 > bottom2 ) // Top 1 is below bottom 2
                flag = false; // No collision
            
            if ( bottom1 < top2 ) // Bottom 1 is above top 2 
                flag = false; // No collision

            if (flag == true)
            {
                asteroid[i].screen = false;
                asteroid[i].ast.x = 680;
                asteroid[i].ast.y = rand() % 480;
            }
        }
        for (int i = 0; i < 100; ++i)
        {
            bool flag = true;
            if (fill[i].screen)
            {
                int left2 = fill[i].ast.x;
                int right2 = fill[i].ast.x + fill[i].ast.w;
                int top2 = fill[i].ast.y;
                int bottom2 = fill[i].ast.y + fill[i].ast.h;
                
                // Check edges
                if ( left1 > right2 )// Left 1 is right of right 2
                    flag = false; // No collision
                
                if ( right1 < left2 ) // Right 1 is left of left 2
                    flag = false; // No collision
                
                if ( top1 > bottom2 ) // Top 1 is below bottom 2
                    flag = false; // No collision
                
                if ( bottom1 < top2 ) // Bottom 1 is above top 2 
                    flag = false; // No collision

                if (flag == true)
                {
                    fill[i].screen = false;
                }
            }
        }
    
    }
   
    if (safe == 2)
    {
        int left1 = zone2.x;
        int right1 = zone2.x + zone2.w;
        int top1 = zone2.y;
        int bottom1 = zone2.y + zone2.h;
       
        // Find edges of rect2
        for (int i = 0; i < 100; ++i)
        {
            bool flag = true;
            
            int left2 = asteroid[i].ast.x;
            int right2 = asteroid[i].ast.x + asteroid[i].ast.w;
            int top2 = asteroid[i].ast.y;
            int bottom2 = asteroid[i].ast.y + asteroid[i].ast.h;
            if (left1 == left2 && right1 == right2 &&
                top1 == top2 && bottom1 == bottom2)
            {
                flag = false;
            }
            // Check edges
            if ( left1 > right2 )// Left 1 is right of right 2
                flag = false; // No collision
            
            if ( right1 < left2 ) // Right 1 is left of left 2
                flag = false; // No collision
            
            if ( top1 > bottom2 ) // Top 1 is below bottom 2
                flag = false; // No collision
            
            if ( bottom1 < top2 ) // Bottom 1 is above top 2 
                flag = false; // No collision

            if (flag == true)
            {
                asteroid[i].screen = false;
                asteroid[i].ast.x = 680;
                asteroid[i].ast.y = rand() % 480;
            }
        }
        for (int i = 0; i < 100; ++i)
        {
            bool flag = true;
            if (fill[i].screen)
            {
                int left2 = fill[i].ast.x;
                int right2 = fill[i].ast.x + fill[i].ast.w;
                int top2 = fill[i].ast.y;
                int bottom2 = fill[i].ast.y + fill[i].ast.h;
                
                // Check edges
                if ( left1 > right2 )// Left 1 is right of right 2
                    flag = false; // No collision
                
                if ( right1 < left2 ) // Right 1 is left of left 2
                    flag = false; // No collision
                
                if ( top1 > bottom2 ) // Top 1 is below bottom 2
                    flag = false; // No collision
                
                if ( bottom1 < top2 ) // Bottom 1 is above top 2 
                    flag = false; // No collision
                
                if (flag == true)
                {
                    fill[i].screen = false;
                }
            }
        }
    
    }
    
    if (safe == 3)
    {
        int left1 = zone3.x;
        int right1 = zone3.x + zone3.w;
        int top1 = zone3.y;
        int bottom1 = zone3.y + zone3.h;
       
        // Find edges of rect2
        for (int i = 0; i < 100; ++i)
        {
            bool flag = true;
            
            int left2 = asteroid[i].ast.x;
            int right2 = asteroid[i].ast.x + asteroid[i].ast.w;
            int top2 = asteroid[i].ast.y;
            int bottom2 = asteroid[i].ast.y + asteroid[i].ast.h;
            if (left1 == left2 && right1 == right2 &&
                top1 == top2 && bottom1 == bottom2)
            {
                flag = false;
            }
            // Check edges
            if ( left1 > right2 )// Left 1 is right of right 2
                flag = false; // No collision
            
            if ( right1 < left2 ) // Right 1 is left of left 2
                flag = false; // No collision
            
            if ( top1 > bottom2 ) // Top 1 is below bottom 2
                flag = false; // No collision
            
            if ( bottom1 < top2 ) // Bottom 1 is above top 2 
                flag = false; // No collision

            if (flag == true)
            {
                asteroid[i].screen = false;
                asteroid[i].ast.x = 680;
                asteroid[i].ast.y = rand() % 480;
            }
        }
        for (int i = 0; i < 100; ++i)
        {
            bool flag = true;
            if (fill[i].screen)
            {
                int left2 = fill[i].ast.x;
                int right2 = fill[i].ast.x + fill[i].ast.w;
                int top2 = fill[i].ast.y;
                int bottom2 = fill[i].ast.y + fill[i].ast.h;
                
                // Check edges
                if ( left1 > right2 )// Left 1 is right of right 2
                    flag = false; // No collision
                
                if ( right1 < left2 ) // Right 1 is left of left 2
                    flag = false; // No collision
                
                if ( top1 > bottom2 ) // Top 1 is below bottom 2
                    flag = false; // No collision
                
                if ( bottom1 < top2 ) // Bottom 1 is above top 2 
                    flag = false; // No collision

                if (flag == true)
                {
                    fill[i].screen = false;
                }
            }
        }
    
    }
   
    if (safe == 4)
    {
        int left1 = zone4.x;
        int right1 = zone4.x + zone4.w;
        int top1 = zone4.y;
        int bottom1 = zone4.y + zone4.h;
       
        // Find edges of rect2
        for (int i = 0; i < 100; ++i)
        {
            bool flag = true;
            
            int left2 = asteroid[i].ast.x;
            int right2 = asteroid[i].ast.x + asteroid[i].ast.w;
            int top2 = asteroid[i].ast.y;
            int bottom2 = asteroid[i].ast.y + asteroid[i].ast.h;
            if (left1 == left2 && right1 == right2 &&
                top1 == top2 && bottom1 == bottom2)
            {
                flag = false;
            }
            // Check edges
            if ( left1 > right2 )// Left 1 is right of right 2
                flag = false; // No collision
            
            if ( right1 < left2 ) // Right 1 is left of left 2
                flag = false; // No collision
            
            if ( top1 > bottom2 ) // Top 1 is below bottom 2
                flag = false; // No collision
            
            if ( bottom1 < top2 ) // Bottom 1 is above top 2 
                flag = false; // No collision

            if (flag == true)
            {
                asteroid[i].screen = false;
                asteroid[i].ast.x = 680;
                asteroid[i].ast.y = rand() % 480;
            }
        }
        for (int i = 0; i < 100; ++i)
        {
            bool flag = true;
            if (fill[i].screen)
            {
                int left2 = fill[i].ast.x;
                int right2 = fill[i].ast.x + fill[i].ast.w;
                int top2 = fill[i].ast.y;
                int bottom2 = fill[i].ast.y + fill[i].ast.h;
                
                // Check edges
                if ( left1 > right2 )// Left 1 is right of right 2
                    flag = false; // No collision
                
                if ( right1 < left2 ) // Right 1 is left of left 2
                    flag = false; // No collision
                
                if ( top1 > bottom2 ) // Top 1 is below bottom 2
                    flag = false; // No collision
                
                if ( bottom1 < top2 ) // Bottom 1 is above top 2 
                    flag = false; // No collision

                if (flag == true)
                {
                    fill[i].screen = false;
                }
            }
        }
    
    }
   
    if (safe == 5)
    {
        int left1 = zone5.x;
        int right1 = zone5.x + zone5.w;
        int top1 = zone5.y;
        int bottom1 = zone5.y + zone5.h;
       
        // Find edges of rect2
        for (int i = 0; i < 100; ++i)
        {
            bool flag = true;
            
            int left2 = asteroid[i].ast.x;
            int right2 = asteroid[i].ast.x + asteroid[i].ast.w;
            int top2 = asteroid[i].ast.y;
            int bottom2 = asteroid[i].ast.y + asteroid[i].ast.h;
            if (left1 == left2 && right1 == right2 &&
                top1 == top2 && bottom1 == bottom2)
            {
                flag = false;
            }
            // Check edges
            if ( left1 > right2 )// Left 1 is right of right 2
                flag = false; // No collision
            
            if ( right1 < left2 ) // Right 1 is left of left 2
                flag = false; // No collision
            
            if ( top1 > bottom2 ) // Top 1 is below bottom 2
                flag = false; // No collision
            
            if ( bottom1 < top2 ) // Bottom 1 is above top 2 
                flag = false; // No collision

            if (flag == true)
            {
                asteroid[i].screen = false;
                asteroid[i].ast.x = 680;
                asteroid[i].ast.y = rand() % 480;
            }
        }
        for (int i = 0; i < 100; ++i)
        {
            bool flag = true;
            if (fill[i].screen)
            {
                int left2 = fill[i].ast.x;
                int right2 = fill[i].ast.x + fill[i].ast.w;
                int top2 = fill[i].ast.y;
                int bottom2 = fill[i].ast.y + fill[i].ast.h;
                
                // Check edges
                if ( left1 > right2 )// Left 1 is right of right 2
                    flag = false; // No collision
                
                if ( right1 < left2 ) // Right 1 is left of left 2
                    flag = false; // No collision
                
                if ( top1 > bottom2 ) // Top 1 is below bottom 2
                    flag = false; // No collision
                
                if ( bottom1 < top2 ) // Bottom 1 is above top 2 
                    flag = false; // No collision

                if (flag == true)
                {
                    fill[i].screen = false;
                }
            }
        }
    
    }
   
    if (safe == 6)
    {
        int left1 = zone6.x;
        int right1 = zone6.x + zone6.w;
        int top1 = zone6.y;
        int bottom1 = zone6.y + zone6.h;
       
        // Find edges of rect2
        for (int i = 0; i < 100; ++i)
        {
            bool flag = true;
            
            int left2 = asteroid[i].ast.x;
            int right2 = asteroid[i].ast.x + asteroid[i].ast.w;
            int top2 = asteroid[i].ast.y;
            int bottom2 = asteroid[i].ast.y + asteroid[i].ast.h;
            if (left1 == left2 && right1 == right2 &&
                top1 == top2 && bottom1 == bottom2)
            {
                flag = false;
            }
            // Check edges
            if ( left1 > right2 )// Left 1 is right of right 2
                flag = false; // No collision
            
            if ( right1 < left2 ) // Right 1 is left of left 2
                flag = false; // No collision
            
            if ( top1 > bottom2 ) // Top 1 is below bottom 2
                flag = false; // No collision
            
            if ( bottom1 < top2 ) // Bottom 1 is above top 2 
                flag = false; // No collision

            if (flag == true)
            {
                asteroid[i].screen = false;
                asteroid[i].ast.x = 680;
                asteroid[i].ast.y = rand() % 480;
            }
        }
        for (int i = 0; i < 100; ++i)
        {
            bool flag = true;
            if (fill[i].screen)
            {
                int left2 = fill[i].ast.x;
                int right2 = fill[i].ast.x + fill[i].ast.w;
                int top2 = fill[i].ast.y;
                int bottom2 = fill[i].ast.y + fill[i].ast.h;
                
                // Check edges
                if ( left1 > right2 )// Left 1 is right of right 2
                    flag = false; // No collision
                
                if ( right1 < left2 ) // Right 1 is left of left 2
                    flag = false; // No collision
                
                if ( top1 > bottom2 ) // Top 1 is below bottom 2
                    flag = false; // No collision
                
                if ( bottom1 < top2 ) // Bottom 1 is above top 2 
                    flag = false; // No collision

                if (flag == true)
                {
                    fill[i].screen = false;
                }
            }
        }
    
    }
    
    if (safe == 7)
    {
        int left1 = zone7.x;
        int right1 = zone7.x + zone7.w;
        int top1 = zone7.y;
        int bottom1 = zone7.y + zone7.h;
       
        // Find edges of rect2
        for (int i = 0; i < 100; ++i)
        {
            bool flag = true;
            
            int left2 = asteroid[i].ast.x;
            int right2 = asteroid[i].ast.x + asteroid[i].ast.w;
            int top2 = asteroid[i].ast.y;
            int bottom2 = asteroid[i].ast.y + asteroid[i].ast.h;
            if (left1 == left2 && right1 == right2 &&
                top1 == top2 && bottom1 == bottom2)
            {
                flag = false;
            }
            // Check edges
            if ( left1 > right2 )// Left 1 is right of right 2
                flag = false; // No collision
            
            if ( right1 < left2 ) // Right 1 is left of left 2
                flag = false; // No collision
            
            if ( top1 > bottom2 ) // Top 1 is below bottom 2
                flag = false; // No collision
            
            if ( bottom1 < top2 ) // Bottom 1 is above top 2 
                flag = false; // No collision

            if (flag == true)
            {
                asteroid[i].screen = false;
                asteroid[i].ast.x = 680;
                asteroid[i].ast.y = rand() % 480;
            }
        }
        for (int i = 0; i < 100; ++i)
        {
            bool flag = true;
            if (fill[i].screen)
            {
                int left2 = fill[i].ast.x;
                int right2 = fill[i].ast.x + fill[i].ast.w;
                int top2 = fill[i].ast.y;
                int bottom2 = fill[i].ast.y + fill[i].ast.h;
                
                // Check edges
                if ( left1 > right2 )// Left 1 is right of right 2
                    flag = false; // No collision
                
                if ( right1 < left2 ) // Right 1 is left of left 2
                    flag = false; // No collision
                
                if ( top1 > bottom2 ) // Top 1 is below bottom 2
                    flag = false; // No collision
                
                if ( bottom1 < top2 ) // Bottom 1 is above top 2 
                    flag = false; // No collision

                if (flag == true)
                {
                    fill[i].screen = false;
                }
            }
        }
    
    }
    
    if (safe == 8)
    {
        int left1 = zone8.x;
        int right1 = zone8.x + zone8.w;
        int top1 = zone8.y;
        int bottom1 = zone8.y + zone8.h;
       
        // Find edges of rect2
        for (int i = 0; i < 100; ++i)
        {
            bool flag = true;
            
            int left2 = asteroid[i].ast.x;
            int right2 = asteroid[i].ast.x + asteroid[i].ast.w;
            int top2 = asteroid[i].ast.y;
            int bottom2 = asteroid[i].ast.y + asteroid[i].ast.h;
            if (left1 == left2 && right1 == right2 &&
                top1 == top2 && bottom1 == bottom2)
            {
                flag = false;
            }
            // Check edges
            if ( left1 > right2 )// Left 1 is right of right 2
                flag = false; // No collision
            
            if ( right1 < left2 ) // Right 1 is left of left 2
                flag = false; // No collision
            
            if ( top1 > bottom2 ) // Top 1 is below bottom 2
                flag = false; // No collision
            
            if ( bottom1 < top2 ) // Bottom 1 is above top 2 
                flag = false; // No collision

            if (flag == true)
            {
                asteroid[i].screen = false;
                asteroid[i].ast.x = 680;
                asteroid[i].ast.y = rand() % 480;
            }
        }
        for (int i = 0; i < 100; ++i)
        {
            bool flag = true;
            if (fill[i].screen)
            {
                int left2 = fill[i].ast.x;
                int right2 = fill[i].ast.x + fill[i].ast.w;
                int top2 = fill[i].ast.y;
                int bottom2 = fill[i].ast.y + fill[i].ast.h;
                
                // Check edges
                if ( left1 > right2 )// Left 1 is right of right 2
                    flag = false; // No collision
                
                if ( right1 < left2 ) // Right 1 is left of left 2
                    flag = false; // No collision
                
                if ( top1 > bottom2 ) // Top 1 is below bottom 2
                    flag = false; // No collision
                
                if ( bottom1 < top2 ) // Bottom 1 is above top 2 
                    flag = false; // No collision
                
                if (flag == true)
                {
                    fill[i].screen = false;
                }
            }
        }
    
    }
    
}

bool zone1_check()
{
    int left2 = zone1.x;
    int right2 = zone1.x + zone1.w;
    int top2 = zone1.y;
    int bottom2 = zone1.y + zone2.h;
    bool check = false;
    
    // Find edges of rect2
    for (int i = 0; i < 100; ++i)
    {
        bool flag = true;
        
        int left1 = asteroid[i].ast.x;
        int right1 = asteroid[i].ast.x + asteroid[i].ast.w;
        int top1 = asteroid[i].ast.y;
        int bottom1 = asteroid[i].ast.y + asteroid[i].ast.h;
        // Check edges
        if ( left1 > right2 )// Left 1 is right of right 2
            flag = false; // No collision
        
        if ( right1 < left2 ) // Right 1 is left of left 2
            flag = false; // No collision
        
        if ( top1 > bottom2 ) // Top 1 is below bottom 2
            flag = false; // No collision
        
        if ( bottom1 < top2 ) // Bottom 1 is above top 2 
            flag = false; // No collision
        if (flag == true)
        {
            check = true;
            return check;
        }
    }
    return check;
}

// MIGHT NEED TO REWORK LOGIC IN HERE IN ORDER FOR PRINTING OF FILL IN
// TO BE MORE EFFECTIVE INSTEAD OF PRINT FUNCTION

void fill_in(const int & safe)
{
    // Function looks at safe zone, if it isn't the safe zone, it should
    // put an asteroid inside the frame of the zone, so not zone 1 is any
    // y value between 0 to 60. 
    if (safe != 1)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (fill[i].screen == false)
            {
                fill[i].screen = true;
                fill[i].ast.x = 680;
                fill[i].ast.y = rand() % 60;
                
                break;
            }
        }
    }
    if (safe != 2)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (fill[i].screen == false)
            {
                fill[i].screen = true;
                fill[i].ast.x = 680;
                fill[i].ast.y = rand() % 60 + 60;
                
                break;
            }
        }
    }
    if (safe != 3)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (fill[i].screen == false)
            {
                fill[i].screen = true;
                fill[i].ast.x = 680;
                fill[i].ast.y = rand() % 60 + 120;
                
                break;
            }
        }
    }
    if (safe != 4)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (fill[i].screen == false)
            {
                fill[i].screen = true;
                fill[i].ast.x = 680;
                fill[i].ast.y = rand() % 60 + 180;
    
                break;
            }
        }
    }
    if (safe != 5)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (fill[i].screen == false)
            {
                fill[i].screen = true;
                fill[i].ast.x = 680;
                fill[i].ast.y = rand() % 60 + 240;
                
                break;
            }
        }
    }
    if (safe != 6)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (fill[i].screen == false)
            {
                fill[i].screen = true;
                fill[i].ast.x = 680;
                fill[i].ast.y = rand() % 60 + 300;
                
                break;
            }
        }   
    }
    if(safe != 7)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (fill[i].screen == false)
            {
                fill[i].screen = true;
                fill[i].ast.x = 680;
                fill[i].ast.y = rand() % 60 + 360;
                
                break;
            }
        }
    }
    if (safe != 8)
    {
        for (int i = 0; i < 100; ++i)
        {
            if (fill[i].screen == false)
            {
                fill[i].screen = true;
                fill[i].ast.x = 680;
                fill[i].ast.y = rand() % 60 + 420;
                
                break;
            }
        }
    }
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
            stretchRect.w = 25;
            stretchRect.h = 25;
            zone();

            TTF_Font *font;
            TTF_Init();
            font = TTF_OpenFont("includes/game_over.ttf",30);
            int i = menu(gScreenSurface,font);
            if(i == 1)
               quit == true;
            
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
                }
                if( !timer.isStarted() )
                {
                    timer.start();
                }
                SDL_BlitScaled(back, NULL, gScreenSurface, &background);
                terr_generation();
                fill_in(safe);
                terr_print();
                safe_zone(safe);
                std::cout << zone1_check() << std::endl;
                std::cout << safe << std::endl;
                
                if (timer.getTicks() % 750 > 735)
                {
                    int random = rand() % 10;
                  
                    switch (random % 2)
                    {
                        case 0 :
                            if (safe == 1)
                            {
                                safe += 1;
                            }
                            else
                            {
                                safe -= 1;
                            }
                            break;
                        case 1:
                            if (safe == 8)
                            {
                                safe -= 1;
                            }
                            else 
                            {
                                safe += 1;
                            }
                            break;
                    }
                }
               
                SDL_BlitScaled(sheep, NULL, gScreenSurface, &stretchRect);
                
                SDL_UpdateWindowSurface(gWindow);
                SDL_Delay(20);                
            }
        }   
    }
//Free resources and close SDL
	close();
    
	return 0;
}
