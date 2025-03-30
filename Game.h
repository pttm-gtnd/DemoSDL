#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include "PlayerTank.h"
#include "EnemyTank.h"
#include "Wall.h"

using namespace std;

class Game {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* background;
    bool running;
    vector<Wall> walls;
    PlayerTank player;
    int enemyNumber = 5;
    vector<EnemyTank> enemies;

    Game();
    ~Game();
    void generateWalls();
    void handleEvents();
    void spawnEnemies();
    void update();
    void render();
    void run();
};

#endif

