#include <SDL.h>
#include <SDL_image.h>

#include "Character.hpp"
#include "Game.hpp"

Character::Character(Info info) : info(info) {

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
    initializeTextureLoop(this->idle, this->idleMovement, textures[0], this->info.pngWidth, this->info.pngHeight);
    if (textures.size() > 1) initializeTextureLoop(this->attack, this->attackMovement, textures[1], this->info.pngWidth, this->info.pngHeight);
    if (textures.size() > 2) initializeTextureLoop(this->run, this->runMovement, textures[2], this->info.pngWidth, this->info.pngHeight);
    if (textures.size() > 3) initializeTextureLoop(this->hurt, this->hurtMovement, textures[3], this->info.pngWidth, this->info.pngHeight);
    if (textures.size() > 4) initializeTextureLoop(this->crouch, this->crouchMovement, textures[4], this->info.pngWidth, this->info.pngHeight);
    if (textures.size() > 5) initializeTextureLoop(this->jump, this->jumpMovement, textures[5], this->info.pngWidth, this->info.pngHeight);
    if (textures.size() > 6) initializeTextureLoop(this->heal, this->healMovement, textures[6], this->info.pngWidth, this->info.pngHeight);
    if (textures.size() > 7) initializeTextureLoop(this->pray, this->prayMovement, textures[7], this->info.pngWidth, this->info.pngHeight);
    if (textures.size() > 8) initializeTextureLoop(this->airAttack, this->airAttackMovement, textures[8], this->info.pngWidth, this->info.pngHeight);
}

void Character::dealDamage() {

    SDL_Rect hitbox;

    hitbox.x = enemies[0]->info.currentFrame.x + 36;
    hitbox.y = enemies[0]->info.currentFrame.y + 96;
    hitbox.w = 42;
    hitbox.h = 28;

    size_t attackWidth = 40;

    SDL_Rect attackFrame;
    if (this->info.facingRight) attackFrame.x = this->info.currentFrame.x + 96 * 2 - attackWidth;
    else attackFrame.x = this->info.currentFrame.x + 2 * this->info.pngWidth - 96 * 2;
    attackFrame.y = this->info.currentFrame.y;
    attackFrame.w = attackWidth;
    attackFrame.h = this->info.currentFrame.h;

    // for (Character& enemy : enemies) {
    if (collision(&attackFrame, &hitbox)) {
        // std::cout << "Collision" << std::endl;
        // std::cout << "pos x coord: " << this->info.position.x << std::endl;
        // std::cout << "frame x coord: " << this->info.currentFrame.x << std::endl;
        // std::cout << "attack x coord: " << attackFrame.x << std::endl;
        // std::cout << "slime x coord: " << enemies[0]->info.currentFrame.x << "\n" << std::endl;

        if (hitbox.x + 21 < this->info.currentFrame.x + 124) enemies[0]->info.facingRight = true;
        else enemies[0]->info.facingRight = false;

        enemies[0]->takeDamage();
    }
    // }
}

void Character::renderTexture(RenderWindow& window, Movement& movement, SDL_RendererFlip& flipType) {
    SDL_RenderCopyEx(window.getRenderer(), movement.texture, &movement.frameVector[movement.frame / movement.loopFrames], &this->info.currentFrame, 0, nullptr, flipType);
    if (movement.frame >= movement.frameVector.size() * movement.loopFrames - 1) movement.frame = 0;
    else movement.frame++;
}

// `renderCharacter()`
// Renders a dynamic character to the screen.
void Character::renderCharacter(RenderWindow& window) {

    // std::cout << this->info.position.x << ", " << this->info.position.y << std::endl;
    // std::cout << this->info.velocityY << std::endl;

    SDL_RendererFlip flipType = this->info.facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    if (this->hurt.isActive()) {
        this->renderTexture(window, this->hurtMovement, flipType);
        if (this->hurtMovement.frame == 0) this->hurt.stop();
    } else if (this->attack.isActive()) {
        if (this->attack.movement->frame == 3 * this->attack.movement->loopFrames) this->dealDamage();
        this->renderTexture(window, this->attackMovement, flipType);
        if (this->attackMovement.frame == 0) this->attack.stop();
    } else if (this->isAirborne()) {
        if (this->airAttack.isActive()) {
            this->renderTexture(window, this->airAttackMovement, flipType);
            if (this->airAttackMovement.frame == 0) this->airAttack.stop();
        } else {
            int frame = -this->info.velocityY / 4 + 4;
            if (frame < 0) frame = 0;
            if (frame > 7) frame = 7;
            SDL_RenderCopyEx(window.getRenderer(), this->jumpMovement.texture, &this->jumpMovement.frameVector[frame], &this->info.currentFrame, 0, nullptr, flipType);
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

    this->move(window);
    this->obeyGravity();
}

void Character::startAttack() {
    this->heal.stop();
    this->attack.start();
}

void Character::takeDamage() {
    this->attack.stop();
    this->run.stop();
    this->crouch.stop();
    this->heal.stop();
    this->pray.stop();

    this->hurt.start();
}

void Character::startMovingRight() {
    this->info.movingRight = true;
}

void Character::startMovingLeft() {
    this->info.movingLeft = true;
}

void Character::move(RenderWindow& window) {
    if (this->isMovingRight()) {
        if (!this->attack.isActive()) this->info.facingRight = true;
    } else if (this->isMovingLeft()) {
        this->info.movingLeft = true;
        if (!this->attack.isActive()) this->info.facingRight = false;
    }

    float movementDelta = this->info.movementSpeed;
    if ((this->isImmobile() && !this->isAirborne()) || (this->isMovingRight() && this->isMovingLeft())) movementDelta = 0;
    
    if (this->isMovingLeft() && -100 < this->info.position.x) {
        this->info.position.x -= movementDelta;
        this->info.currentFrame.x -= movementDelta;
    }

    if (this->isMovingRight() && this->info.position.x < window.getWidth() - 150) {
        this->info.position.x += movementDelta;
        this->info.currentFrame.x += movementDelta;
    }
}

bool Character::isMovingRight() {
    return this->info.movingRight;
}

bool Character::isMovingLeft() {
    return this->info.movingLeft;
}

void Character::stopMovingRight() {
    this->info.movingRight = false;
}

void Character::stopMovingLeft() {
    this->info.movingLeft = false;
}


void Character::startJump() {
    if (!this->isAirborne()) {
        this->heal.stop();
        this->jump.start();
        this->info.velocityY = this->info.jumpForce;
    }
}

bool Character::isAirborne() {
    return this->jump.isActive();
}

void Character::obeyGravity() {
    if ((this->info.position.y > this->info.groundHeight) || (this->info.position.y >= this->info.groundHeight && this->info.velocityY < 0)) {
        // If we are below the ground or touching it with negative velocity, stop jumping.
        this->jump.stop();
        this->info.position.y = this->info.groundHeight;
        this->info.velocityY = 0;
    } else if (this->info.velocityY > 0 || this->info.position.y < this->info.groundHeight) {
        // If we have positive velocity or are airborne, obey gravity.
        this->info.position.y -= this->info.velocityY;
        this->info.velocityY += GRAVITY;
        if (this->info.velocityY < -10) this->info.velocityY = -10;
    }
    this->info.currentFrame.y = this->info.position.y;
}

bool Character::isImmobile() {
    return this->hurt.isActive() || this->attack.isActive() || this->heal.isActive() || this->crouch.isActive() || this->pray.isActive();
}
