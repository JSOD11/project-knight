#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>

const int GRAVITY = -1;

struct Movement {
    SDL_Texture* texture;
    std::vector<SDL_Rect> frameVector;
    size_t frame;
    // Higher value for loopFrames = slower movement.
    size_t loopFrames;
    bool isActive;
};

struct Command {
    inline void start() {
        movement->isActive = true;
    }

    inline void stop() {
        movement->isActive = false;
        movement->frame = 0;
    }

    inline bool isActive() const {
        return movement->isActive;
    }

    Movement* movement;
};

#endif // GAME_H
