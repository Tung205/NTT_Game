#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
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
    bool quit = false;
    SDL_Event e;
    while(! quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_KEYDOWN){
                if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) quit = true;
            }
        }

        graphics.prepareScene();
        game.LogicGame();
        game.render(graphics);
        graphics.presentScene();
    }
    game.Destroy();
    graphics.quit();

    return 0;
}
