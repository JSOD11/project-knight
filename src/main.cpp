#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Character.hpp"
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
    size_t movementSpeed = 6;
    size_t jumpForce = 15;
    int groundHeight = window.getHeight() - sizeScaling * characterHeight - 110;
    Vector2f startingPosition(200, groundHeight);
    SDL_Rect startingFrame;
    startingFrame.x = startingPosition.x, startingFrame.y = startingPosition.y, startingFrame.w = characterWidth * sizeScaling, startingFrame.h = characterHeight * sizeScaling;

    Character knight(startingPosition, startingFrame, characterWidth, characterHeight, movementSpeed, jumpForce, groundHeight);


    // The tuple has form: (texture, startPosition, numElements, rowLength, loopFrames)

    // Load and initialize knight textures.
    std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t>> textures;
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Idle.png"), 0, 8, 2, 3));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Attacks.png"), 0, 7, 8, 3));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Run.png"), 0, 8, 2, 2));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/crouch_idle.png"), 0, 8, 2, 3));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Jump.png"), 0, 8, 2, 3));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Health.png"), 0, 8, 2, 5));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Pray.png"), 2, 10, 4, 5));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/attack_from_air.png"), 0, 3, 2, 3));
    knight.initializeMovementLoops(textures);

    size_t slimeHeight = 128;
    size_t slimeWidth = 128;
    size_t slimeScaling = 1;
    Vector2f slimePosition(500, groundHeight);
    SDL_Rect slimeFrame;
    slimeFrame.x = slimePosition.x, slimeFrame.y = slimePosition.y, slimeFrame.w = slimeWidth * slimeScaling, slimeFrame.h = slimeHeight * slimeScaling;
    
    Character slime(slimePosition, slimeFrame, slimeWidth, slimeHeight, 1, 0, groundHeight);

    // Load and initialize slime textures.
    std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t>> slimeTextures;
    slimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Blue_Slime/Idle.png"), 0, 8, 8, 8));
    slimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Blue_Slime/Attack_3.png"), 0, 4, 4, 5));
    slimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Blue_Slime/Run.png"), 0, 7, 7, 8));
    slimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Blue_Slime/Idle.png"), 0, 8, 8, 3));
    slimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Blue_Slime/Idle.png"), 0, 8, 8, 3));
    slimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Blue_Slime/Idle.png"), 0, 8, 8, 3));
    slimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Blue_Slime/Idle.png"), 0, 8, 8, 3));
    slimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Blue_Slime/Idle.png"), 0, 8, 8, 3));
    slime.initializeMovementLoops(slimeTextures);

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

                case SDLK_UP:
                    slime.startAttack();
                    break;

                case SDLK_j:
                    if (knight.isAirborne() && knight.crouch.isActive()) {
                        knight.airAttack.start();
                    } else {
                        knight.startAttack();
                    }
                    break;
                
                case SDLK_k:
                    if (!knight.heal.isActive()) knight.heal.start();
                    break;

                case SDLK_LEFT:
                    slime.startMovingLeft();
                    break;
                
                case SDLK_RIGHT:
                    slime.startMovingRight();
                    break;
                
                case SDLK_d:
                    knight.startMovingRight();
                    break;

                case SDLK_a:
                    knight.startMovingLeft();
                    break;
                
                case SDLK_w:
                    knight.startJump();
                    break;

                case SDLK_m:
                    knight.pray.start();
                    break;

                case SDLK_s:
                    knight.crouch.start();
                    break;
                }
                break;
            
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {

                case SDLK_LEFT:
                    slime.stopMovingLeft();
                    break;
                
                case SDLK_RIGHT:
                    slime.stopMovingRight();
                    break;

                case SDLK_a:
                    knight.stopMovingLeft();
                    break;

                case SDLK_d:
                    knight.stopMovingRight();
                    break;
                
                case SDLK_m:
                    knight.pray.stop();
                    break;

                case SDLK_s:
                    knight.crouch.stop();
                    break;
                }
                break;
            }
        }

        // Rendering.
        window.clearWindow();

        // Render environment.
        renderBackground(window, bg1, bg2, bg3);
        renderGround(window, tileTexture);

        // Render characters.
        slime.renderCharacter(window);
        knight.renderCharacter(window);

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
