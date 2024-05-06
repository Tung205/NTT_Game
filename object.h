#ifndef _OBJECT__H
#define _OBJECT__H
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "graphics.h"
#include "background.h"
using namespace std;
class Object {
    public:
    int x;
	int y;
	int w;
	int h;
    float dx;
	float dy;
	int side;
	int health;
	int reload;
	SDL_Texture *texture;

	bool collides(Object* other) {
	    return (max(x, other->x) < min(x + w, other->x + other->w))
	        && (max(y, other->y) < min(y + h, other->y + other->h));
	}

	void move() {
	    x += dx;
	    y += dy;
	}

	bool offScreen() {
	     if(x < 0 || y < 0 || x > SCREEN_WIDTH || y > SCREEN_HEIGHT){
            return true;
	     }
	     return false;
	}
};

void initPlayer(Object& player) {
    player.x = 100;
    player.y = 100;
    player.health = 1;
    player.side = SIDE_PLAYER;
    player.reload = 0;
}

void DirectionOriented(int x1, int y1, int x2, int y2, float *dx, float *dy, int LEVEL)
{
	float steps = max(abs(x1 - x2), abs(y1 - y2));

	if (steps == 0)
	{
		*dx = *dy = 0;
		return;
	}
	*dx = (float)(x1 - x2)/steps;
	*dy = (float)(y1 - y2)/steps;
	if (LEVEL < 5){
        *dx = (int)*dx;
        *dy = (int)*dy;
	}
}

#endif // _OBJECT__H
