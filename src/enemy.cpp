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
}

void Enemy::dealDamage() {
    SDL_Rect attackBox = this->buildAttackBox();

    if (collision(&attackBox, &knight->info.hitbox)) {
        if (knight->info.centerCoordinates.x < this->info.centerCoordinates.x) knight->info.facingRight = true;
        else knight->info.facingRight = false;
        if (!knight->hurt.isActive()) knight->takeDamage(this->info.attackDamage);
    }
}

// `renderEnemy()`
// Renders a dynamic Enemy to the screen.
void Enemy::renderEnemy(RenderWindow& window) {

    // std::cout << this->info.position.x << ", " << this->info.position.y << std::endl;
    // std::cout << this->info.velocityY << std::endl;
    if (collision(&this->info.currentFrame, &knight->info.hitbox)) {
        if (std::rand() % 120 == 0) {
            this->attack.start();
        }
    }

    SDL_RendererFlip flipType = this->info.facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    if (this->death.isActive()) {
        this->renderTexture(window, this->deathMovement, flipType);
        if (this->deathMovement.frame == 0) {
            this->death.stop();
            this->died = true;
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
    } else if ((this->isMovingLeft() || this->isMovingRight()) && !(this->isMovingLeft() && this->isMovingRight())) {
        this->renderTexture(window, this->runMovement, flipType);
        if (std::rand() % 400 == 0) {
            this->stopMovingRight();
            this->stopMovingLeft();
        }
    } else {
        this->renderTexture(window, this->idleMovement, flipType);
        if (std::rand() % 400 == 0) {
            if (std::rand() % 2 == 0) this->startMovingRight();
            else this->startMovingLeft();
        }
    }

    this->move(window);

    // renderBox(window, this->info.hitbox);
    // SDL_Rect attackBox = this->buildAttackBox();
    // renderBox(window, attackBox);
}

bool Enemy::isDead() {
    return this->died;
}

void createSlime(RenderWindow& window, std::string color, int groundHeight, int posX) {
    Enemy* slime = new Enemy(initializeInfo(85, 20, 1, Vector2i(128, 128), slimeHitbox, slimeAttackBox, 1, 5, groundHeight, posX));

    // Load and initialize slime textures.
    std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t>> textures;
    textures.push_back(std::make_tuple(window.loadTexture(("../graphics/enemies/slimes/" + color + "_Slime/Idle.png").c_str()), 0, 8, 8, 8));
    textures.push_back(std::make_tuple(window.loadTexture(("../graphics/enemies/slimes/" + color + "_Slime/Attack_3.png").c_str()), 0, 4, 4, 5));
    textures.push_back(std::make_tuple(window.loadTexture(("../graphics/enemies/slimes/" + color + "_Slime/Run.png").c_str()), 0, 7, 7, 5));
    textures.push_back(std::make_tuple(window.loadTexture(("../graphics/enemies/slimes/" + color + "_Slime/Hurt.png").c_str()), 0, 6, 6, 5));
    textures.push_back(std::make_tuple(window.loadTexture(("../graphics/enemies/slimes/" + color + "_Slime/Dead.png").c_str()), 0, 3, 3, 7));
    slime->initializeMovementLoops(textures);
    enemies[slime] = true;
}
