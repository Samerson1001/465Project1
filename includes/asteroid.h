#include <SDL.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <ctime>

#ifndef ASTEROID_H
#define ASTEROID_H

class Asteroid
{
public:
    Asteroid()
        : speed(2), screen(false)
    {
        asteroid = SDL_LoadBMP("includes/asteroid.bmp");
        ast.x = 640;
        ast.y = rand() % 480;
        ast.w = 32;
        ast.h = 32;
    }

    void left()
    {
        if (ast.x < 20)
        {
            screen = false;
            ast.x = 640;
            ast.y = rand() % 480;
        }
        ast.x -= speed;
        
    }

    
public:
    SDL_Rect ast;
    double speed;
    bool screen;
    SDL_Surface * asteroid;
};

#endif
