#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <SDL_image.h>
#include <map>

#include "Game.hpp"
#include "RenderWindow.hpp"
#include "Character.hpp"

class Enemy : public Character {
    protected:
        bool died;
        bool bodyDamage;
        int sightDistance;

        Movement deathEffectMovement;
        Movement birthEffectMovement;
    
    public:
        Enemy(bool bodyDamage, int sightDistance, size_t poise, Info info) : Character(poise, info), bodyDamage(bodyDamage), sightDistance(sightDistance) {
            died = false;
        };

        Command deathEffect;
        Command birthEffect;
        void initializeMovementLoops(std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t, size_t>>& textures);
        void setSightDistance(int sightDistance);
        bool dealDamage();
        void renderEnemy(RenderWindow& window);
        bool isDead();
        bool dealsBodyDamage();
};

extern std::map<Enemy*, bool> enemies;

#endif // ENEMY_H
