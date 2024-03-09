#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
#include <SDL_image.h>

#include "RenderWindow.hpp"

struct Info {
    int health;
    int maxHealth;
    size_t attackDamage;
    float sizeScaling;
    Vector2i position;
    SDL_Rect currentFrame;
    Vector2i pngSize; // (width, height)
    SDL_Rect hitbox;
    int hitboxX;
    SDL_Rect attackBox;
    Vector2i centerCoordinates; // (centerX, centerY)
    size_t movementSpeed;
    size_t jumpForce;
    int groundHeight;

    bool facingRight;
    bool movingRight;
    bool movingLeft;
    int velocityY;
};

struct Movement {
    SDL_Texture* texture;
    std::vector<SDL_Rect> frameVector;
    size_t frame;
    // Higher value for loopFrames = slower movement.
    bool disabled;
    size_t loopFrames;
    size_t executionFrame;
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

    inline void enable() {
        movement->disabled = false;
    }

    inline void disable() {
        movement->disabled = true;
    }

    inline bool isDisabled() {
        return movement->disabled;
    }

    Movement* movement;
};

class Character {
    protected:

        // TODO: Take these movements and put them into a list.
        // RenderCharacter processes each movement one by one in
        // order of priority.

        size_t poise;

        Movement idleMovement;
        Movement attackMovement;
        Movement runMovement;
        Movement hurtMovement;
        Movement deathMovement;
    
    public:
        Info info;

        Character(size_t poise, Info info) : poise(poise), info(info) {};
        void initializeTextureLoop(Command& command, Movement& movement, std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t, size_t>& texture, Vector2i pngSize);
        void renderTexture(RenderWindow& window, Movement& movement, SDL_RendererFlip& flipType, bool hitbox);

        Command idle;
        Command attack;
        Command run;
        Command hurt;
        Command death;
        virtual void takeDamage(size_t damage);

        virtual bool canMove();
        virtual bool canChangeDirection();
        virtual bool isImmobile();
        virtual void move(RenderWindow& window, float movementSpeed);
        virtual void startMovingRight();
        virtual void startMovingLeft();
        virtual void changeCoordinates(bool x, int delta);
        virtual void setCoordinate(bool x, int coordinate);
        virtual bool isMovingRight();
        virtual bool isMovingLeft();
        virtual void stopMovingRight();
        virtual void stopMovingLeft();
        virtual SDL_Rect buildAttackBox();

        virtual void renderBox(RenderWindow& window, SDL_Rect& box);
};

Info initializeInfo(int health, size_t attackDamage, float sizeScaling, Vector2i pngSize, SDL_Rect hitbox, SDL_Rect attackBox, size_t movementSpeed, size_t jumpForce, int groundHeight, size_t posX);

#endif // CHARACTER_H
