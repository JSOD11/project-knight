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
    if (textures.size() > 4) this->initializeTextureLoop(this->death, this->deathMovement, textures[4], this->info.pngSize);
    if (textures.size() > 5) this->initializeTextureLoop(this->deathEffect, this->deathEffectMovement, textures[5], Vector2i(31, 32));
    if (textures.size() > 6) this->initializeTextureLoop(this->birthEffect, this->birthEffectMovement, textures[6], Vector2i(32, 32));
}

void Enemy::setSightDistance(int sightDistance) {
    this->sightDistance = sightDistance;
}

bool Enemy::dealDamage() {
    SDL_Rect attackBox = this->buildAttackBox();

    if (collision(&attackBox, &knight->info.hitbox)) {
        if (knight->info.centerCoordinates.x < this->info.centerCoordinates.x) knight->info.facingRight = true;
        else knight->info.facingRight = false;
        if (!knight->hurt.isActive()) {
            knight->takeDamage(this->info.attackDamage);
            return true;
        }
    }

    return false;
}

// `renderEnemy()`
// Renders a dynamic Enemy to the screen.
void Enemy::renderEnemy(RenderWindow& window) {

    // std::cout << this->info.position.x << ", " << this->info.position.y << std::endl;
    // std::cout << this->info.velocityY << std::endl;

    if (this->info.health <= 0) this->death.start();

    if (!this->birthEffect.isActive() && !this->deathEffect.isActive()) {
        if (std::abs(knight->info.centerCoordinates.x - this->info.centerCoordinates.x) <= this->sightDistance) {
            this->stopMovingRight();
            this->stopMovingLeft();
            if (knight->info.centerCoordinates.x < this->info.centerCoordinates.x) this->startMovingLeft();
            else this->startMovingRight();
        }

        if (std::abs(knight->info.centerCoordinates.x - this->info.centerCoordinates.x) <= 30) {
            this->stopMovingRight();
            this->stopMovingLeft();
        }

        if (collision(&this->info.currentFrame, &knight->info.hitbox)) {
            if (std::rand() % 60 == 0) {
                this->attack.start();
            }
        }
    }
            
    SDL_RendererFlip flipType = this->info.facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    if (this->birthEffect.isActive()) {
        this->renderTexture(window, this->birthEffectMovement, flipType, true);
        if (this->birthEffectMovement.frame == 0) this->birthEffect.stop();
    } else if (this->deathEffect.isActive()) {
        this->renderTexture(window, this->deathEffectMovement, flipType, true);
        if (this->deathEffectMovement.frame == 0) {
            this->death.stop();
            this->deathEffect.stop();
            this->died = true;
        }
    } else if (this->death.isActive()) {
        this->renderTexture(window, this->deathMovement, flipType, false);
        if (this->deathMovement.frame == 0) {
            this->deathEffect.start();
        }
    } else if (this->hurt.isActive()) {
        this->renderTexture(window, this->hurtMovement, flipType, false);
        if (this->hurtMovement.frame == 0) {
            this->hurt.stop();
        }
    } else if (this->attack.isActive()) {
        if (this->attack.movement->frame == 3 * this->attack.movement->loopFrames) this->dealDamage();
        this->renderTexture(window, this->attackMovement, flipType, false);
        if (this->attackMovement.frame == 0) this->attack.stop();
    } else if ((this->isMovingLeft() || this->isMovingRight()) && !(this->isMovingLeft() && this->isMovingRight())) {
        this->renderTexture(window, this->runMovement, flipType, false);
        if (std::rand() % 400 == 0) {
            this->stopMovingRight();
            this->stopMovingLeft();
        }
    } else {
        this->renderTexture(window, this->idleMovement, flipType, false);
        if (std::rand() % 400 == 0) {
            if (std::rand() % 2 == 0) this->startMovingRight();
            else this->startMovingLeft();
        }
    }

    this->move(window, this->info.movementSpeed);

    // renderBox(window, this->info.hitbox);
    // SDL_Rect attackBox = this->buildAttackBox();
    // renderBox(window, attackBox);
}

bool Enemy::isDead() {
    return this->died;
}

bool Enemy::dealsBodyDamage() {
    return this->bodyDamage;
}
