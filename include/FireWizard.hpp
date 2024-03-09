#ifndef FIREWIZARD_H
#define FIREWIZARD_H

#include <SDL.h>

#include "RenderWindow.hpp"
#include "Enemy.hpp"

const SDL_Rect FireWizardHitbox = {
    .x = 54,
    .y = 93,
    .w = 30,
    .h = 97
};
const size_t FireWizardAttackWidth = 100;
const SDL_Rect FireWizardAttackBox = {
    .x = 189,
    .y = FireWizardHitbox.y,
    .w = FireWizardAttackWidth,
    .h = FireWizardHitbox.h
};

class FireWizard : public Enemy {
    private:
    
    public:
        FireWizard(bool bodyDamage, int sightDistance, size_t poise, Info info) 
            : Enemy(bodyDamage, sightDistance, poise, info) {};
};

void createFireWizard(RenderWindow& window, int groundHeight, int posX);

#endif // FIREWIZARD_H
