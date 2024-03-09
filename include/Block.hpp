#ifndef BLOCK_H
#define BLOCK_H

#include <SDL.h>
#include <SDL_image.h>

#include "Math.hpp"

class Block {
    private:
        Vector2i position;
        SDL_Rect currentFrame;
        SDL_Texture* texture;
    
    public:
        Block(Vector2i position, SDL_Texture* texture);
        const Vector2i& getPosition() const {
            return this->position;
        }
        SDL_Texture* getTexture() const;
        SDL_Rect getCurrentFrame() const;
};

#endif // BLOCK_H
