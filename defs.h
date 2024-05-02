#ifndef _DEFS__H
#define _DEFS__H

#define WINDOW_TITLE "GameBANDAN"
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700

#define TARGET_TO_KILL 10
#define _LEVEL  1
#define PLAYER_SPEED          6
#define PLAYER_BULLET_SPEED   20
#define PLAYER_RELOAD          8
#define MAX_KEYBOARD_KEYS 350

#define SIDE_PLAYER 0
#define SIDE_ALIEN  1

#define FRAME_PER_SECOND 60

#define ENEMY_BULLET_SPEED    5


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
#endif
