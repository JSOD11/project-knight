#include <SDL.h>
#include <SDL_image.h>

#include "Character.hpp"
#include "Game.hpp"

Character::Character(Vector2f position, SDL_Rect startingFrame, size_t width, size_t height, float movementSpeed, float jumpForce, int groundHeight)
    : position(position), currentFrame(startingFrame), width(width), height(height), movementSpeed(movementSpeed), jumpForce(jumpForce), groundHeight(groundHeight) {
    this->movingLeft = false;
    this->movingRight = false;
    this->facingRight = true;
    this->velocityY = 0;
}


void initializeTextureLoop(Command& command, Movement& movement, std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t>& texture, size_t width, size_t height) {

    movement.texture = std::get<0>(texture);
    if (!movement.texture) std::cout << "Failed to load texture.\n";

    size_t start = std::get<1>(texture);
    size_t N = std::get<2>(texture);
    size_t rowLength = std::get<3>(texture);
    movement.frame = 0;
    movement.loopFrames = std::get<4>(texture);
    movement.isActive = false;

    command.movement = &movement;

    // N is the number of images in the loop. rowLength is the length of a row in the png.
    for (size_t i = start; i < N; i++) {
        SDL_Rect frame;
        frame.x = (i % rowLength) * width;
        frame.y = (i / rowLength) * height;
        frame.w = width;
        frame.h = height;
        movement.frameVector.push_back(frame);
    }
}

void Character::initializeMovementLoops(std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t>>& textures) {
    initializeTextureLoop(this->idle, this->idleMovement, textures[0], this->width, this->height);
    if (textures.size() > 1) initializeTextureLoop(this->attack, this->attackMovement, textures[1], this->width, this->height);
    if (textures.size() > 2) initializeTextureLoop(this->run, this->runMovement, textures[2], this->width, this->height);
    if (textures.size() > 3) initializeTextureLoop(this->crouch, this->crouchMovement, textures[3], this->width, this->height);
    if (textures.size() > 4) initializeTextureLoop(this->jump, this->jumpMovement, textures[4], this->width, this->height);
    if (textures.size() > 5) initializeTextureLoop(this->heal, this->healMovement, textures[5], this->width, this->height);
    if (textures.size() > 6) initializeTextureLoop(this->pray, this->prayMovement, textures[6], this->width, this->height);
    if (textures.size() > 7) initializeTextureLoop(this->airAttack, this->airAttackMovement, textures[7], this->width, this->height);
}

void Character::renderTexture(RenderWindow& window, Movement& movement, SDL_RendererFlip& flipType) {
    SDL_RenderCopyEx(window.getRenderer(), movement.texture, &movement.frameVector[movement.frame / movement.loopFrames], &this->currentFrame, 0, nullptr, flipType);
    if (movement.frame >= movement.frameVector.size() * movement.loopFrames - 1) movement.frame = 0;
    else movement.frame++;
}

// `renderCharacter()`
// Renders a dynamic character to the screen.
void Character::renderCharacter(RenderWindow& window) {

    this->move(window);
    this->obeyGravity();

    // std::cout << this->position.x << ", " << this->position.y << std::endl;
    // std::cout << this->velocityY << std::endl;

    SDL_RendererFlip flipType = this->facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    if (this->attack.isActive()) {
        this->renderTexture(window, this->attackMovement, flipType);
        if (this->attackMovement.frame == 0) this->attack.stop();
    } else if (this->isAirborne()) {
        if (this->airAttack.isActive()) {
            this->renderTexture(window, this->airAttackMovement, flipType);
            if (this->airAttackMovement.frame == 0) this->airAttack.stop();
        } else {
            int frame = -this->velocityY / 4 + 4;
            if (frame < 0) frame = 0;
            if (frame > 7) frame = 7;
            SDL_RenderCopyEx(window.getRenderer(), this->jumpMovement.texture, &this->jumpMovement.frameVector[frame], &this->currentFrame, 0, nullptr, flipType);
        }
    } else if (this->heal.isActive()) {
        this->renderTexture(window, this->healMovement, flipType);
        if (this->healMovement.frame == 0) this->healMovement.isActive = false;
    } else if ((this->pray.isActive())) {
        this->renderTexture(window, this->prayMovement, flipType);
    } else if (this->crouch.isActive()) {
        this->renderTexture(window, this->crouchMovement, flipType);
    } else if ((this->isMovingLeft() || this->isMovingRight()) && !(this->isMovingLeft() && this->isMovingRight())) {
        this->renderTexture(window, this->runMovement, flipType);
    } else {
        this->renderTexture(window, this->idleMovement, flipType);
    }
}

void Character::startAttack() {
    this->heal.stop();
    this->attackMovement.isActive = true;
}

void Character::startMovingRight() {
    this->movingRight = true;
}

void Character::startMovingLeft() {
    this->movingLeft = true;
}

void Character::move(RenderWindow& window) {
    if (this->isMovingRight()) {
        if (!this->attack.isActive()) this->facingRight = true;
    } else if (this->isMovingLeft()) {
        this->movingLeft = true;
        if (!this->attack.isActive()) this->facingRight = false;
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

bool Character::isMovingRight() {
    return this->movingRight;
}

bool Character::isMovingLeft() {
    return this->movingLeft;
}

void Character::stopMovingRight() {
    this->movingRight = false;
}

void Character::stopMovingLeft() {
    this->movingLeft = false;
}


void Character::startJump() {
    if (!this->isAirborne()) {
        this->heal.stop();

        this->jumpMovement.isActive = true;
        this->velocityY = this->jumpForce;
    }
}

bool Character::isAirborne() {
    return this->jumpMovement.isActive;
}

void Character::obeyGravity() {
    if (this->position.y >= this->groundHeight && this->velocityY <= 0) {
        this->jumpMovement.isActive = false;
        this->position.y = groundHeight;
        this->velocityY = 0;
    } else if (this->velocityY > 0 || this->position.y < this->groundHeight) {
        this->position.y -= this->velocityY;
        this->velocityY += GRAVITY;
    }
    this->currentFrame.y = this->position.y;
}

bool Character::isImmobile() {
    return this->attackMovement.isActive || this->healMovement.isActive || this->crouchMovement.isActive || this->prayMovement.isActive;
}
