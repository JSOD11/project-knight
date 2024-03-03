#include "Game.hpp"

bool collision(SDL_Rect* x, SDL_Rect* y) {
    if (SDL_HasIntersection(x, y) == SDL_TRUE) {
        return true;
    }
    return false;
}
