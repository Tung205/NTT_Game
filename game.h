#ifndef _GAME__H
#define _GAME__H
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <list>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "defs.h"
#include "graphics.h"
#include "background.h"
#include "object.h"
#include "explosion.h"
#include "counting.h"

struct Game {

    int Life = LIFE_PLAYER;
    bool WinTheGame = false;
    int LEVEL = _LEVEL;
    int targetToKill = TARGET_TO_KILL;
    int exist_boss = 0;
    int change_bullet = CHANGE_BULLET;
    Explosion explosion;
    Counting counting_life;
    Counting counting_target;
    Background background;
    Object player;
    Object* boss_temp;
    Mix_Music *gMusic;
    Mix_Chunk *explode;

    list<Object*> bullets;
	list<Object*> fighters;

    SDL_Texture *bulletTexture, *enemyTexture, *enemyBulletTexture,
     *enemyBulletTexture2, *explosionTexture, *bossTexture,
     *countingTexture, *countingTarget, *Win, *Lose, *life, *target;


    int timeToCreateEnemy;
    int timeToResetStage;
    int check_explosion = 0;
    int bulletCollidesX = 0;
    int bulletCollidesY = 0;

    void empty(list<Object*>& object){
        while (!object.empty()) {
            Object* e = object.front();
            object.pop_front();
            if (e != &player){
                    delete e;
                    e = nullptr;
            }
        }
    }

    void reset(){
        empty(fighters);
        empty(bullets);
        fighters.push_back(&player);
	    initPlayer(player);
        timeToCreateEnemy = 0;
        timeToResetStage = FRAME_PER_SECOND * 3;
        targetToKill = TARGET_TO_KILL;
        exist_boss = 0;
        change_bullet = 10;
	}

    void init(Graphics& graphics)
    {
        Mix_Music *gMusic1 = graphics.loadMusic("sound3.mp3");
        gMusic = graphics.loadMusic("sound3.mp3");
        explode = graphics.loadSound("explode.mp3");

        Win = graphics.loadTexture("WinGame.png");
        Lose = graphics.loadTexture("LoseGame.png");

        player.texture = graphics.loadTexture("player_1.png");
        SDL_QueryTexture(player.texture, NULL, NULL, &player.w, &player.h);
        bulletTexture = graphics.loadTexture("playerbullet.png");
        enemyTexture = graphics.loadTexture("enemy.png");
        background.texture = graphics.loadTexture("bg1.png");

        life = graphics.loadTexture("LIFE.png");
        target = graphics.loadTexture("Target.png");

        explosionTexture = graphics.loadTexture("pngegg.png");
        explosion.init(explosionTexture, FRAMES, CLIPS);

        countingTexture = graphics.loadTexture("counting.png");
        counting_life.init(countingTexture, LIFE_FRAMES, LIFE_COUNT);

        countingTarget = graphics.loadTexture("counting.png");
        counting_target.init(countingTarget, LIFE_FRAMES, LIFE_COUNT);

        graphics.playMusic(gMusic);

        reset();
    }

    void updateLevel(Graphics& graphics, int LEVEL){
        if (LEVEL == 2){
            background.texture = graphics.loadTexture("bg2.png");
            enemyBulletTexture = graphics.loadTexture("bulletobject.png");
        }
        if (LEVEL == 3){
            background.texture = graphics.loadTexture("bg3.png");
        }
        if (LEVEL == 4){
            background.texture = graphics.loadTexture("bg4.png");
            enemyBulletTexture = graphics.loadTexture("red_light.png");
        }
        if (LEVEL == 5){
            background.texture = graphics.loadTexture ("bg5.png");
            enemyBulletTexture2 = graphics.loadTexture ("red_light.png");
            enemyBulletTexture = graphics.loadTexture ("flash.png");
            bossTexture = graphics.loadTexture ("boss1.png");
        }
        if (LEVEL == 6){
            background.texture = graphics.loadTexture ("bg6.png");
           enemyBulletTexture = graphics.loadTexture ("red_light.png");
            enemyBulletTexture2 = graphics.loadTexture ("bulletobject.png");
        }
        if (LEVEL == 7){
            background.texture = graphics.loadTexture ("bg7.png");
            enemyBulletTexture2 = graphics.loadTexture ("red_light.png");
            enemyBulletTexture = graphics.loadTexture ("flash.png");
            bossTexture = graphics.loadTexture ("boss2.png");
        }
        reset();
    }

    void createBoss(){
        Object* boss = new Object();
        boss_temp = boss;
        fighters.push_back(boss);
        boss->x = SCREEN_WIDTH * 3 /4;
        boss->y = SCREEN_HEIGHT/2;
        if (LEVEL == 5 || LEVEL  == 6){
        boss->dy = 5;
        } else {
        boss->dy = 0;
        }
        boss->dx = 0;
        boss->health = 1;
        boss->side = SIDE_ALIEN;
        boss->texture = bossTexture;
        SDL_QueryTexture(boss->texture, NULL, NULL,& boss->w,& boss->h);
        boss->reload = 10;
    }

    void setPlayerBullet()
    {
        Object *bullet = new Object();
        bullets.push_back(bullet);
        bullet->x = player.x;
        bullet->y = player.y + (player.h / 2) - (bullet->h / 2);
        bullet->dx = PLAYER_BULLET_SPEED;
        bullet->health = 10;
        bullet->texture = bulletTexture;
        bullet->side = SIDE_PLAYER;
        SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);
        player.reload = PLAYER_RELOAD; // không có dòng này thì đạn lã 1 loạt
    }

    void setEnemyBullet(Object* enemy)
    {
        Object *bullet = new Object();
        bullets.push_back(bullet);
        bullet->x = enemy->x;
        bullet->y = enemy->y;
        bullet->health = 1;
        if (LEVEL >= 5){
        if (change_bullet > 0){
        bullet->texture = enemyBulletTexture2;
        change_bullet--;
            if (change_bullet <= 0) {
                bullet-> texture = enemyBulletTexture;
                change_bullet = 10;
            }
        }
        } else {
        bullet->texture = enemyBulletTexture;
        }
        bullet->side = SIDE_ALIEN;
        SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

        bullet->x += (enemy->w / 2) - (bullet->w / 2);
        bullet->y += (enemy->h / 2) - (bullet->h / 2);
        if (LEVEL > 2){
        DirectionOriented(player.x + (player.w / 2), player.y + (player.h / 2),
                  enemy->x, enemy->y, &bullet->dx, &bullet->dy, LEVEL);
        }

        bullet->dx *= ENEMY_BULLET_SPEED;
        bullet->dy *= ENEMY_BULLET_SPEED;

        enemy->reload = (rand() % FRAME_PER_SECOND * 2);
    }

    void controlPlayer(){

        if (player.health == 0) return;
        if (player.reload > 0) player.reload--;
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_UP]) player.dy = -PLAYER_SPEED;
        if (currentKeyStates[SDL_SCANCODE_DOWN]) player.dy = PLAYER_SPEED;
        if (currentKeyStates[SDL_SCANCODE_LEFT]) player.dx = -PLAYER_SPEED;
        if (currentKeyStates[SDL_SCANCODE_RIGHT])player.dx = PLAYER_SPEED;
        if (currentKeyStates[SDL_SCANCODE_SPACE] && player.reload == 0) setPlayerBullet();
    }

    bool bulletHitFighter(Object *b)
    {
        for (Object* fighter: fighters) {
            if (fighter->side != b->side && b->collides(fighter)) {

                if (fighter != boss_temp){
                fighter->health =0;
                }
                if (fighter != &player){
                     --targetToKill;
                    if (targetToKill == 0){
                        WinTheGame = true;
                    }
                }
                return true;
            }
        }
        return false;
    }

    void fireBullets()
    {
        auto it = bullets.begin();
        while (it != bullets.end()) {
            auto temp = it++;
            Object* b = *temp;
            b->move();
            if (bulletHitFighter(b)){
                ++check_explosion;
                bulletCollidesX = b->x;
                bulletCollidesY = b->y;
            }
            if (bulletHitFighter(b)|| b->offScreen()){
                delete b;
                bullets.erase(temp);
            }
        }
    }

    void doEnemies(){
        for (Object* e: fighters) {
            if (e != &player && player.health != 0 && --e->reload <= 0){
                setEnemyBullet(e);
            }
        }
    }

    void CreateEnemy() {
        if (--timeToCreateEnemy <= 0) {
            Object *enemy = new Object();
            fighters.push_back(enemy);
            enemy->x = SCREEN_WIDTH;
            enemy->y = rand() % SCREEN_HEIGHT;
            enemy->dx = -(2 + (rand() % 4));
            enemy->health = 1;
            enemy->reload = FRAME_PER_SECOND * (1 + (rand() % 3));
            enemy->side = SIDE_ALIEN;
            enemy->texture = enemyTexture;
            SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);
            if (LEVEL <= 5){
            timeToCreateEnemy = 30 + (rand() % 80);
            }
            if (LEVEL > 5){
                timeToCreateEnemy = 100 + (rand() % 75);
            }
        }
    }


    void doFighters()
    {
        auto it = fighters.begin();
        it++;
        while (it != fighters.end()) {
            auto temp = it++;
            Object* fighter = *temp;
            fighter->move();
            if (fighter->y == SCREEN_HEIGHT-30){
                fighter-> dy = - 5;
            }
            if ( fighter->y == 0+20){
                fighter-> dy = 5;
            }

            if (fighter->x < -fighter->w) fighter->health = 0;
            if (fighter->health == 0) {
                delete fighter;
                fighters.erase(temp);
                continue;
            }
        }

        player.move();
        player.dx = 0;
        player.dy = 0;
        if (player.x < 0) player.x = 0;
        else if (player.x >= SCREEN_WIDTH - player.w)
            player.x = SCREEN_WIDTH - player.w;
        if (player.y < 0) player.y = 0;
        else if (player.y >= SCREEN_HEIGHT - player.h)
            player.y = SCREEN_HEIGHT - player.h;
	}
	bool WINGAME(){
	    if( LEVEL == 8){
            return true;
	    }
	    return false;
	}
	bool LOSEGAME(){
	    if (Life == 0){
            return true;
	    }
	    return false;
	}

    void LogicGame() {
        if (WINGAME() == false && LOSEGAME() == false){
            background.doBackground();
            controlPlayer();
            doFighters();
            if (LEVEL >= 2){
                doEnemies();
            }
            if (LEVEL >= 5) {
                if (exist_boss ==0){
                    createBoss();
                    exist_boss++;
                }
            }
            if (LEVEL != 5){
                CreateEnemy();
            }
        fireBullets();

        if (player.health == 0 && --timeToResetStage <= 0){
                Life--;
                counting_life.tick(Life);
                reset();
            }
        }
    }

    void drawExplosion(Graphics& graphics){
        while(explosion.currentFrame < FRAMES){
            explosion.tick();
            graphics.render(bulletCollidesX, bulletCollidesY, explosion);
        }
        explosion.currentFrame = 0;
    }

    void render(Graphics& graphics) {
        if (WINGAME() == true){
            SDL_Rect dest;
            dest.x = 0;
            dest.y = 0;
            dest.w = SCREEN_WIDTH;
            dest.h = SCREEN_HEIGHT;
            SDL_RenderCopy(graphics.renderer, Win, NULL, &dest);
        }
        if (LOSEGAME() == true){
            SDL_Rect dest1;
            dest1.x = 0;
            dest1.y = 0;
            dest1.w = SCREEN_WIDTH;
            dest1.h = SCREEN_HEIGHT;
            SDL_RenderCopy(graphics.renderer, Lose, NULL, &dest1);
        }
        if (WINGAME() == false && LOSEGAME() == false){
            if (WinTheGame == true){
                LEVEL++;
                updateLevel(graphics, LEVEL);
                WinTheGame = false;
            }
            background.drawBackground(graphics.renderer);

            for (Object* b: bullets){
                graphics.renderTexture(b->texture, b->x, b->y);
            }

            for (Object* b: fighters){
                if (b->health > 0){
                    graphics.renderTexture(b->texture, b->x, b->y);
                }
            }

            graphics.renderTexture(life, 20, 20);
            graphics.renderCounting(100, 20, counting_life);

            graphics.renderTexture(target, 780, 20);
            int c = ((int)targetToKill/2);
            if (c < 0) c = 1;
            counting_target.tick(c);
            graphics.renderCounting(910, 20, counting_target);

            if (check_explosion > 0){
                drawExplosion(graphics);
                check_explosion--;
                graphics.play(explode);
            }
        }
    }

    void Destroy(){
        if (gMusic != nullptr) Mix_FreeMusic( gMusic);
        if (explode != nullptr) Mix_FreeChunk( explode);
    }
};


#endif // _GAME__H
