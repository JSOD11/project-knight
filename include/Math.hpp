#ifndef MATH_H
#define MATH_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdlib.h>

struct Vector2f {
    Vector2f() :x(0.0f), y(0.0f) {};
    Vector2f(float x, float y) :x(x), y(y) {};
    float x, y;

    void print() {
        std::cout << "<" << x << ", " << y << ">" << std::endl;
    }
};


#endif // MATH_H
