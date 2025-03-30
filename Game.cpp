#include <SDL_image.h>
#include "Game.h"
#include <iostream>
#include "Constants.h"
#include <algorithm>

using namespace std;
SDL_Texture* background;

Game::Game() : player(((MAP_WIDTH -1) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE) {
    running = true;
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
            running = false;
        }
        window = SDL_CreateWindow("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                  SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!window) {
            cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
            running = false;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
            running = false;
        }
        SDL_Surface* tempSurface = IMG_Load("C:/Users/PC/Downloads/background.jpg");
        if (tempSurface) {
        background = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
        } else {
        cerr << "Failed to load background: " << IMG_GetError() << endl;
        background = nullptr;
    }
        generateWalls();
        spawnEnemies();
}

Game::~Game() {
        if (background) {
        SDL_DestroyTexture(background);
        }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

void Game::generateWalls() {
    for (int i = 3; i < MAP_HEIGHT - 3; i += 2) {
            for (int j = 3; j < MAP_WIDTH - 3; j += 2) {
                Wall w = Wall(j * TILE_SIZE, i * TILE_SIZE);
                walls.push_back(w);
            }
        }
}

void Game::handleEvents() {
    SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP: player.move(0, -5, walls); break;
                    case SDLK_DOWN: player.move(0, 5, walls); break;
                    case SDLK_LEFT: player.move(-5, 0, walls); break;
                    case SDLK_RIGHT: player.move(5, 0, walls); break;
                    case SDLK_SPACE: player.shoot(); break;
                }
            }
        }
}

void Game::spawnEnemies() {
    enemies.clear();
        for (int i = 0; i < enemyNumber; ++i) {
            int ex, ey;
            bool validPosition = false;
            while (!validPosition) {
                ex = (rand() % (MAP_WIDTH - 2) + 1) * TILE_SIZE;
                ey = (rand() % (MAP_HEIGHT - 2) + 1) * TILE_SIZE;
                validPosition = true;
                for (const auto& wall : walls) {
                    if (wall.active && wall.x == ex && wall.y == ey) {
                        validPosition = false;
                        break;
                    }
                }
            }
            enemies.push_back(EnemyTank(ex, ey));
        }
}

void Game::update() {
    player.updateBullets();

        for (auto& enemy : enemies) {
            enemy.move(walls);
            enemy.updateBullets();
            if (rand() % 100 < 2) {
                enemy.shoot();
            }
        }

        for (auto& enemy : enemies) {
            for (auto& bullet : enemy.bullets) {
                for (auto& wall : walls) {
                    if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                        wall.active = false;
                        bullet.active = false;
                        break;
                    }
                }
            }
        }

        for (auto& bullet : player.bullets) {
            for (auto& wall : walls) {
                if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                    wall.active = false;
                    bullet.active = false;
                    break;
                }
            }
        }
        for (auto& bullet : player.bullets) {
            for (auto& enemy : enemies) {
                if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                    enemy.active = false;
                    bullet.active = false;
                }
            }
        }
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                     [](EnemyTank &e) { return !e.active; }), enemies.end());
        if (enemies.empty()) {
            running = false;
        }
        for (auto& enemy : enemies) {
            for (auto& bullet : enemy.bullets) {
                if (SDL_HasIntersection(&bullet.rect, &player.rect)) {
                    running = false;
                    return;
                }
            }
        }
}

void Game::render() {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);

        if (background) {
        SDL_RenderCopy(renderer, background, NULL, NULL);
        }

        for (int i = 0; i < walls.size(); i++) {
            walls[i].render(renderer);
        }

        player.render(renderer);

        for (auto &enemy : enemies) {
            enemy.render(renderer);
        }

        SDL_RenderPresent(renderer);
}

void Game::run() {
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);
    }
}



