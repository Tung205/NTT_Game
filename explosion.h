#ifndef _EXPLOSION__H
#define _EXPLOSION__H
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include <vector>

class Explosion {
    public:
    SDL_Texture* texture;
    std::vector<SDL_Rect> clips;
    int currentFrame = 0;

    void init(SDL_Texture* _texture, int frames, const int _clips [][4]) {
        texture = _texture;

        SDL_Rect clip;
        for (int i = 0; i < frames; i++) {
            clip.x = _clips[i][0];
            clip.y = _clips[i][1];
            clip.w = _clips[i][2];
            clip.h = _clips[i][3];
            clips.push_back(clip);
        }
    }
    void tick() {
        currentFrame = (currentFrame + 1) ;
    }

    const SDL_Rect* getCurrentClip() const {
        return &(clips[currentFrame]);
    }
};
#endif // _EXPLOSION__H
