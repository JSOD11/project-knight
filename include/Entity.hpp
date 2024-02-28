#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>
#include <SDL_image.h>

#include "Math.hpp"

class Entity {
    private:
        Vector2f position;
        SDL_Rect currentFrame;
        SDL_Texture* texture;
    
    public:
        Entity(Vector2f position, SDL_Texture* texture);
        const Vector2f& getPosition() const {
            return this->position;
        }
        SDL_Texture* getTexture() const;
        SDL_Rect getCurrentFrame() const;
};

#endif // ENTITY_H
