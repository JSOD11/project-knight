#include "Game.hpp"
#include "RenderWindow.hpp"
#include "Character.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

void processInput(RenderWindow& window, SDL_Event& event, bool& engineRunning) {
    switch (event.type) {
        case SDL_QUIT:
            engineRunning = false;
            std::cout << "\nShutting down engine... " << std::endl;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_0: {
                bool isFullScreen = SDL_GetWindowFlags(window.getWindow()) & SDL_WINDOW_FULLSCREEN_DESKTOP;
                SDL_SetWindowFullscreen(window.getWindow(), isFullScreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
                break;
            }

            case SDLK_UP:
                Enemy* enemy;
                for (auto& pair : enemies) {
                    enemy = pair.first;
                }
                if (enemy) enemy->attack.start();
                break;

            case SDLK_j:
                if (knight->isAirborne() && knight->crouch.isActive()) {
                    knight->airAttack.start();
                } else if (knight->attack.isActive()) {
                    knight->startAttack(2);
                } else {
                    knight->startAttack(1);
                }
                break;

            case SDLK_k:
                if (!knight->hurt.isActive() && !knight->roll.isDisabled() && \
                    (knight->isMovingRight() || knight->isMovingLeft()) && \
                    !(knight->isMovingRight() && knight->isMovingLeft())) knight->startRoll();
                break;
            
            case SDLK_l:
                if (!knight->heal.isActive() && knight->canHeal()) knight->heal.start();
                break;
            
            case SDLK_d:
                if (!knight->roll.isActive()) knight->startMovingRight();
                break;

            case SDLK_a:
                if (!knight->roll.isActive()) knight->startMovingLeft();
                break;
            
            case SDLK_w:
                if (!knight->hurt.isActive()) knight->startJump();
                break;

            case SDLK_m:
                knight->pray.start();
                break;

            case SDLK_s:
                knight->crouch.start();
                break;
            }
            break;
        
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            case SDLK_a:
                knight->stopMovingLeft();
                break;

            case SDLK_d:
                knight->stopMovingRight();
                break;
            
            case SDLK_m:
                knight->pray.stop();
                break;

            case SDLK_s:
                knight->crouch.stop();
                break;
            }
            break;
    }
}

bool collision(SDL_Rect* x, SDL_Rect* y) {
    if (SDL_HasIntersection(x, y) == SDL_TRUE) {
        return true;
    }
    return false;
}
