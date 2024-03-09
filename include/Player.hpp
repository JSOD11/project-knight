#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>

#include "Game.hpp"
#include "RenderWindow.hpp"
#include "Character.hpp"

const float vignetteStartingSize = 470;

class Player : public Character {
    private:
        Movement crouchMovement;
        Movement jumpMovement;
        Movement healMovement;
        Movement prayMovement;
        Movement airAttackMovement;
        Movement rollMovement;
        Movement attack2Movement;

        size_t healGems;
        float corruption;
        float maxCorruption;

        // TODO: Double jump movement.

        int invincibilityFrame;
    
    public:
        float vignetteSize;
        Player(size_t poise, Info info) : Character(poise, info) {
            this->healGems = 5;
            this->corruption = 0;
            this->maxCorruption = 100;
            this->invincibilityFrame = -1;
            this->vignetteSize = vignetteStartingSize;
        };
        void initializeMovementLoops(std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t, size_t>>& textures);
        void renderHUD(RenderWindow& window, std::vector<SDL_Texture*>& hudTextures);
        bool dealDamage(RenderWindow& window, bool airAttack);
        void renderPlayer(RenderWindow& window);

        void startAttack(size_t attackNumber);
        void takeDamage(size_t damage) override;

        bool canMove() override;
        bool isImmobile() override;

        Command crouch;
        Command jump;

        void startJump();
        bool isAirborne();
        void obeyGravity();

        Command heal;
        Command pray;
        Command airAttack;
        Command roll;
        Command attack2;

        void startRoll();

        bool canHeal();
};

const SDL_Rect knightHitbox = {
    .x = 114,
    .y = 34,
    .w = 32,
    .h = 92
};
const size_t knightAttackWidth = 80;
const SDL_Rect knightAttackBox = {
    .x = 200, // 108 + 84
    .y = knightHitbox.y,
    .w = knightAttackWidth,
    .h = knightHitbox.h
};

extern Player* knight;
void createPlayer(RenderWindow& window, int groundHeight, int posX);

#endif // PLAYER_H
