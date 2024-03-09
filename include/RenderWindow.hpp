#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <SDL.h>
#include <SDL_image.h>

#include "Block.hpp"

class RenderWindow {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        size_t width;
        size_t height;

    public:
        RenderWindow(const char* title, int width, int height);
        SDL_Window* getWindow();
        void clearWindow();
        void destroyWindow();
        void renderStatic(const Block& Block);
        void display();
        SDL_Texture* loadTexture(const char* filePath);
        void destroyTexture(SDL_Texture* texture);
        int getRefreshRate();
        int getWidth();
        int getHeight();
        SDL_Renderer* getRenderer();
};

#endif // RENDER_WINDOW_H
