#include <SDL.h>
#include <SDL_image.h>

#include "Character.hpp"
#include "Game.hpp"

void Character::initializeTextureLoop(Command& command, Movement& movement, std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t, size_t>& texture, Vector2i pngSize) {

    movement.texture = std::get<0>(texture);
    if (!movement.texture) std::cout << "Failed to load texture.\n";

    size_t start = std::get<1>(texture);
    size_t N = std::get<2>(texture);
    size_t rowLength = std::get<3>(texture);
    movement.frame = 0;
    movement.executionFrame = std::get<5>(texture);
    movement.disabled = false;
    movement.loopFrames = std::get<4>(texture);
    movement.isActive = false;

    command.movement = &movement;

    // N is the number of images in the loop. rowLength is the length of a row in the png.
    for (size_t i = start; i < start + N; i++) {
        SDL_Rect frame;
        frame.x = (i % rowLength) * pngSize.x;
        frame.y = (i / rowLength) * pngSize.y;
        frame.w = pngSize.x;
        frame.h = pngSize.y;
        movement.frameVector.push_back(frame);
    }
}

void Character::renderTexture(RenderWindow& window, Movement& movement, SDL_RendererFlip& flipType, bool hitbox) {
    SDL_Rect frame = this->info.currentFrame;
    if (hitbox) frame = this->info.hitbox;
    SDL_RenderCopyEx(window.getRenderer(), movement.texture, &movement.frameVector[movement.frame / movement.loopFrames], &frame, 0, nullptr, flipType);
    if (movement.frame >= movement.frameVector.size() * movement.loopFrames - 1) movement.frame = 0;
    else movement.frame++;
}

void Character::takeDamage(size_t damage) {
    if (std::rand() % this->poise == 0) {
        this->attack.stop();
        this->run.stop();
        this->hurt.start();
    }
    this->info.health -= damage;
}

bool Character::canMove() {
    return !this->isImmobile();
}

bool Character::canChangeDirection() {
    return !this->attack.isActive() && !this->death.isActive();
}

bool Character::isImmobile() {
    return this->hurt.isActive() || this->attack.isActive() || this->death.isActive() || (this->isMovingRight() && this->isMovingLeft());
}

void Character::move(RenderWindow& window, float movementSpeed) {
    if (this->canChangeDirection()) {
        if (this->isMovingRight()) {
            this->info.facingRight = true;
        } else if (this->isMovingLeft()) {
            this->info.movingLeft = true;
            this->info.facingRight = false;
        }
    }

    float movementDelta = movementSpeed;
    if (!this->canMove()) movementDelta = 0;

    if (this->isMovingRight() && this->info.hitbox.x + this->info.hitbox.w < window.getWidth()) {
        this->changeCoordinates(true, movementDelta);
    }
    
    if (this->isMovingLeft() && 0 < this->info.hitbox.x) {
        this->changeCoordinates(true, -movementDelta);
    }
}

void Character::startMovingRight() {
    // When the character is facing right, just add the hitbox x coord.
    this->info.hitbox.x = info.currentFrame.x + info.hitboxX;
    this->info.movingRight = true;
}

void Character::startMovingLeft() {
    // Since the character may not be symmetrical, we need to redefine the hitbox x coordinate when the
    // character turns to the left. To do this, start from the upper right corner of the current frame, then
    // subtract hitboxX and also the hitbox width (since the box starts at the character's upper right corner).
    this->info.hitbox.x = info.currentFrame.x + info.sizeScaling * info.pngSize.x - info.hitboxX - info.hitbox.w;
    this->info.movingLeft = true;
}

void Character::changeCoordinates(bool x, int delta) {
    if (x) {
        this->info.position.x += delta;
        this->info.currentFrame.x += delta;
        this->info.hitbox.x += delta;
        this->info.centerCoordinates.x += delta;
    } else {
        this->info.position.y += delta;
        this->info.currentFrame.y += delta;
        this->info.hitbox.y += delta;
        this->info.centerCoordinates.y += delta;
    }
}

void Character::setCoordinate(bool x, int coordinate) {
    if (x) {
        int delta = coordinate - this->info.position.x;
        this->info.position.x += delta;
        this->info.currentFrame.x += delta;
        this->info.hitbox.x += delta;
        this->info.centerCoordinates.x += delta;
    } else {
        int delta = coordinate - this->info.position.y;
        this->info.position.y += delta;
        this->info.currentFrame.y += delta;
        this->info.hitbox.y += delta;
        this->info.centerCoordinates.y += delta;
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

SDL_Rect Character::buildAttackBox() {
    SDL_Rect attackBox = this->info.currentFrame;
    if (this->info.facingRight) attackBox.x += this->info.attackBox.x - this->info.attackBox.w;
    else attackBox.x += this->info.sizeScaling * this->info.pngSize.x - this->info.attackBox.x;
    attackBox.y += this->info.attackBox.y;

    attackBox.w = this->info.attackBox.w;
    attackBox.h = this->info.attackBox.h;
    return attackBox;
}

void Character::renderBox(RenderWindow& window, SDL_Rect& box) {
    SDL_SetRenderDrawColor(window.getRenderer(), 255, 0, 0, 255);
    SDL_RenderDrawRect(window.getRenderer(), &box);
    SDL_RenderDrawPoint(window.getRenderer(), box.x + box.w / 2, box.y + box.h / 2);
}

Info initializeInfo(int health, size_t attackDamage, float sizeScaling, Vector2i pngSize, SDL_Rect hitbox, SDL_Rect attackBox, size_t movementSpeed, size_t jumpForce, int groundHeight, size_t posX) {
    Info info = {
        .health = health,
        .maxHealth = health,
        .attackDamage = attackDamage,
        .sizeScaling = sizeScaling,
        .position = Vector2i(posX, groundHeight),
        .pngSize = pngSize,
        .hitboxX = hitbox.x,
        .attackBox = attackBox,
        .movementSpeed = movementSpeed,
        .jumpForce = jumpForce,
        .groundHeight = groundHeight,

        .facingRight = true,
        .movingRight = false,
        .movingLeft = false,
        .velocityY = 0
    };
    info.currentFrame.x = info.position.x, info.currentFrame.y = info.position.y, info.currentFrame.w = info.pngSize.x * sizeScaling, info.currentFrame.h = info.pngSize.y * sizeScaling;
    info.hitbox.x = info.currentFrame.x + hitbox.x;
    info.hitbox.y = info.currentFrame.y + hitbox.y;
    info.hitbox.w = hitbox.w;
    info.hitbox.h = hitbox.h;
    info.centerCoordinates.x = info.hitbox.x + info.hitbox.w / 2, info.centerCoordinates.y = info.hitbox.y + info.hitbox.h / 2;

    return info;
}
