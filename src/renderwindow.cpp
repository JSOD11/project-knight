#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "RenderWindow.hpp"
#include "Block.hpp"

RenderWindow::RenderWindow(const char* title, int width, int height) 
    :window(nullptr), renderer(nullptr), width(width), height(height) {
    this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!this->window)
        std::cout << "\nRenderWindow(): " << SDL_GetError() << "\n" << std::endl;

    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

SDL_Window* RenderWindow::getWindow() {
    return this->window;
}

void RenderWindow::clearWindow() {
    SDL_RenderClear(this->renderer);
}

void RenderWindow::destroyWindow() {
    SDL_DestroyWindow(this->window);
}

void RenderWindow::renderStatic(const Block& Block) {
    SDL_Rect src;
    src.x = Block.getCurrentFrame().x;
    src.y = Block.getCurrentFrame().y;
    src.w = Block.getCurrentFrame().w;
    src.h = Block.getCurrentFrame().h;

    SDL_Rect dst;
    dst.x = Block.getPosition().x * 4;
    dst.y = Block.getPosition().y * 4;
    dst.w = Block.getCurrentFrame().w * 4;
    dst.h = Block.getCurrentFrame().h * 4;
    
    SDL_RenderCopy(this->renderer, Block.getTexture(), &src, &dst);
}

SDL_Texture* RenderWindow::loadTexture(const char* filePath) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath);
    if (!texture) std::cout << "\nloadTexture(): Failed to load texture.\n" << std::endl;

    return texture;
}

void RenderWindow::destroyTexture(SDL_Texture* texture) {
    SDL_DestroyTexture(texture);
}

void RenderWindow::display() {
    SDL_RenderPresent(this->renderer);
}

int RenderWindow::getRefreshRate() {
    int displayIndex = SDL_GetWindowDisplayIndex(this->window);
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(displayIndex, 0, &mode);
    return mode.refresh_rate;
}

int RenderWindow::getWidth() {
    return (int)this->width;
}

int RenderWindow::getHeight() {
    return (int)this->height;
}

SDL_Renderer* RenderWindow::getRenderer() {
    return this->renderer;
}
