#include <SDL.h>
#include <SDL_image.h>

#include "Character.hpp"
#include "Game.hpp"

Character::Character(Vector2f position, SDL_Rect startingFrame, size_t width, size_t height, size_t loopSpeed, size_t movementSpeed, float jumpForce, size_t groundHeight)
    : position(position), currentFrame(startingFrame), width(width), height(height), loopSpeed(loopSpeed), movementSpeed(movementSpeed), jumpForce(jumpForce), groundHeight(groundHeight) {
    this->movingLeft = false;
    this->movingRight = false;
    this->facingRight = true;
    this->airborne = false;
    this->velocityY = 0;
}


void initializeTextureLoop(Movement& movement, SDL_Texture*& tex, size_t N, size_t L, size_t width, size_t height) {

    if (!tex) std::cout << "Failed to load texture.\n";

    movement.texture = tex;
    movement.frame = 0;
    movement.isActive = false;

    // N is the number of images in the loop. L is the length of a row in the png.
    for (size_t i = 0; i < N; i++) {
        SDL_Rect frame;
        frame.x = (i % L) * width;
        frame.y = (i / L) * height;
        frame.w = width;
        frame.h = height;
        movement.frameVector.push_back(frame);
    }
}

void Character::initializeMovementLoops(SDL_Texture* idleTexture, SDL_Texture* attackTexture, SDL_Texture* runningTexture, SDL_Texture* crouchTexture) {
    initializeTextureLoop(this->idle, idleTexture, 8, 2, this->width, this->height);
    initializeTextureLoop(this->attack0, attackTexture, 7, 8, this->width, this->height);
    initializeTextureLoop(this->run, runningTexture, 8, 2, this->width, this->height);
    initializeTextureLoop(this->crouch, crouchTexture, 8, 2, this->width, this->height);
}

void Character::renderTexture(RenderWindow& window, SDL_Texture* texture, std::vector<SDL_Rect>& frames, SDL_RendererFlip& flipType, size_t loopSpeed, size_t& frame) {
    SDL_RenderCopyEx(window.getRenderer(), texture, &frames[frame / loopSpeed], &this->currentFrame, 0, nullptr, flipType);
    if (frame == frames.size() * loopSpeed - 1) frame = 0;
    else frame++;
}

// `renderCharacter()`
// Renders a dynamic character to the screen.
void Character::renderCharacter(RenderWindow& window) {

    this->obeyGravity();

    // std::cout << this->position.x << ", " << this->position.y << std::endl;

    SDL_RendererFlip flipType = this->facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    if (this->attack0.isActive) {
        this->renderTexture(window, this->attack0.texture, this->attack0.frameVector, flipType, this->loopSpeed, this->attack0.frame);
        if (this->attack0.frame == 0) this->attack0.isActive = false;
    } else if (this->isCrouching()) {
        this->renderTexture(window, this->crouch.texture, this->crouch.frameVector, flipType, this->loopSpeed, this->crouch.frame);
    } else if ((this->isMovingLeft() || this->isMovingRight()) && !(this->isMovingLeft() && this->isMovingRight())) {
        this->renderTexture(window, this->run.texture, this->run.frameVector, flipType, 2, this->run.frame);
    }  else {
        this->renderTexture(window, this->idle.texture, this->idle.frameVector, flipType, this->loopSpeed, this->idle.frame);
    }
}

bool Character::currentlyAttacking() {
    return this->attack0.isActive;
}

void Character::resetAttack0Frames() {
    this->attack0.frame = 0;
}

void Character::turnAttackingStatusOn() {
    this->attack0.isActive = true;
}

bool Character::isMovingRight() {
    return this->movingRight;
}

bool Character::isMovingLeft() {
    return this->movingLeft;
}

void Character::move(RenderWindow& window, bool moveRight) {
    if (moveRight) {
        this->movingRight = true;
        if (!this->currentlyAttacking()) this->facingRight = true;
    } else {
        this->movingLeft = true;
        if (!this->currentlyAttacking()) this->facingRight = false;
    }

    float movementDelta = this->movementSpeed;
    if ((this->currentlyAttacking() && !this->isAirborne()) || this->isCrouching() || (this->isMovingRight() && this->isMovingLeft())) movementDelta = 0;
    
    if (this->isMovingLeft() && -100 < this->position.x) {
        this->position.x -= movementDelta;
        this->currentFrame.x -= movementDelta;
    }

    if (this->isMovingRight() && this->position.x < window.getWidth() - 150) {
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

void Character::startCrouching() {
    this->crouch.isActive = true;
}

void Character::stopCrouching() {
    this->crouch.isActive = false;
}

bool Character::isCrouching() {
    return this->crouch.isActive;
}

void Character::jump() {
    if (!this->isAirborne()) {
        this->airborne = true;
        this->velocityY = this->jumpForce;
    }
}

bool Character::isAirborne() {
    return this->airborne;
}

void Character::obeyGravity() {
    if (this->position.y >= this->groundHeight && this->velocityY <= 0) {
        this->airborne = false;
        this->position.y = groundHeight;
        this->velocityY = 0;
    } else if (this->velocityY > 0 || this->position.y < this->groundHeight) {
        this->position.y -= this->velocityY;
        this->velocityY += GRAVITY;
    }
    this->currentFrame.y = this->position.y;
}
