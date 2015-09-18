#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Asteroid X constant
const int ast_x = 608;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

void terr_generation();
void terr_print();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

SDL_Surface* gScreenSurface= NULL;
