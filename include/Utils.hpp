#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>
#include <SDL_image.h>

namespace utils
{
    inline float hireTimeInSeconds() {
        float t = SDL_GetTicks();
        t *= 0.001f;

        return t;
    }
}



#endif // UTILS_H
