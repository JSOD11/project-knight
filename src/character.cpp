#include <SDL.h>
#include <SDL_image.h>

#include "Character.hpp"

Character::Character(Vector2f position, SDL_Rect startingFrame, size_t width, size_t height, size_t loopSpeed, size_t movementSpeed)
    : position(position), currentFrame(startingFrame), width(width), height(height), loopSpeed(loopSpeed), movementSpeed(movementSpeed),
    idleTexture(nullptr), attackTexture(nullptr) {
    this->movingLeft = false;
    this->movingRight = false;
    this->facingRight = true;
}

void Character::initializeMovementLoops(SDL_Texture* idleTexture, SDL_Texture* attackTexture, SDL_Texture* runningTexture) {    

    // Load idle texture and create movement loop.
    this->idleTexture = idleTexture;
    this->idleFrame = 0;
    if (!idleTexture)
        std::cout << "Failed to load knight texture.\n";
    for (size_t i = 0; i < 8; i++) {
        SDL_Rect frame;
        frame.x = (i % 2) * width;
        frame.y = (i / 2) * height;
        frame.w = width;
        frame.h = height;
        this->idleFrames.push_back(frame);
    }

    // Load attack texture.
    this->attackTexture = attackTexture;
    if (!attackTexture)
        std::cout << "Failed to load attack texture.\n";

    // Create attack0 movement loop.
    this->attack0Frame = 0;
    for (size_t i = 0; i < 7; i++) {
        SDL_Rect frame;
        frame.x = i * width;
        frame.y = 0;
        frame.w = width;
        frame.h = height;
        this->attack0Frames.push_back(frame);
    }

    // Load running texture.
    this->runningTexture = runningTexture;
    if (!runningTexture)
        std::cout << "Failed to load running texture.\n";
    // Create running movement loop.
    this->runningFrame = 0;
    for (size_t i = 0; i < 8; i++) {
        SDL_Rect frame;
        frame.x = (i % 2) * width;
        frame.y = (i / 2) * height;
        frame.w = width;
        frame.h = height;
        this->runningFrames.push_back(frame);
    }
}


// `renderCharacter()`
// Renders a dynamic character to the screen.
void Character::renderCharacter(RenderWindow& window) {
    SDL_RendererFlip flipType = this->facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    if (this->isAttacking) {
        SDL_RenderCopyEx(window.getRenderer(), this->attackTexture, &this->attack0Frames[this->attack0Frame / this->loopSpeed], &this->currentFrame, 0, nullptr, flipType);
        if (this->attack0Frame == this->attack0Frames.size() * this->loopSpeed - 1) {
            this->isAttacking = false;
            this->resetAttack0Frames();
        } else {
            this->attack0Frame++;
        }
    } else if ((this->isMovingLeft() || this->isMovingRight()) && !(this->isMovingLeft() && this->isMovingRight())) {
        SDL_RenderCopyEx(window.getRenderer(), this->runningTexture, &this->runningFrames[this->runningFrame / this->loopSpeed], &this->currentFrame, 0, nullptr, flipType);
        if (this->runningFrame == this->runningFrames.size() * this->loopSpeed - 1) this->runningFrame = 0;
        else this->runningFrame++;
    } else {
        SDL_RenderCopyEx(window.getRenderer(), this->idleTexture, &this->idleFrames[this->idleFrame / this->loopSpeed], &this->currentFrame, 0, nullptr, flipType);
        if (this->idleFrame == this->idleFrames.size() * this->loopSpeed - 1) this->idleFrame = 0;
        else this->idleFrame++;
    }
}

bool Character::currentlyAttacking() {
    return this->isAttacking;
}

void Character::resetAttack0Frames() {
    this->attack0Frame = 0;
}

void Character::turnAttackingStatusOn() {
    this->isAttacking = true;
}

bool Character::isMovingLeft() {
    return this->movingLeft;
}

bool Character::isMovingRight() {
    return this->movingRight;
}

void Character::moveLeft(RenderWindow& window) {
    (void) window;
    this->movingLeft = true;
    this->facingRight = false;
    size_t movementDelta = this->movementSpeed;
    if (this->currentlyAttacking() || this->isMovingRight()) movementDelta = 0;
    if (-100 < this->position.x) {
        this->position.x -= movementDelta;
        this->currentFrame.x -= movementDelta;
    }
}

void Character::moveRight(RenderWindow& window) {
    this->movingRight = true;
    this->facingRight = true;
    size_t movementDelta = this->movementSpeed;
    if (this->currentlyAttacking() || this->isMovingLeft()) movementDelta = 0;
    if (this->position.x < window.getWidth() - 150) {
        this->position.x += movementDelta;
        this->currentFrame.x += movementDelta;
    }
}

void Character::stopMovingLeft() {
    this->movingLeft = false;
}

void Character::stopMovingRight() {
    this->movingRight = false;
}
