#include <SDL.h>
#include <SDL_image.h>

#include "Player.hpp"
#include "Game.hpp"
#include "RenderWindow.hpp"
#include "Enemy.hpp"

void createPlayer(RenderWindow& window, int groundHeight, int posX) {
    knight = new Player(1, initializeInfo(100, 30, 2, Vector2i(128, 64), knightHitbox, knightAttackBox, 6, 15, groundHeight, posX));

    // The tuple has form: (texture, startPosition, numElements, rowLength, loopFrames)
    std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t>> textures;
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Idle.png"), 0, 8, 2, 3));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Attacks.png"), 0, 7, 8, 3));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Run.png"), 0, 8, 2, 2));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Hurt.png"), 0, 3, 2, 10));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Death.png"), 0, 4, 2, 100));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/crouch_idle.png"), 0, 8, 2, 3));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Jump.png"), 0, 8, 2, 3));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Health.png"), 0, 8, 2, 5));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Pray.png"), 2, 10, 4, 5));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/attack_from_air.png"), 0, 3, 2, 3));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/roll.png"), 0, 4, 2, 4));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Attacks.png"), 7, 3, 8, 3));
    knight->initializeMovementLoops(textures);
}

void Player::initializeMovementLoops(std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t>>& textures) {
    this->initializeTextureLoop(this->idle, this->idleMovement, textures[0], this->info.pngSize);
    if (textures.size() > 1) this->initializeTextureLoop(this->attack, this->attackMovement, textures[1], this->info.pngSize);
    if (textures.size() > 2) this->initializeTextureLoop(this->run, this->runMovement, textures[2], this->info.pngSize);
    if (textures.size() > 3) this->initializeTextureLoop(this->hurt, this->hurtMovement, textures[3], this->info.pngSize);
    if (textures.size() > 4) this->initializeTextureLoop(this->death, this->deathMovement, textures[4], this->info.pngSize);
    if (textures.size() > 5) this->initializeTextureLoop(this->crouch, this->crouchMovement, textures[5], this->info.pngSize);
    if (textures.size() > 6) this->initializeTextureLoop(this->jump, this->jumpMovement, textures[6], this->info.pngSize);
    if (textures.size() > 7) this->initializeTextureLoop(this->heal, this->healMovement, textures[7], this->info.pngSize);
    if (textures.size() > 8) this->initializeTextureLoop(this->pray, this->prayMovement, textures[8], this->info.pngSize);
    if (textures.size() > 9) this->initializeTextureLoop(this->airAttack, this->airAttackMovement, textures[9], this->info.pngSize);
    if (textures.size() > 10) this->initializeTextureLoop(this->roll, this->rollMovement, textures[10], this->info.pngSize);
    if (textures.size() > 11) this->initializeTextureLoop(this->attack2, this->attack2Movement, textures[11], this->info.pngSize);
}

void Player::renderHUD(RenderWindow& window, std::vector<SDL_Texture*>& hudTextures) {

    SDL_Rect vignetteRect;
    if (!this->death.isActive()) this->vignetteSize = 470 - this->corruption / 5;
    int vignetteSize = this->vignetteSize;
    if (this->death.isActive()) this->vignetteSize -= 0.2;
    vignetteRect.x = vignetteSize, vignetteRect.y = vignetteSize, vignetteRect.h = 1000 - 2 * vignetteSize, vignetteRect.w = 1000 - 2 * vignetteSize;
    SDL_RenderCopy(window.getRenderer(), hudTextures[15], &vignetteRect, nullptr);

    SDL_Rect healthBarBox;
    size_t healthWidth = 300;

    healthBarBox.x = 100, healthBarBox.y = window.getHeight() - 75, healthBarBox.w = healthWidth, healthBarBox.h = 15;
    SDL_RenderCopy(window.getRenderer(), hudTextures[10], nullptr, &healthBarBox);
    healthBarBox.w = healthWidth * this->info.health / this->info.maxHealth;
    SDL_RenderCopy(window.getRenderer(), hudTextures[12], nullptr, &healthBarBox);

    SDL_Rect corruptionBarBox;
    size_t corruptionWidth = 200;

    corruptionBarBox.x = 100, corruptionBarBox.y = window.getHeight() - 60, corruptionBarBox.w = corruptionWidth, corruptionBarBox.h = 15;
    SDL_RenderCopy(window.getRenderer(), hudTextures[10], nullptr, &corruptionBarBox);
    corruptionBarBox.w = corruptionWidth * this->corruption / this->maxCorruption;
    SDL_RenderCopy(window.getRenderer(), hudTextures[16], nullptr, &corruptionBarBox);

    SDL_Rect iconBox;
    iconBox.x = 50, iconBox.y = window.getHeight() - 80, iconBox.w = 50, iconBox.h = 50;
    SDL_RenderCopy(window.getRenderer(), hudTextures[13], nullptr, &iconBox);

    SDL_Rect gemRect;
    gemRect.x = 57, gemRect.y = window.getHeight() - 72, gemRect.w = 38, gemRect.h = 38;
    SDL_RenderCopy(window.getRenderer(), hudTextures[14], nullptr, &gemRect);

    SDL_Texture* number = hudTextures[this->healGems];
    SDL_Rect numberRect;
    numberRect.x = 80, numberRect.y = window.getHeight() - 55, numberRect.w = 20, numberRect.h = 20;
    SDL_RenderCopy(window.getRenderer(), number, nullptr, &numberRect);
}

bool Player::dealDamage(RenderWindow& window, bool airAttack) {

    (void) window;

    SDL_Rect attackBox;
    if (airAttack) {
        attackBox = this->info.hitbox;
        attackBox.y += 10;
    } else attackBox = this->buildAttackBox();

    bool dealtDamage = false;

    for (auto& pair : enemies) {
        Enemy* enemy = pair.first;
        if (collision(&attackBox, &enemy->info.hitbox)) {
            if (enemy->info.centerCoordinates.x < this->info.centerCoordinates.x) enemy->info.facingRight = true;
            else enemy->info.facingRight = false;
            enemy->takeDamage(this->info.attackDamage);
            this->corruption -= 1;
            if (enemy->info.health - this->info.attackDamage <= 0) this->corruption -= 5;
            if (this->corruption < 0) this->corruption = 0;
            dealtDamage = true;
        }
    }

    return dealtDamage;
}

// `renderPlayer()`
// Renders a dynamic Player to the screen.
void Player::renderPlayer(RenderWindow& window) {

    // std::cout << this->info.position.x << ", " << this->info.position.y << std::endl;
    // std::cout << this->info.velocityY << std::endl;

    this->corruption += 0.01;

    if (this->corruption >= this->maxCorruption) this->death.start();

    for (auto& pair : enemies) {
        Enemy* enemy = pair.first;
        if (enemy->dealsBodyDamage() && collision(&knight->info.hitbox, &enemy->info.hitbox)) {
            this->takeDamage(enemy->info.attackDamage);
        }
    }

    if (this->invincibilityFrame >= 0) {
        if (this->invincibilityFrame == 60) this->invincibilityFrame = -1;
        else this->invincibilityFrame++;
    }

    float movementSpeed = this->info.movementSpeed;

    SDL_RendererFlip flipType = this->info.facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    if (this->death.isActive()) {
        this->renderTexture(window, this->deathMovement, flipType, false);
        if (this->deathMovement.frame == 0) {
            this->death.stop();

            // For now, we're just respawning right after death.
            this->info.health = 100;
            this->healGems = 9;
            this->corruption = 0;
            this->invincibilityFrame = 0;
            this->vignetteSize = 470;
        }
    } else if (this->hurt.isActive()) {
        this->roll.stop();
        this->renderTexture(window, this->hurtMovement, flipType, false);
        if (this->hurtMovement.frame == 0) {
            this->hurt.stop();
            if (this->info.health <= 0) this->death.start();
        }
    } else if (this->roll.isActive()) {
        movementSpeed *= 2;
        this->renderTexture(window, this->rollMovement, flipType, false);
        if (this->roll.movement->frame == 0) {
            if (this->isAirborne()) this->roll.disable();
            this->roll.stop();
        }
    } else if (this->attack.isActive()) {
        if (!(this->attack.movement->frame == 0 && this->attack2.isActive())) {
            if (this->attack.movement->frame == 3 * this->attack.movement->loopFrames) this->dealDamage(window, false);
            this->renderTexture(window, this->attackMovement, flipType, false);
            if (this->attack.movement->frame == 0) this->attack.stop();
        } else {
            if (this->attack2.movement->frame == 1 * this->attack2.movement->loopFrames) this->dealDamage(window, false);
            this->renderTexture(window, this->attack2Movement, flipType, false);
            if (this->attack2.movement->frame == 0) {
                this->attack2.stop();
                this->attack.stop();
            }
        }
    } else if (this->isAirborne()) {
        if (this->airAttack.isActive()) {
            if (this->dealDamage(window, true)) {
                this->jump.stop();
                this->startJump();
            }
            this->renderTexture(window, this->airAttackMovement, flipType, false);
            if (this->airAttackMovement.frame == 0) this->airAttack.stop();
        } else {
            int frame = -this->info.velocityY / 4 + 4;
            if (frame < 0) frame = 0;
            if (frame > 7) frame = 7;
            SDL_RenderCopyEx(window.getRenderer(), this->jumpMovement.texture, &this->jumpMovement.frameVector[frame], &this->info.currentFrame, 0, nullptr, flipType);
        }
    } else if (this->heal.isActive()) {
        if (this->heal.movement->frame == 3 * this->heal.movement->loopFrames) {
            this->info.health = std::min(100, this->info.health + 30);
            this->corruption += 20;
            this->healGems--;
        }
        this->renderTexture(window, this->healMovement, flipType, false);
        if (this->healMovement.frame == 0) this->healMovement.isActive = false;
    } else if ((this->pray.isActive())) {
        this->renderTexture(window, this->prayMovement, flipType, false);
    } else if (this->crouch.isActive()) {
        this->renderTexture(window, this->crouchMovement, flipType, false);
    } else if ((this->isMovingLeft() || this->isMovingRight()) && !(this->isMovingLeft() && this->isMovingRight())) {
        this->renderTexture(window, this->runMovement, flipType, false);
    } else {
        this->renderTexture(window, this->idleMovement, flipType, false);
    }

    this->move(window, movementSpeed);
    if (!this->roll.isActive()) this->obeyGravity();

    // renderBox(window, this->info.hitbox);
    // SDL_Rect attackBox = buildAttackBox();
    // renderBox(window, attackBox);
}

void Player::startAttack(size_t attackNumber) {
    this->heal.stop();
    if (attackNumber == 2) this->attack2.start();
    else this->attack.start();
}

void Player::takeDamage(size_t damage) {
    if (!this->hurt.isActive() && this->invincibilityFrame < 0) {
        this->attack.stop();
        this->run.stop();
        this->crouch.stop();
        this->heal.stop();
        this->pray.stop();
        if (this->info.facingRight) this->changeCoordinates(true, -10);
        else this->changeCoordinates(true, 10);
        this->hurt.start();
        this->invincibilityFrame = 0;
        this->info.health -= damage;
    }
}

bool Player::canMove() {
    return !this->isImmobile() || this->isAirborne();
}

bool Player::isImmobile() {
    return this->death.isActive() || this->hurt.isActive() || this->attack.isActive() || this->heal.isActive() || this->crouch.isActive() || this->pray.isActive();
}

void Player::startJump() {
    if (!this->death.isActive() && !this->isAirborne()) {
        this->heal.stop();
        this->jump.start();
        this->info.velocityY = this->info.jumpForce;
    }
}

bool Player::isAirborne() {
    return this->jump.isActive();
}

void Player::obeyGravity() {
    if ((this->info.position.y > this->info.groundHeight) || (this->info.position.y >= this->info.groundHeight && this->info.velocityY < 0)) {
        // If we are below the ground or touching it with negative velocity, stop jumping.
        this->jump.stop();
        this->roll.enable();
        this->setCoordinate(false, this->info.groundHeight);
        this->info.velocityY = 0;
    } else if (this->info.velocityY > 0 || this->info.position.y < this->info.groundHeight) {
        // If we have positive velocity or are airborne, obey gravity.
        this->changeCoordinates(false, -this->info.velocityY);
        this->info.velocityY += GRAVITY;
        if (this->info.velocityY < -10) this->info.velocityY = -10;
    }
}

void Player::startRoll() {
    if (!this->death.isActive()) {
        this->heal.stop();
        this->info.velocityY = 0;
        this->attack.stop();
        this->roll.start();
    }
}

bool Player::canHeal() {
    return this->healGems > 0;
}
