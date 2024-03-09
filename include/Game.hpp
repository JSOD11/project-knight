#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>

#include "Math.hpp"
#include "RenderWindow.hpp"
#include "Character.hpp"

const int GRAVITY = -1;

void processInput(RenderWindow& window, SDL_Event& event, bool& engineRunning);
bool collision(SDL_Rect* x, SDL_Rect* y);

#endif // GAME_H
