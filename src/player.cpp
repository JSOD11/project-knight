#include <SDL.h>
#include <SDL_image.h>

#include "Player.hpp"
#include "Game.hpp"
#include "RenderWindow.hpp"
#include "Enemy.hpp"

void createPlayer(RenderWindow& window, int groundHeight, int posX) {
    knight = new Player(initializeInfo(100, 30, 2, Vector2i(128, 64), knightHitbox, knightAttackBox, 6, 15, groundHeight, posX));

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
}

void Player::renderHealthBar(RenderWindow& window, SDL_Texture*& healthBarBackground, SDL_Texture*& bar, SDL_Texture*& healthBar) {

    (void) bar;

    SDL_Rect healthBarBox;
    size_t maxWidth = 200;

    healthBarBox.x = 40, healthBarBox.y = window.getHeight() - 40, healthBarBox.w = maxWidth, healthBarBox.h = 10;
    SDL_RenderCopy(window.getRenderer(), healthBarBackground, nullptr, &healthBarBox);
    // SDL_RenderCopy(window.getRenderer(), bar, nullptr, &healthBarBox);
    healthBarBox.x = 40, healthBarBox.y = window.getHeight() - 40, healthBarBox.w = maxWidth * this->info.health / this->info.maxHealth, healthBarBox.h = 10;
    SDL_RenderCopy(window.getRenderer(), healthBar, nullptr, &healthBarBox);
}

void Player::dealDamage() {
    SDL_Rect attackBox = this->buildAttackBox();
    for (auto& pair : enemies) {
        Enemy* enemy = pair.first;
        if (collision(&attackBox, &enemy->info.hitbox)) {
            if (enemy->info.centerCoordinates.x < this->info.centerCoordinates.x) enemy->info.facingRight = true;
            else enemy->info.facingRight = false;
            enemy->takeDamage(this->info.attackDamage);
        }
    }
}

// `renderPlayer()`
// Renders a dynamic Player to the screen.
void Player::renderPlayer(RenderWindow& window) {

    // std::cout << this->info.position.x << ", " << this->info.position.y << std::endl;
    // std::cout << this->info.velocityY << std::endl;

    for (auto& pair : enemies) {
        Enemy* enemy = pair.first;
        if (collision(&knight->info.hitbox, &enemy->info.hitbox)) {
            this->takeDamage(enemy->info.attackDamage);
        }
    }

    SDL_RendererFlip flipType = this->info.facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    if (this->death.isActive()) {
        this->renderTexture(window, this->deathMovement, flipType);
        if (this->deathMovement.frame == 0) {
            this->death.stop();

            // For now, we're just respawning right after death.
            this->info.health = 100;
        }
    } else if (this->hurt.isActive()) {
        this->renderTexture(window, this->hurtMovement, flipType);
        if (this->hurtMovement.frame == 0) {
            this->hurt.stop();
            if (this->info.health <= 0) this->death.start();
        }
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
        if (this->heal.movement->frame == 3 * this->heal.movement->loopFrames) this->info.health = std::min(100, this->info.health + 30);
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

    // renderBox(window, this->info.hitbox);
    // SDL_Rect attackBox = buildAttackBox();
    // renderBox(window, attackBox);
}

void Player::startAttack() {
    this->heal.stop();
    this->attack.start();
}

void Player::takeDamage(size_t damage) {
    if (!this->hurt.isActive()) {
        this->attack.stop();
        this->run.stop();
        this->crouch.stop();
        this->heal.stop();
        this->pray.stop();
        if (this->info.facingRight) this->changeCoordinates(true, -10);
        else this->changeCoordinates(true, 10);
        this->hurt.start();
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
        this->setCoordinate(false, this->info.groundHeight);
        this->info.velocityY = 0;
    } else if (this->info.velocityY > 0 || this->info.position.y < this->info.groundHeight) {
        // If we have positive velocity or are airborne, obey gravity.
        this->changeCoordinates(false, -this->info.velocityY);
        this->info.velocityY += GRAVITY;
        if (this->info.velocityY < -10) this->info.velocityY = -10;
    }
}
