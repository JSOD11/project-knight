#include <SDL.h>
#include <SDL_image.h>

#include "Character.hpp"
#include "Game.hpp"

Character::Character(Vector2f position, SDL_Rect startingFrame, size_t width, size_t height, size_t loopSpeed, size_t movementSpeed, float jumpForce, size_t groundHeight)
    : position(position), currentFrame(startingFrame), width(width), height(height), loopSpeed(loopSpeed), movementSpeed(movementSpeed), jumpForce(jumpForce), groundHeight(groundHeight) {
    this->movingLeft = false;
    this->movingRight = false;
    this->facingRight = true;
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

void Character::initializeMovementLoops(std::vector<SDL_Texture*>& textures) {
    initializeTextureLoop(this->idle, textures[0], 8, 2, this->width, this->height);
    initializeTextureLoop(this->attack0, textures[1], 7, 8, this->width, this->height);
    initializeTextureLoop(this->run, textures[2], 8, 2, this->width, this->height);
    initializeTextureLoop(this->crouch, textures[3], 8, 2, this->width, this->height);
    initializeTextureLoop(this->jump, textures[4], 8, 2, this->width, this->height);
    initializeTextureLoop(this->heal, textures[5], 8, 2, this->width, this->height);
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
    // std::cout << this->velocityY << std::endl;

    SDL_RendererFlip flipType = this->facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    if (this->attack0.isActive) {
        this->renderTexture(window, this->attack0.texture, this->attack0.frameVector, flipType, this->loopSpeed, this->attack0.frame);
        if (this->attack0.frame == 0) this->attack0.isActive = false;
    } else if (this->isAirborne()) {
        int frame = -this->velocityY / 5 + 4;
        if (frame < 0) frame = 0;
        if (frame > 7) frame = 7;
        SDL_RenderCopyEx(window.getRenderer(), this->jump.texture, &this->jump.frameVector[frame], &this->currentFrame, 0, nullptr, flipType);
    } else if (this->isHealing()) {
        this->renderTexture(window, this->heal.texture, this->heal.frameVector, flipType, this->loopSpeed, this->heal.frame);
        if (this->heal.frame == 0) this->heal.isActive = false;
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
    if ((this->isImmobile() && !this->isAirborne()) || (this->isMovingRight() && this->isMovingLeft())) movementDelta = 0;
    
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

void Character::startJump() {
    if (!this->isAirborne()) {
        this->jump.isActive = true;
        this->velocityY = this->jumpForce;
    }
}

bool Character::isAirborne() {
    return this->jump.isActive;
}

void Character::obeyGravity() {
    if (this->position.y >= this->groundHeight && this->velocityY <= 0) {
        this->jump.isActive = false;
        this->position.y = groundHeight;
        this->velocityY = 0;
    } else if (this->velocityY > 0 || this->position.y < this->groundHeight) {
        this->position.y -= this->velocityY;
        this->velocityY += GRAVITY;
    }
    this->currentFrame.y = this->position.y;
}

void Character::startHealing() {
    this->heal.isActive = true;
}

bool Character::isHealing() {
    return this->heal.isActive;
}

bool Character::isImmobile() {
    return this->attack0.isActive || this->heal.isActive || this->crouch.isActive;
}
