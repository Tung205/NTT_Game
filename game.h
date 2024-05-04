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

void initPlayer(Object& player) {
    player.x = 100;
    player.y = 100;
    player.health = 1;
    player.side = SIDE_PLAYER;
    player.reload = 0;
}

void DirectionOriented(int x1, int y1, int x2, int y2, float *dx, float *dy, int LEVEL) // tính toán độ dốc ?? (hệ số góc)
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

struct Game {
    int check = 1;
    bool WinTheGame = false;
    int LEVEL = _LEVEL;
    int targetToKill = TARGET_TO_KILL;
    int exist_boss = 0;
    int change_bullet = 10;
    Explosion explosion;
    Background background;

    Object player; // thực thể người chơi -> player
    Object* boss_temp;
    Mix_Music *gMusic;
    Mix_Chunk *explode;
    // 1 loạt các list
    list<Object*> bullets; // list đạn bắn -> bullet
	list<Object*> fighters; // list con quái -> enemy

    SDL_Texture *bulletTexture, *enemyTexture, *enemyBulletTexture, *enemyBulletTexture2, *explosionTexture, *bossTexture;


    int timeToCreateEnemy; // bộ hẹn thời gian xuất hiện quái vật
    int timeToResetStage; // thiết lập thời gian lặp lại giai đoạn
    int check_explosion = 0;
    int bulletCollidesX = 0;
    int bulletCollidesY = 0;

    void empty(list<Object*>& object) { //-> game
        while (!object.empty()) {
            Object* e = object.front();
            object.pop_front();
            if (e != &player){
                    delete e;
                    e = nullptr;
            }
        }
    }

    void reset() // reset mọi thứ -> game
    {
        empty(fighters);
        empty(bullets);
        fighters.push_back(&player);

	    initPlayer(player);

        timeToCreateEnemy = 0;
        timeToResetStage = FRAME_PER_SECOND * 3;
        targetToKill = TARGET_TO_KILL;
        exist_boss = 1;
        change_bullet = 10;

	}
	void initMusic(Graphics& graphics){
        Mix_Music *gMusic1 = graphics.loadMusic("sound3.mp3");
      //  Mix_Music *gMusic = graphics.loadMusic("sound3.mp3");
        Mix_Chunk *explode1 = graphics.loadSound("explode.mp3");
        explode = explode1;
      // graphics.playMusic(gMusic);
    }
    void init(Graphics& graphics) // load các hình ảnh -> game
    {
        player.texture = graphics.loadTexture("player_1.png");
        SDL_QueryTexture(player.texture, NULL, NULL, &player.w, &player.h);
        bulletTexture = graphics.loadTexture("playerbullet.png");
        enemyTexture = graphics.loadTexture("enemy.png");
        background.texture = graphics.loadTexture("background_Lv1.png");
        explosionTexture = graphics.loadTexture("pngegg.png");
        explosion.init(explosionTexture, FRAMES, CLIPS);
        Mix_Music *gMusic1 = graphics.loadMusic("sound3.mp3");
          gMusic = graphics.loadMusic("sound3.mp3");
        explode = graphics.loadSound("explode.mp3");

       graphics.playMusic(gMusic);
        reset();
    }

    void updateLevel(Graphics& graphics, int LEVEL){
        if (LEVEL == 2){
            background.texture = graphics.loadTexture("background_Lv2.png");
            enemyBulletTexture = graphics.loadTexture("bulletobject.png");
        }
        if (LEVEL == 3){
            background.texture = graphics.loadTexture("background_Lv3.png");
           // enemyBulletTexture = graphics.loadTexture("bulletobject.png");
        }
        if (LEVEL == 4){
            background.texture = graphics.loadTexture("background4.png");
            enemyBulletTexture = graphics.loadTexture("red_light.png");
        }
        if (LEVEL == 5){
            background.texture = graphics.loadTexture ("background_Lv2.png");
            enemyBulletTexture2 = graphics.loadTexture ("red_light.png");
            enemyBulletTexture = graphics.loadTexture ("flash.png");
            bossTexture = graphics.loadTexture ("boss1.png");
        }
        if (LEVEL == 6){
            background.texture = graphics.loadTexture ("background_Lv6.png");
           enemyBulletTexture = graphics.loadTexture ("red_light.png");
            enemyBulletTexture2 = graphics.loadTexture ("bulletobject.png");
          //  bossTexture = graphics.loadTexture ("boss1.png");
        }
        if (LEVEL == 7){
            background.texture = graphics.loadTexture ("Background.png");
            enemyBulletTexture2 = graphics.loadTexture ("red_light.png");
           enemyBulletTexture = graphics.loadTexture ("flash.png");
            bossTexture = graphics.loadTexture ("boss2.png");
        }
        reset();
    }

    void createBoss(){
        if (--timeToCreateEnemy <= 0){
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
    }

    void setPlayerBullet() // khai hỏa bên mình -> player
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

        player.reload = PLAYER_RELOAD;
    }

    void setEnemyBullet(Object* enemy) // khai hỏa bên đối thủ -> enemy
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

    void doPlayer(){

        if (player.health == 0) return;
        if (player.reload > 0) player.reload--;
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_UP]) player.dy = -PLAYER_SPEED;
        if (currentKeyStates[SDL_SCANCODE_DOWN]) player.dy = PLAYER_SPEED;
        if (currentKeyStates[SDL_SCANCODE_LEFT]) player.dx = -PLAYER_SPEED;
        if (currentKeyStates[SDL_SCANCODE_RIGHT])player.dx = PLAYER_SPEED;
        if (currentKeyStates[SDL_SCANCODE_SPACE] && player.reload == 0) setPlayerBullet();
         // nhân vật di chuyển
    }

    bool bulletHitFighter(Object *b) // va chạm -> đạn bắn trúng địch -> enemy
    {
        for (Object* fighter: fighters) {
            if (fighter->side != b->side && b->collides(fighter)) {

                if (fighter != boss_temp){
                fighter->health =0;
                }
                return true;
            }
        }
        return false;
    }

    void doBullets() //-> bullet
    {
        auto it = bullets.begin();
        while (it != bullets.end()) {
            auto temp = it++;
            Object* b = *temp; // đạn bắn thứ i trong 1 băng đạn
            b->move(); // di chuyển
            if (bulletHitFighter(b)){
                ++check_explosion;
                bulletCollidesX = b->x;
                bulletCollidesY = b->y;
                --targetToKill;
                if (targetToKill == 0){
                    WinTheGame = true;
                }
            }
            if (bulletHitFighter(b)|| b->offScreen()) { //nếu bắn trúng địch hoặc ra khỏi màn hình -> xóa.
                delete b;
                bullets.erase(temp);
            }


        }
    }

    void doEnemies() { // -> enemy

        for (Object* e: fighters) { // thực thể địch

            if (e != &player && player.health != 0 && --e->reload <= 0) // nếu e không phải người chơi và máu khác 0 và reload <0
                setEnemyBullet(e); // khai hỏa bên đối thủ

        }
    }

    void CreateEnemy(void) { //-> enemy
        if (--timeToCreateEnemy <= 0) { // khi bộ đếm thời gian trở về 0
            Object *enemy = new Object();
            fighters.push_back(enemy);
            enemy->x = SCREEN_WIDTH;
            enemy->y = rand() % SCREEN_HEIGHT;
            enemy->dx = -(2 + (rand() % 4));
            enemy->health = 1;
            enemy->reload = FRAME_PER_SECOND * (1 + (rand() % 3));
            enemy->side = SIDE_ALIEN;
            enemy->texture = enemyTexture;
            SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h); // tạo quái
            if (LEVEL <= 5){
            timeToCreateEnemy = 30 + (rand() % 80);
            }
            if (LEVEL > 5){
                timeToCreateEnemy = 100 + (rand() % 75);
            } // tiếp tục set thời gian tiếp
        }
    }


    void doFighters(void) // nhân vật và quái di chuyển -> game (gộp 2 cái enemy & player)
    {
        auto it = fighters.begin(); // -> enemy
        it++;


        while (it != fighters.end()) {
            auto temp = it++;
            Object* fighter = *temp;
            fighter->move(); // quái vật di chuyển
            if (fighter->y == SCREEN_HEIGHT-30){
                fighter-> dy = - 5;
            }
            if ( fighter->y == 0+20){
                fighter-> dy = 5;
            }

            if (fighter->x < -fighter->w) fighter->health = 0; //-> out màn hình -> sức mạnh = 0 -> delete

            if (fighter->health == 0) {
                delete fighter;
                fighters.erase(temp);
                continue;
            }
        }

        player.move(); // nhân vật di chuyển -> player
        player.dx = 0;
        player.dy = 0;
        if (player.x < 0) player.x = 0;
        else if (player.x >= SCREEN_WIDTH - player.w)
            player.x = SCREEN_WIDTH - player.w;
        if (player.y < 0) player.y = 0;
        else if (player.y >= SCREEN_HEIGHT - player.h)
            player.y = SCREEN_HEIGHT - player.h;
	}

    void doLogic() { //-> game

        background.doBackground();

        doPlayer();
        doFighters();
        if (LEVEL >= 2){
        doEnemies();
        }

        if (LEVEL >= 5) {
            if (exist_boss ==1){
            createBoss();
            exist_boss--;
            }
        }
        if (LEVEL != 5){
        CreateEnemy();
        }
        doBullets();
        //cerr << "Target to Kill: " << targetToKill << endl;
        if (player.health == 0 && --timeToResetStage <= 0){
                reset();
        }

    }
    void drawExplosion(Graphics& graphics){
        while(explosion.currentFrame < FRAMES){
            explosion.tick();
            graphics.render(bulletCollidesX, bulletCollidesY, explosion);
        }
        explosion.currentFrame = 0;
    }
    void render(Graphics& graphics) // vẽ -> game
    {

        if (WinTheGame == true){
                LEVEL++;
                updateLevel(graphics, LEVEL);
                WinTheGame = false;
        }

        background.drawBackground(graphics.renderer); // vẽ nền

		for (Object* b: bullets)
            graphics.renderTexture(b->texture, b->x, b->y);

        for (Object* b: fighters)
            if (b->health > 0)
                graphics.renderTexture(b->texture, b->x, b->y);

        if (check_explosion > 0){
            drawExplosion(graphics);
            check_explosion--;
            graphics.play(explode);
        }

    }
    void Destroy(){
        if (gMusic != nullptr) Mix_FreeMusic( gMusic );
        if (explode != nullptr) Mix_FreeChunk( explode );

    }
};


#endif // _GAME__H
