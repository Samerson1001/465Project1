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
        : speed(4), screen(false)
    {
        asteroid = SDL_LoadBMP("includes/asteroid.bmp");
        ast.x = 680;
        ast.y = rand() % 480;
        ast.w = 32;
        ast.h = 32;
    }

    void left()
    {
        if (ast.x < 5)
        {
            screen = false;
            ast.x = 640;
            ast.y = rand() % 480;
        }
        ast.x -= speed;
        
    }
    void print()
    {
        if (screen)
        {
           
            left();
            SDL_BlitSurface(asteroid, NULL,
                                gScreenSurface, &ast);
        }
    }
    bool overlap_check(Asteroid & asteroid)
    {
        bool flag = true;
        
        // Find edges of rect1
        int left1 = ast.x;
        int right1 = ast.x + ast.w;
        int top1 = ast.y;
        int bottom1 = ast.y + ast.h;
        
        // Find edges of rect2
        int left2 = asteroid.ast.x;
        int right2 = asteroid.ast.x+ asteroid.ast.w;
        int top2 = asteroid.ast.y;
        int bottom2 = asteroid.ast.y + asteroid.ast.h;
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


    
        return flag;
    }

    
public:
    SDL_Rect ast;
    double speed;
    bool screen;
    SDL_Surface * asteroid;
};

#endif
