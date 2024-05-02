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

	bool collides(Object* other) { // va chạm
	    return (max(x, other->x) < min(x + w, other->x + other->w))
	        && (max(y, other->y) < min(y + h, other->y + other->h));
	}

	void move() { // di chuyển
	    x += dx;
	    y += dy;
	}

	bool offScreen() { // di chuyển ra ngoài màn hình
	     return x < 0 || y < 0 || x > SCREEN_WIDTH || y > SCREEN_HEIGHT;
	}
};

#endif // _OBJECT__H
