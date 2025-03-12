#include <iostream>
#include <SDL.h>
#include <SDL_main.h>
#include <vector>
#include <algorithm>


using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Battle City";
const int TILE_SIZE = 40;

void logErrorAndExit(const char* msg, const char* error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}

SDL_Window* initSDL(int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* WINDOW_TITLE)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

    return window;
}

SDL_Renderer* createRenderer(SDL_Window* window)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    return renderer;
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

class Bullet {
public:
    SDL_Rect rect;
    int speed;
    bool active;

    Bullet(int x, int y) {
        rect = {x, y, 5, 10};
        speed = 6;
        active = true;
    }

    void move() {
        rect.y -= speed;
        if (rect.y < 0) active = false;
    }

    void draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
};

class Tank {
public:
    SDL_Rect rect;
    float speed;
    vector<Bullet> bullets;

    Tank(int x, int y) {
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        speed = 4.0f;
    }

    void move(int dx, int dy) {
        rect.x += dx * speed;
        rect.y += dy * speed;
    }

    void shoot() {
        bullets.push_back(Bullet(rect.x + TILE_SIZE / 2 - 2, rect.y));
    }

    void updateBullets() {
        for (auto& bullet : bullets) {
            bullet.move();
        }
        bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return !b.active; }), bullets.end());
    }

    void draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        for (auto& bullet : bullets) {
            bullet.draw(renderer);
        }
    }
};

int main(int argc, char* argv[])

{
    SDL_Window* window = initSDL(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SDL_Renderer* renderer = createRenderer(window);

    Tank player(SCREEN_WIDTH / 2, SCREEN_HEIGHT - TILE_SIZE * 2);
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        int dx = 0, dy = 0;
        if (keystate[SDL_SCANCODE_W]) dy = -1;
        if (keystate[SDL_SCANCODE_S]) dy = 1;
        if (keystate[SDL_SCANCODE_A]) dx = -1;
        if (keystate[SDL_SCANCODE_D]) dx = 1;
        player.move(dx, dy);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        player.draw(renderer);
        SDL_RenderPresent(renderer);
    }

    quitSDL(window, renderer);
    return 0;
}
