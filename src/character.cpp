#include <SDL.h>
#include <SDL_image.h>

#include "Character.hpp"
#include "Game.hpp"

void Character::initializeTextureLoop(Command& command, Movement& movement, std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t>& texture, Vector2i pngSize) {

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
        frame.x = (i % rowLength) * pngSize.x;
        frame.y = (i / rowLength) * pngSize.y;
        frame.w = pngSize.x;
        frame.h = pngSize.y;
        movement.frameVector.push_back(frame);
    }
}

void Character::renderTexture(RenderWindow& window, Movement& movement, SDL_RendererFlip& flipType) {
    SDL_RenderCopyEx(window.getRenderer(), movement.texture, &movement.frameVector[movement.frame / movement.loopFrames], &this->info.currentFrame, 0, nullptr, flipType);
    if (movement.frame >= movement.frameVector.size() * movement.loopFrames - 1) movement.frame = 0;
    else movement.frame++;
}

void Character::takeDamage(size_t damage) {
    if (!this->hurt.isActive()) {
        this->attack.stop();
        this->run.stop();
        if (this->info.facingRight) this->changeCoordinates(true, -10);
        else this->changeCoordinates(true, 10);
        this->hurt.start();
        this->info.health -= damage;
    }
}

void Character::startMovingRight() {
    this->info.movingRight = true;
}

void Character::startMovingLeft() {
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
