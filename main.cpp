#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "graphics.h"
#include "game.h"


int main(int argc, char* argv[]){

    Graphics graphics;
    graphics.init();

    Game game;
    game.init(graphics);

    bool quit = false;
    SDL_Event e;

    while(! quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT) quit = true;
        }
        graphics.prepareScene();
        game.doLogic();
        game.render(graphics);
        graphics.presentScene();
        SDL_Delay(10);
    }
    graphics.quit();
    return 0;
}
