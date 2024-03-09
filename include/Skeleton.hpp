#ifndef SKELETON_H
#define SKELETON_H

#include <SDL.h>

#include "RenderWindow.hpp"
#include "Enemy.hpp"

const SDL_Rect skeletonHitbox = {
    .x = 65,
    .y = 105,
    .w = 40,
    .h = 85
};
const size_t skeletonAttackWidth = 80;
const SDL_Rect skeletonAttackBox = {
    .x = 174,
    .y = skeletonHitbox.y,
    .w = skeletonAttackWidth,
    .h = skeletonHitbox.h
};

class Skeleton : public Enemy {
    private:
    
    public:
        Skeleton(bool bodyDamage, int sightDistance, size_t poise, Info info) 
            : Enemy(bodyDamage, sightDistance, poise, info) {};
};

void createSkeleton(RenderWindow& window, int groundHeight, int posX);

#endif // SKELETON_H
