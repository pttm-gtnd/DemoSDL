#include "Menu.h"
#include "Constants.h"
#include <iostream>

using namespace std;

Menu::Menu(SDL_Renderer* renderer) {
    this->renderer = renderer;

    SDL_Surface* tempSurface = IMG_Load("start_menu.png");
    if (!tempSurface) {
        cout << "Lỗi: Không thể tải ảnh start_menu.png! " << IMG_GetError() << endl;
    }

    background = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (!background) {
        cout << "Lỗi: Không thể tạo texture từ ảnh! " << SDL_GetError() << endl;
    }
}

Menu::~Menu() {
    SDL_DestroyTexture(background);
}

bool Menu::show() {
    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) { // Nhấn Enter để bắt đầu game
                    return true;
                } else if (event.key.keysym.sym == SDLK_ESCAPE) { // Nhấn ESC để thoát game
                    return false;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (background) {
            SDL_Rect destRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, background, NULL, &destRect);
        } else {
            cout << "Cảnh báo: Background texture chưa được load!" << endl;
        }

        SDL_RenderPresent(renderer);
    }

    return false;
}

