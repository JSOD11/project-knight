#ifndef SLIME_H
#define SLIME_H

#include <SDL.h>

#include "RenderWindow.hpp"

const SDL_Rect slimeHitbox = {
    .x = 38,
    .y = 96,
    .w = 46,
    .h = 31
};
const size_t slimeAttackWidth = 72;
const SDL_Rect slimeAttackBox = {
    .x = 102, // 38 + 46 + a little more
    .y = slimeHitbox.y,
    .w = slimeAttackWidth,
    .h = slimeHitbox.h
};

class Slime : public Enemy {
    private:
    
    public:
        Slime(bool bodyDamage, int sightDistance, size_t poise, Info info) 
            : Enemy(bodyDamage, sightDistance, poise, info) {};
};

void createSlime(RenderWindow& window, std::string color, int groundHeight, int posX);

#endif // SLIME_H
