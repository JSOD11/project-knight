#include <SDL.h>
#include <SDL_image.h>

#include "Enemy.hpp"
#include "Game.hpp"
#include "Player.hpp"

void Enemy::initializeMovementLoops(std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t>>& textures) {
    this->initializeTextureLoop(this->idle, this->idleMovement, textures[0], this->info.pngSize);
    if (textures.size() > 1) this->initializeTextureLoop(this->attack, this->attackMovement, textures[1], this->info.pngSize);
    if (textures.size() > 2) this->initializeTextureLoop(this->run, this->runMovement, textures[2], this->info.pngSize);
    if (textures.size() > 3) this->initializeTextureLoop(this->hurt, this->hurtMovement, textures[3], this->info.pngSize);
}

void Enemy::dealDamage() {
    SDL_Rect attackBox = this->info.currentFrame;
    if (this->info.facingRight) attackBox.x += this->info.attackBox.x - this->info.attackBox.w;
    else attackBox.x += this->info.sizeScaling * this->info.pngSize.x - this->info.attackBox.x;
    attackBox.y += this->info.attackBox.y;

    attackBox.w = this->info.attackBox.w;
    attackBox.h = this->info.attackBox.h;

    if (collision(&attackBox, &knight->info.hitbox)) {
        if (knight->info.centerCoordinates.x < this->info.centerCoordinates.x) knight->info.facingRight = true;
        else knight->info.facingRight = false;
        if (!knight->hurt.isActive()) knight->takeDamage();
    }
}

// `renderEnemy()`
// Renders a dynamic Enemy to the screen.
void Enemy::renderEnemy(RenderWindow& window) {

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
    } else if ((this->isMovingLeft() || this->isMovingRight()) && !(this->isMovingLeft() && this->isMovingRight())) {
        this->renderTexture(window, this->runMovement, flipType);
    } else {
        this->renderTexture(window, this->idleMovement, flipType);
    }

    this->move(window);
}

void Enemy::move(RenderWindow& window) {
    if (this->isMovingRight()) {
        if (!this->attack.isActive()) this->info.facingRight = true;
    } else if (this->isMovingLeft()) {
        this->info.movingLeft = true;
        if (!this->attack.isActive()) this->info.facingRight = false;
    }

    float movementDelta = this->info.movementSpeed;
    if (this->isImmobile() || (this->isMovingRight() && this->isMovingLeft())) movementDelta = 0;

    if (this->isMovingRight() && this->info.position.x < window.getWidth() - 150) {
        this->changeCoordinates(true, movementDelta);
    }
    
    if (this->isMovingLeft() && -100 < this->info.position.x) {
        this->changeCoordinates(true, -movementDelta);
    }
    this->info.currentFrame.x = this->info.position.x;
}

bool Enemy::isImmobile() {
    return this->hurt.isActive() || this->attack.isActive();
}
