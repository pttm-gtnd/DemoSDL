#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_image.h>

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();
    bool show();

private:
    SDL_Renderer* renderer;
    SDL_Texture* background;
};

#endif

