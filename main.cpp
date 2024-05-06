#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "graphics.h"
#include "defs.h"
#include "game.h"
using namespace std;


int main(int argc, char *argv[])
{
    Graphics graphics;
    graphics.init();
    Game game;
    game.init(graphics);
    SDL_Texture* Menu = graphics.loadTexture("Menu.png");
    SDL_Rect dest;
    dest.x = 0;
    dest.y = 0;
    dest.w = SCREEN_WIDTH;
    dest.h = SCREEN_HEIGHT;
    SDL_RenderCopy(graphics.renderer, Menu, NULL, &dest);
    bool quit = false;
    bool play = false;
    SDL_Event e;
    while(! quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_KEYDOWN){
                 if (e.key.keysym.scancode == SDL_SCANCODE_RETURN){
                    play = true;
                    SDL_DestroyTexture(Menu);
                 }
                 if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) quit = true;
            }
        }
        if (play == true){
        graphics.prepareScene();
        game.LogicGame();
        game.render(graphics);
        }
        graphics.presentScene();
    }
    game.Destroy();
    graphics.quit();

    return 0;
}
