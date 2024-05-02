#ifndef _BACKGROUND__H
#define _BACKGROUND__H
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "graphics.h"
class Background{
    public:
    int backgroundX = 0;
    SDL_Texture * texture;
    void doBackground(){
        if (--backgroundX < -SCREEN_WIDTH)
        {
            backgroundX = 0;
        }
    }
    void drawBackground(SDL_Renderer* renderer) {
        SDL_Rect dest;
        for (int x = backgroundX ; x < SCREEN_WIDTH ; x += SCREEN_WIDTH) {
            dest.x = x;
            dest.y = 0;
            dest.w = SCREEN_WIDTH;
            dest.h = SCREEN_HEIGHT;

            SDL_RenderCopy(renderer, texture, NULL, &dest);
        }
    }
};
#endif // _BACKGROUND__H

