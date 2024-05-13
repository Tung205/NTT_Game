#ifndef _DEFS__H
#define _DEFS__H

#define WINDOW_TITLE "Game Shooting Ghost!!!"
#define SCREEN_WIDTH                1000
#define SCREEN_HEIGHT               700

#define LIFE_PLAYER                 15
#define TARGET_TO_KILL              30
#define CHANGE_BULLET               10
#define _LEVEL                      1
#define PLAYER_SPEED                10
#define PLAYER_BULLET_SPEED         20
#define PLAYER_RELOAD               8
#define CHANGE_BULLET_COUNTING      3
#define SIDE_PLAYER                 0
#define SIDE_ALIEN                  1
#define FRAME_PER_SECOND            60
#define ENEMY_BULLET_SPEED          5
#define DEFEND_COUNTING             5
#define COUNTING_TILL_FINISH        2000

const int CLIPS[][4] = {
    {5, 5, 175, 175},
    {185, 5, 175, 175},
    {365, 5, 175, 180},
    {545, 5, 175, 175},
    {755, 5, 175, 175},

    {5, 185, 175, 175},
    {185, 185, 175, 175},
    {365, 185, 175, 180},
    {545, 185, 175, 175},
    {755, 185, 175, 175},

    {5, 365, 175, 175},
    {185, 365, 175, 175},
    {365, 365, 175, 180},
    {545, 365, 175, 175},
    {755, 365, 175, 175},

    {5, 545, 175, 175},
    {185, 545, 175, 175},
    {365, 545, 175, 180},
    {545, 545, 175, 175},
    {755, 545, 175, 175},

};
const int FRAMES = sizeof(CLIPS)/sizeof(int)/4;

const int LIFE_COUNT[][4]{
    {0, 0, 50, 40},
    {50, 0, 50, 40},
    {100, 0, 50, 40},
    {150, 0, 50, 40},
    {200, 0, 50, 40},
    {250, 0, 50, 40},

    {0, 40, 50, 40},
    {50, 40, 50, 40},
    {100, 40, 50, 40},
    {150, 40, 50, 40},
    {200, 40, 50, 40},
    {250, 40, 50, 40},

    {0, 80, 50, 40},
    {50, 80, 50, 40},
    {100, 80, 50, 40},
    {150, 80, 50, 40},
    {200, 80, 50, 40},
    {250, 80, 50, 40},

    {0, 120, 50, 40},
    {50, 120, 50, 40},
    {100, 120, 50, 40},
    {150, 120, 50, 40},
    {200, 120, 50, 40},
    {250, 120, 50, 40},

    {0, 160, 50, 40},
    {50, 160, 50, 40},
    {100, 160, 50, 40},
    {150, 160, 50, 40},
    {200, 160, 50, 40},
    {250, 160, 50, 40},
};
const int LIFE_FRAMES = sizeof(LIFE_COUNT)/ sizeof(int)/4;
#endif
