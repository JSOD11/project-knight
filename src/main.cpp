#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Character.hpp"
#include "Slime.hpp"
#include "Entity.hpp"
#include "Utils.hpp"

void renderBackground(RenderWindow& window, SDL_Texture* bg1, SDL_Texture* bg2, SDL_Texture* bg3);
void renderGround(RenderWindow& window, SDL_Texture* texture);

int main(int argc, char* argv[]) {

    (void) argc;
    (void) argv;

    if (SDL_Init(SDL_INIT_VIDEO) > 0 || !(IMG_Init(IMG_INIT_PNG))) {
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    RenderWindow window("Pearl Knight (Beta 0.0)", 1280, 720);
    
    SDL_Texture* tileTexture = window.loadTexture("../graphics/world/medieval/Tiles/floor_tile_3.png");
    if (!tileTexture) {
        std::cout << "Failed to load tile texture.\n";
        return 1;
    }

    SDL_Texture* bg1 = window.loadTexture("../graphics/background/mountains/background1.png");
    SDL_Texture* bg2 = window.loadTexture("../graphics/background/mountains/background2.png");
    SDL_Texture* bg3 = window.loadTexture("../graphics/background/mountains/background3.png");
    
    size_t sizeScaling = 2;
    size_t characterHeight = 64;
    size_t characterWidth = 128;
    size_t loopSpeed = 3;
    float movementSpeed = 6.5;
    size_t jumpForce = 27;
    size_t groundHeight = window.getHeight() - sizeScaling * characterHeight - 110;
    Vector2f startingPosition(200, groundHeight);
    SDL_Rect startingFrame;
    startingFrame.x = startingPosition.x, startingFrame.y = startingPosition.y, startingFrame.w = characterWidth * sizeScaling, startingFrame.h = characterHeight * sizeScaling;

    Character pearlKnight(startingPosition, startingFrame, characterWidth, characterHeight, loopSpeed, movementSpeed, jumpForce, groundHeight);

    // Load and initialize knight textures.
    std::vector<SDL_Texture*> textures;
    textures.push_back(window.loadTexture("../graphics/player/knight/Idle.png"));
    textures.push_back(window.loadTexture("../graphics/player/knight/Attacks.png"));
    textures.push_back(window.loadTexture("../graphics/player/knight/Run.png"));
    textures.push_back(window.loadTexture("../graphics/player/knight/crouch_idle.png"));
    textures.push_back(window.loadTexture("../graphics/player/knight/Jump.png"));
    textures.push_back(window.loadTexture("../graphics/player/knight/Health.png"));
    pearlKnight.initializeMovementLoops(textures);

    size_t slimeHeight = 128;
    size_t slimeWidth = 128;
    size_t slimeLoopSpeed = 7;
    size_t slimeScaling = 1;
    Vector2f slimePosition(500, groundHeight);
    SDL_Rect slimeFrame;
    slimeFrame.x = slimePosition.x, slimeFrame.y = slimePosition.y, slimeFrame.w = slimeWidth * slimeScaling, slimeFrame.h = slimeHeight * slimeScaling;
    
    Slime slime(slimePosition, slimeFrame, slimeWidth, slimeHeight, slimeLoopSpeed, movementSpeed);

    // Load and initialize slime textures.
    SDL_Texture* slimeIdleTexture = window.loadTexture("../graphics/enemies/slimes/Blue_Slime/Idle.png");
    SDL_Texture* slimeAttackTexture = nullptr; // window.loadTexture("../graphics/player/knight/Attacks.png");
    SDL_Texture* slimeRunningTexture = nullptr; // window.loadTexture("../graphics/player/knight/Run.png");
    slime.initializeMovementLoops(slimeIdleTexture, slimeAttackTexture, slimeRunningTexture);

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

                case SDLK_j:
                    if (!pearlKnight.currentlyAttacking()) pearlKnight.resetAttack0Frames();
                    pearlKnight.turnAttackingStatusOn();
                    break;
                
                case SDLK_k:
                    if (!pearlKnight.isHealing()) pearlKnight.startHealing();
                    break;
                
                case SDLK_d:
                    pearlKnight.move(window, true);
                    break;

                case SDLK_a:
                    pearlKnight.move(window, false);
                    break;
                
                case SDLK_w:
                    pearlKnight.startJump();
                    break;

                case SDLK_s:
                    pearlKnight.startCrouching();
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

                case SDLK_s:
                    pearlKnight.stopCrouching();
                    break;
                }
                break;
            }
        }

        if (pearlKnight.isMovingRight()) pearlKnight.move(window, true);
        if (pearlKnight.isMovingLeft()) pearlKnight.move(window, false);

        // Rendering.
        window.clearWindow();
        renderBackground(window, bg1, bg2, bg3);
        renderGround(window, tileTexture);
        slime.renderSlime(window);
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

void renderGround(RenderWindow& window, SDL_Texture* texture) {

    size_t groundHeight = 150;

    size_t numTiles = window.getWidth() / 32;

    for (size_t i = 0; i < numTiles; i++) {
        Vector2f tilePosition(i * 32, groundHeight);
        window.renderStatic(Entity(tilePosition, texture));
    }
}
