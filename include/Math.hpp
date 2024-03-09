#ifndef MATH_H
#define MATH_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdlib.h>

struct Vector2i {
    Vector2i() :x(0.0f), y(0.0f) {};
    Vector2i(int x, int y) :x(x), y(y) {};
    int x, y;

    inline void print() {
        std::cout << "<" << x << ", " << y << ">" << std::endl;
    }
};


#endif // MATH_H
