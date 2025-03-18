#include <iostream>
#include <SDL.h>
#include <SDL_main.h>
#include <vector>
#include <algorithm>


using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;
const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;

class Game{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    Game() {
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
        renderer = SDL_CreateRender(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
            running = false;
        }
    }
    void render() {
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int i = 1; i < MAP_HEIGHT - 1; ++i) {
            for (int j = 1; j < MAP_WIDTH - 1; ++ i) {
                SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                SDL_RenderFillRect(renderer, &tile);
            }
        }
        SDL_RenderPresent(renderer);
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
