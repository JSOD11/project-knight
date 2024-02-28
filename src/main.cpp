#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Character.hpp"
#include "Entity.hpp"
#include "Utils.hpp"

void renderBackground(RenderWindow& window, SDL_Texture* bg1, SDL_Texture* bg2, SDL_Texture* bg3);
void renderGround(RenderWindow& window, SDL_Texture* texture, size_t groundHeight);

int main(int argc, char* argv[]) {

    (void) argc;
    (void) argv;

    if (SDL_Init(SDL_INIT_VIDEO) > 0 || !(IMG_Init(IMG_INIT_PNG))) {
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    RenderWindow window("Pearl Knight (Beta 0.0)", 1280, 720);
    
    SDL_Texture* tileTexture = window.loadTexture("../graphics/medieval/Tiles/floor_tile_3.png");
    if (!tileTexture) {
        std::cout << "Failed to load tile texture.\n";
        return 1;
    }

    SDL_Texture* bg1 = window.loadTexture("../graphics/mountains/background1.png");
    SDL_Texture* bg2 = window.loadTexture("../graphics/mountains/background2.png");
    SDL_Texture* bg3 = window.loadTexture("../graphics/mountains/background3.png");
    
    size_t groundHeight = 150;
    size_t sizeScaling = 2;
    size_t characterHeight = 64;
    size_t characterWidth = 128;
    size_t loopSpeed = 3;
    size_t movementSpeed = 5;
    Vector2f startingPosition(200, window.getHeight() - groundHeight - sizeScaling * characterHeight + 40);
    SDL_Rect startingFrame;
    startingFrame.x = startingPosition.x, startingFrame.y = startingPosition.y, startingFrame.w = characterWidth * sizeScaling, startingFrame.h = characterHeight * sizeScaling;

    Character pearlKnight(startingPosition, startingFrame, characterWidth, characterHeight, loopSpeed, movementSpeed);

    // Load and initialize knight textures.
    SDL_Texture* idleTexture = window.loadTexture("../graphics/knight/Idle.png");
    SDL_Texture* attackTexture = window.loadTexture("../graphics/knight/Attacks.png");
    SDL_Texture* runningTexture = window.loadTexture("../graphics/knight/Run.png");
    pearlKnight.initializeMovementLoops(idleTexture, attackTexture, runningTexture);

    // Cap game at 60 FPS.
    const size_t targetFPS = 60;
    const size_t frameDelay = 1000 / targetFPS;
    uint32_t frameStart, frameTime;

    bool engineRunning = true;
    SDL_Event event;
    std::cout << "\nStarting up engine...\n" << std::endl;
    while (engineRunning) {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
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

                case SDLK_SPACE:
                    if (!pearlKnight.currentlyAttacking()) pearlKnight.resetAttack0Frames();
                    pearlKnight.turnAttackingStatusOn();
                    break;
                
                case SDLK_a:
                    if (!pearlKnight.currentlyAttacking()) pearlKnight.moveLeft(window);
                    break;

                case SDLK_d:
                    if (!pearlKnight.currentlyAttacking()) pearlKnight.moveRight(window);
                    break;
                }
                break;
            
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                case SDLK_a:
                    pearlKnight.stopMovingLeft();
                    break;

                case SDLK_d:
                    pearlKnight.stopMovingRight();
                    break;
                }
                break;
            }
        }

        // TODO: Condense these into one move function based on which way we are facing.
        if (pearlKnight.isMovingLeft()) pearlKnight.moveLeft(window);
        if (pearlKnight.isMovingRight()) pearlKnight.moveRight(window);

        // Rendering.
        window.clearWindow();
        renderBackground(window, bg1, bg2, bg3);
        renderGround(window, tileTexture, groundHeight);
        pearlKnight.renderCharacter(window);
        window.display();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    }

    // TODO: Destroy all textures.
    window.destroyTexture(tileTexture);

    window.destroyWindow();
    SDL_Quit();

    return 0;
}

void renderBackground(RenderWindow& window, SDL_Texture* bg1, SDL_Texture* bg2, SDL_Texture* bg3) {
    SDL_RenderCopy(window.getRenderer(), bg1, NULL, NULL);
    SDL_RenderCopy(window.getRenderer(), bg2, NULL, NULL);
    SDL_RenderCopy(window.getRenderer(), bg3, NULL, NULL);
}

void renderGround(RenderWindow& window, SDL_Texture* texture, size_t groundHeight) {

    size_t numTiles = window.getWidth() / 32;

    for (size_t i = 0; i < numTiles; i++) {
        Vector2f tilePosition(i * 32, groundHeight);
        window.renderStatic(Entity(tilePosition, texture));
    }
}
