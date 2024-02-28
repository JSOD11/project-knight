#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
#include <SDL_image.h>

#include "RenderWindow.hpp"

class Character {
    private:
        Vector2f position;
        SDL_Rect currentFrame;
        size_t width;
        size_t height;
        size_t loopSpeed;
        size_t movementSpeed;
        bool movingLeft;
        bool movingRight;

        // All frames are facing right, so right is the default.
        // If not facing right, then we flip the animations.
        bool facingRight;

        SDL_Texture* idleTexture;
        std::vector<SDL_Rect> idleFrames;
        size_t idleFrame;

        SDL_Texture* attackTexture;
        bool isAttacking;
        
        std::vector<SDL_Rect> attack0Frames;
        size_t attack0Frame;

        SDL_Texture* runningTexture;
        std::vector<SDL_Rect> runningFrames;
        size_t runningFrame;
    
    public:
        Character(Vector2f position, SDL_Rect startingFrame, size_t width, size_t height, size_t loopSpeed, size_t movementSpeed);
        void initializeMovementLoops(SDL_Texture* idleTexture, SDL_Texture* attackTexture, SDL_Texture* runningTexture);
        void renderCharacter(RenderWindow& window);

        bool currentlyAttacking();
        void resetAttack0Frames();
        void turnAttackingStatusOn();

        bool isMovingLeft();
        bool isMovingRight();
        void moveLeft(RenderWindow& window);
        void moveRight(RenderWindow& window);
        void stopMovingLeft();
        void stopMovingRight();
};

#endif // CHARACTER_H
