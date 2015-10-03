#ifndef MENU_H
#define MENU_H

// General layout of menu was referenced from Lazy Foo Tutorials
bool menu(SDL_Surface* screen, TTF_Font* font)
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

				// Free surfaces once the menu items have been clicked and return true or false depending on which item was clicked
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
					if(event.key.keysym.sym == SDLK_ESCAPE)
					{
						for(int i = 0; i < MENU_ITEMS; i++)
						{
							SDL_FreeSurface(menus[i]);
						}

						return 1;
					}
					else if (event.key.keysym.sym == SDLK_RETURN)
					{
						for (int i = 0; i < MENU_ITEMS; i++)
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

#endif
