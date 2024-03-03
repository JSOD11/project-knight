#include <SDL.h>
#include <SDL_image.h>

#include "Block.hpp"

Block::Block(Vector2i position, SDL_Texture* texture) 
    :position(position), texture(texture) {
    this->currentFrame.x = 0;
    this->currentFrame.y = 0;
    this->currentFrame.w = 32;
    this->currentFrame.h = 32;
}

SDL_Texture* Block::getTexture() const {
    return this->texture;
}

SDL_Rect Block::getCurrentFrame() const {
    return this->currentFrame;
}
