#ifndef _GAME__H
#define _GAME__H
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <list>
#include <SDL_mixer.h>
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
    int change_bullet_counting = CHANGE_BULLET_COUNTING;
    int defend_counting = DEFEND_COUNTING;
    bool defend_status = false;

    bool play = false;
    bool help = false;

    int countingTillFinish = COUNTING_TILL_FINISH;
    Explosion explosion;
    Counting counting_life;
    Counting counting_target;
    Counting counting_changeBullet;
    Counting counting_defend;
    Background background;
    Object player;
    Mix_Music *gMusic;
    Mix_Chunk *explode;

    list<Object*> bullets;
	list<Object*> fighters;

    SDL_Texture *bulletTexture, *bulletTexture2, *enemyTexture, *enemyBulletTexture,
     *enemyBulletTexture2, *explosionTexture, *bossTexture, *defendTexture,
     *countingTexture, *countingTarget, *countingChangeBullet, *countingDefend,
     *Win, *Lose, *life, *target, *defend, *changeBullet, *Menu, *HelpTexture;


    int timeToCreateEnemy;
    int timeToResetStage;
    bool check_explosion = false;
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
        explode = graphics.loadSound("explode1.mp3");

        Menu = graphics.loadTexture("Menu1.png");
        Win = graphics.loadTexture("WinGame1.png");
        Lose = graphics.loadTexture("LoseGame1.png");
        HelpTexture = graphics.loadTexture("Help1.png");

        player.texture = graphics.loadTexture("player_1.png");
        SDL_QueryTexture(player.texture, NULL, NULL, &player.w, &player.h);
        bulletTexture = graphics.loadTexture("playerbullet.png");
        bulletTexture2 = graphics.loadTexture("purple_light.png");
        enemyTexture = graphics.loadTexture("enemy.png");
        defendTexture = graphics.loadTexture("defend.png");

        life = graphics.loadTexture("LIFE.png");
        target = graphics.loadTexture("Target.png");
        changeBullet = graphics.loadTexture("change_bullet_pic.png");
        defend = graphics.loadTexture("defendpic.png");

        explosionTexture = graphics.loadTexture("pngegg.png");
        explosion.init(explosionTexture, FRAMES, CLIPS);

        countingTexture = graphics.loadTexture("counting.png");
        counting_life.init(countingTexture, LIFE_FRAMES, LIFE_COUNT);

        countingTarget = graphics.loadTexture("counting.png");
        counting_target.init(countingTarget, LIFE_FRAMES, LIFE_COUNT);

        countingChangeBullet = graphics.loadTexture("counting.png");
        counting_changeBullet.init(countingChangeBullet, LIFE_FRAMES, LIFE_COUNT);
        counting_changeBullet.tick(change_bullet_counting);

        countingDefend = graphics.loadTexture("counting.png");
        counting_defend.init(countingChangeBullet, LIFE_FRAMES, LIFE_COUNT);
        counting_defend.tick(defend_counting);

        graphics.playMusic(gMusic);

        reset();
    }

    void updateLevel(Graphics& graphics, int LEVEL){
        if (LEVEL == 1){
            background.texture = graphics.loadTexture("bg11.png");
        }
        if (LEVEL == 2){
            background.texture = graphics.loadTexture("bg2.png");
            enemyBulletTexture = graphics.loadTexture("bulletobject.png");
        }
        if (LEVEL == 3){
            background.texture = graphics.loadTexture("bg31.png");
        }
        if (LEVEL == 4){
            background.texture = graphics.loadTexture("bg41.png");
            enemyBulletTexture = graphics.loadTexture("red_light.png");
        }
        if (LEVEL == 5){
            background.texture = graphics.loadTexture ("bg51.png");
            enemyBulletTexture2 = graphics.loadTexture ("red_light.png");
            enemyBulletTexture = graphics.loadTexture ("flash.png");
            bossTexture = graphics.loadTexture ("boss1.png");
        }
        if (LEVEL == 6){
            background.texture = graphics.loadTexture ("bg61.png");
           enemyBulletTexture = graphics.loadTexture ("red_light.png");
            enemyBulletTexture2 = graphics.loadTexture ("bulletobject.png");
        }
        if (LEVEL == 7){
            background.texture = graphics.loadTexture ("bg71.png");
            enemyBulletTexture2 = graphics.loadTexture ("red_light.png");
            enemyBulletTexture = graphics.loadTexture ("flash.png");
            bossTexture = graphics.loadTexture ("boss2.png");
        }
        reset();
    }

    void createBoss(){
        Object* boss = new Object();
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

    void setPlayerBullet(int type)
    {
        Object *bullet = new Object();
        bullets.push_back(bullet);
        bullet->x = player.x;
        bullet->y = player.y + (player.h / 2) - (bullet->h / 2);
        bullet->dx = PLAYER_BULLET_SPEED;
        bullet->health = 10;
        if (type == 1) bullet->texture = bulletTexture;
        if (type == 2) bullet->texture = bulletTexture2;
        bullet->side = SIDE_PLAYER;
        SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);
        player.reload = PLAYER_RELOAD;
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
        }
        if (change_bullet <= 0) {
            bullet-> texture = enemyBulletTexture;
            change_bullet = 10;
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
        if (currentKeyStates[SDL_SCANCODE_A]) player.x = 20;
        if (currentKeyStates[SDL_SCANCODE_D]) player.x = 980;
        if (currentKeyStates[SDL_SCANCODE_W]) player.y = 20;
        if (currentKeyStates[SDL_SCANCODE_S]) player.y = 980;
        if (currentKeyStates[SDL_SCANCODE_SPACE] && player.reload == 0) setPlayerBullet(1);
        if (currentKeyStates[SDL_SCANCODE_Q]&& player.reload == 0){
            if (change_bullet_counting > 0){
                cerr << "Change bullet " << change_bullet_counting << endl;
                setPlayerBullet(2);
                change_bullet_counting--;
            }
        }
        if (currentKeyStates[SDL_SCANCODE_E]&& player.reload == 0){
            if (defend_counting > 0){
                cerr << "defend counting" << defend_counting << endl;
                defend_status = true;
                countingTillFinish = COUNTING_TILL_FINISH;
                defend_counting--;
                player.reload = PLAYER_RELOAD;
            }
        }
    }

    bool bulletHitFighter(Object *b)
    {
        for (Object* fighter: fighters) {
            if (fighter->side != b->side && b->collides(fighter)) {
                if (fighter == &player && defend_status == true){
                    return false;
                }
                if (fighter->texture != bossTexture || (fighter == &player && defend_status == false)){
                    fighter->health =0;
                }
                if (fighter != &player){
                     --targetToKill;
                    if (targetToKill == 0){
                        WinTheGame = true;
                        LEVEL++;
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
                check_explosion  = true;
                bulletCollidesX = b->x;
                bulletCollidesY = b->y;
                if (b->texture != bulletTexture2){
                    delete b;
                    bullets.erase(temp);
                }
            }
            if (b->offScreen()){
                delete b;
                bullets.erase(temp);
            }
            if (defend_status == true){
                countingTillFinish--;
                if (countingTillFinish < 0){
                    defend_status = false;
                }
            }
        }
    }

    void fireEnemiesBullet(){
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
            enemy->dx = -(1 + (rand() % 5));
            enemy->health = 1;
            enemy->reload = FRAME_PER_SECOND * (2 + (rand() % 2));
            enemy->side = SIDE_ALIEN;
            enemy->texture = enemyTexture;
            SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);
            if (LEVEL <= 5){
                timeToCreateEnemy = 20 + (rand() % 30);
            }
            if (LEVEL > 5){
                timeToCreateEnemy = 100 + (rand() % 75);
            }
        }
    }

    void controlFighters()
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
            if ( fighter->y == 20){
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
            play = false;
            return true;
	    }
	    return false;
	}
	bool LOSEGAME(){
	    if (Life == 0){
            play = false;
            return true;
	    }
	    return false;
	}

    void LogicGame() {
        if (play == false){
            const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
            if (currentKeyStates[SDL_SCANCODE_RETURN]){
                    help = false;
                    play = true;
                    WinTheGame = true;
                    Life = LIFE_PLAYER;
                    LEVEL = _LEVEL;
                    change_bullet_counting = CHANGE_BULLET_COUNTING;
                    defend_counting = DEFEND_COUNTING;
                    check_explosion = 0;
                    bulletCollidesX = 0;
                    bulletCollidesY = 0;
                    defend_status = false;
                    reset();
            }
            if (currentKeyStates[SDL_SCANCODE_H]){
                help = true;
            }
        }
        if (WINGAME() == false && LOSEGAME() == false && play == true){
            background.doBackground();
            controlPlayer();
            controlFighters();
            if (LEVEL >= 2){
                fireEnemiesBullet();
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
        if (play == false){
            SDL_Rect dest;
            dest.x = 0;
            dest.y = 0;
            dest.w = SCREEN_WIDTH;
            dest.h = SCREEN_HEIGHT;
            SDL_RenderCopy(graphics.renderer, Menu, NULL, &dest);
        }
        if (help == true){
            SDL_Rect dest;
            dest.x = 0;
            dest.y = 0;
            dest.w = SCREEN_WIDTH;
            dest.h = SCREEN_HEIGHT;
            SDL_RenderCopy(graphics.renderer, HelpTexture, NULL, &dest);

        }
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
        if (WINGAME() == false && LOSEGAME() == false && play == true){

            if (WinTheGame == true){
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

            counting_life.tick(Life);
            graphics.renderTexture(life, 20, 20);
            graphics.renderCounting(100, 20, counting_life);

            graphics.renderTexture(target, 780, 20);
            int c = targetToKill;
            if (c < 0) c = 1;
            counting_target.tick(c);
            graphics.renderCounting(910, 20, counting_target);

            counting_changeBullet.tick(change_bullet_counting);
            if (change_bullet_counting >0){
                graphics.renderTexture(changeBullet,20, 650);
                graphics.renderCounting(130, 650, counting_changeBullet);
            }

            counting_defend.tick(defend_counting);
            if (defend_counting >0){
                graphics.renderTexture(defend,830, 650);
                graphics.renderCounting(910, 650, counting_defend);
            }

            if (defend_status == true){
                graphics.renderTexture(defendTexture, player.x, player.y);
            }
            if (check_explosion == true){
                graphics.play(explode);
                drawExplosion(graphics);

                check_explosion = false;
            }
        }
    }

    void Destroy(){
        if (gMusic != nullptr) Mix_FreeMusic(gMusic);
        if (explode != nullptr) Mix_FreeChunk(explode);
    }
};


#endif // _GAME__H
