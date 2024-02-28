#include <SDL.h>
#include <SDL_image.h>

#include "Slime.hpp"

Slime::Slime(Vector2f position, SDL_Rect startingFrame, size_t width, size_t height, size_t loopSpeed, size_t movementSpeed)
    : position(position), currentFrame(startingFrame), width(width), height(height), loopSpeed(loopSpeed), movementSpeed(movementSpeed),
    idleTexture(nullptr), attackTexture(nullptr) {
    this->movingLeft = false;
    this->movingRight = false;
    this->facingRight = true;
    this->isAttacking = false;
}

void Slime::initializeMovementLoops(SDL_Texture* idleTexture, SDL_Texture* attackTexture, SDL_Texture* runningTexture) {    

    // Load idle texture and create movement loop.
    this->idleTexture = idleTexture;
    this->idleFrame = 0;
    if (!idleTexture)
        std::cout << "Failed to load idle texture.\n";
    for (size_t i = 0; i < 4; i++) {
        SDL_Rect frame;
        frame.x = i * this->width;
        frame.y = 0;
        frame.w = width;
        frame.h = height;
        this->idleFrames.push_back(frame);
    }

    for (int i = 2; i >= 0; i--) {
        SDL_Rect frame;
        frame.x = i * this->width;
        frame.y = 0;
        frame.w = width;
        frame.h = height;
        this->idleFrames.push_back(frame);
    }

    // Load attack texture.
    this->attackTexture = attackTexture;
    // if (!attackTexture)
    //     std::cout << "Failed to load attack texture.\n";

    // Create attack movement loop.
    // this->attackFrame = 0;
    // for (size_t i = 0; i < 7; i++) {
    //     SDL_Rect frame;
    //     frame.x = i * width;
    //     frame.y = 0;
    //     frame.w = width;
    //     frame.h = height;
    //     this->attackFrames.push_back(frame);
    // }

    // Load running texture.
    this->runningTexture = runningTexture;
    // if (!runningTexture)
    //     std::cout << "Failed to load running texture.\n";
    // // Create running movement loop.
    // this->runningFrame = 0;
    // for (size_t i = 0; i < 8; i++) {
    //     SDL_Rect frame;
    //     frame.x = (i % 2) * width;
    //     frame.y = (i / 2) * height;
    //     frame.w = width;
    //     frame.h = height;
    //     this->runningFrames.push_back(frame);
    // }
}


// `renderSlime()`
// Renders a dynamic Slime to the screen.
void Slime::renderSlime(RenderWindow& window) {
    SDL_RendererFlip flipType = this->facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    if (this->isAttacking) {
        SDL_RenderCopyEx(window.getRenderer(), this->attackTexture, &this->attackFrames[this->attackFrame / this->loopSpeed], &this->currentFrame, 0, nullptr, flipType);
        if (this->attackFrame == this->attackFrames.size() * this->loopSpeed - 1) {
            this->isAttacking = false;
            this->resetAttackFrames();
        } else {
            this->attackFrame++;
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

bool Slime::currentlyAttacking() {
    return this->isAttacking;
}

void Slime::resetAttackFrames() {
    this->attackFrame = 0;
}

void Slime::turnAttackingStatusOn() {
    this->isAttacking = true;
}

bool Slime::isMovingLeft() {
    return this->movingLeft;
}

bool Slime::isMovingRight() {
    return this->movingRight;
}

void Slime::moveLeft(RenderWindow& window) {
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

void Slime::moveRight(RenderWindow& window) {
    this->movingRight = true;
    this->facingRight = true;
    size_t movementDelta = this->movementSpeed;
    if (this->currentlyAttacking() || this->isMovingLeft()) movementDelta = 0;
    if (this->position.x < window.getWidth() - 150) {
        this->position.x += movementDelta;
        this->currentFrame.x += movementDelta;
    }
}

void Slime::stopMovingLeft() {
    this->movingLeft = false;
}

void Slime::stopMovingRight() {
    this->movingRight = false;
}
