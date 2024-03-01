#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
#include <SDL_image.h>

#include "RenderWindow.hpp"
#include "Game.hpp"

class Character {
    private:
        Vector2f position;
        SDL_Rect currentFrame;
        size_t width;
        size_t height;
        float movementSpeed;
        bool movingLeft;
        bool movingRight;

        // All frames are facing right, so right is the default.
        // If not facing right, then we flip the animations.
        bool facingRight;

        float velocityY;
        float jumpForce;
        int groundHeight;

        Movement idleMovement;
        Movement attackMovement;
        Movement runMovement;
        Movement crouchMovement;
        Movement jumpMovement;
        Movement healMovement;
        Movement prayMovement;
        Movement airAttackMovement;
    
    public:
        Character(Vector2f position, SDL_Rect startingFrame, size_t width, size_t height, float movementSpeed, float jumpForce, int groundHeight);
        void initializeMovementLoops(std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t>>& textures);
        void renderCharacter(RenderWindow& window);
        void renderTexture(RenderWindow& window, Movement& movement, SDL_RendererFlip& flipType);

        Command idle;
        Command attack;
        void startAttack();
        Command run;

        void startMovingRight();
        void startMovingLeft();
        void move(RenderWindow& window);
        bool isMovingRight();
        bool isMovingLeft();
        void stopMovingRight();
        void stopMovingLeft();

        Command crouch;
        Command jump;

        void startJump();
        bool isAirborne();
        void obeyGravity();

        Command heal;
        Command pray;
        Command airAttack;

        bool isImmobile();
};

#endif // CHARACTER_H
