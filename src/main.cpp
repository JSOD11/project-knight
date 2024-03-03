#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Block.hpp"
#include "Utils.hpp"

Player* knight;
std::vector<Enemy*> enemies;

void renderBackground(RenderWindow& window, std::vector<SDL_Texture*> bg);
void renderGround(RenderWindow& window, SDL_Texture* texture);

Info initializeInfo(size_t sizeScaling, Vector2i pngSize, SDL_Rect hitbox, SDL_Rect attackBox, Vector2i centerCoordinates, size_t movementSpeed, size_t jumpForce, int groundHeight, size_t posX) {
    Info info = {
        .sizeScaling = sizeScaling,
        .position = Vector2i(posX, groundHeight),
        .pngSize = pngSize,
        .attackBox = attackBox,
        .movementSpeed = movementSpeed,
        .jumpForce = jumpForce,
        .groundHeight = groundHeight,

        .facingRight = true,
        .movingRight = false,
        .movingLeft = false,
        .velocityY = 0
    };
    info.currentFrame.x = info.position.x, info.currentFrame.y = info.position.y, info.currentFrame.w = info.pngSize.x * sizeScaling, info.currentFrame.h = info.pngSize.y * sizeScaling;
    info.hitbox.x = info.currentFrame.x + hitbox.x;
    info.hitbox.y = info.currentFrame.y + hitbox.y;
    info.hitbox.w = hitbox.w;
    info.hitbox.h = hitbox.h;
    info.centerCoordinates.x = info.position.x + centerCoordinates.x, info.centerCoordinates.y = info.position.y + centerCoordinates.y;

    return info;
}

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

    std::vector<SDL_Texture*> bg;
    bg.push_back(window.loadTexture("../graphics/background/cave/background1.png"));
    bg.push_back(window.loadTexture("../graphics/background/cave/background2.png"));
    bg.push_back(window.loadTexture("../graphics/background/cave/background3.png"));
    bg.push_back(window.loadTexture("../graphics/background/cave/background4a.png"));
    bg.push_back(window.loadTexture("../graphics/background/cave/background4b.png"));

    int groundHeight = (window.getHeight() - 2 * 64 - 110);
    
    Player* player = new Player(initializeInfo(2, Vector2i(128, 64), knightHitbox, knightAttackBox, knightCenterCoordinates, 6, 15, groundHeight, 200));
    knight = player;

    // The tuple has form: (texture, startPosition, numElements, rowLength, loopFrames)
    std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t>> textures;
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Idle.png"), 0, 8, 2, 3));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Attacks.png"), 0, 7, 8, 3));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Run.png"), 0, 8, 2, 2));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Hurt.png"), 0, 3, 2, 4));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/crouch_idle.png"), 0, 8, 2, 3));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Jump.png"), 0, 8, 2, 3));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Health.png"), 0, 8, 2, 5));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/Pray.png"), 2, 10, 4, 5));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/player/knight/attack_from_air.png"), 0, 3, 2, 3));
    knight->initializeMovementLoops(textures);


    Enemy* slime = new Enemy(initializeInfo(1, Vector2i(128, 128), slimeHitbox, slimeAttackBox, slimeCenterCoordinates, 1, 5, groundHeight, 500));

    // Load and initialize slime textures.
    std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t>> slimeTextures;
    slimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Blue_Slime/Idle.png"), 0, 8, 8, 8));
    slimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Blue_Slime/Attack_3.png"), 0, 4, 4, 5));
    slimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Blue_Slime/Run.png"), 0, 7, 7, 5));
    slimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Blue_Slime/Hurt.png"), 0, 6, 6, 5));
    slime->initializeMovementLoops(slimeTextures);
    enemies.push_back(slime);

    Enemy* redSlime = new Enemy(initializeInfo(1, Vector2i(128, 128), slimeHitbox, slimeAttackBox, slimeCenterCoordinates, 1, 5, groundHeight, 350));

    // Load and initialize slime textures.
    std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t>> redSlimeTextures;
    redSlimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Red_Slime/Idle.png"), 0, 8, 8, 8));
    redSlimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Red_Slime/Attack_3.png"), 0, 4, 4, 5));
    redSlimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Red_Slime/Run.png"), 0, 7, 7, 5));
    redSlimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Red_Slime/Hurt.png"), 0, 6, 6, 5));
    redSlime->initializeMovementLoops(redSlimeTextures);
    enemies.push_back(redSlime);

    Enemy* greenSlime = new Enemy(initializeInfo(1, Vector2i(128, 128), slimeHitbox, slimeAttackBox, slimeCenterCoordinates, 1, 5, groundHeight, 650));

    std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t>> greenSlimeTextures;
    greenSlimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Green_Slime/Idle.png"), 0, 8, 8, 8));
    greenSlimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Green_Slime/Attack_3.png"), 0, 4, 4, 5));
    greenSlimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Green_Slime/Run.png"), 0, 7, 7, 5));
    greenSlimeTextures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/slimes/Green_Slime/Hurt.png"), 0, 6, 6, 5));
    greenSlime->initializeMovementLoops(greenSlimeTextures);
    enemies.push_back(greenSlime);


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
                    slime->attack.start();
                    break;

                case SDLK_j:
                    if (knight->isAirborne() && knight->crouch.isActive()) {
                        knight->airAttack.start();
                    } else {
                        knight->startAttack();
                    }
                    break;
                
                case SDLK_k:
                    if (!knight->heal.isActive()) knight->heal.start();
                    break;

                case SDLK_LEFT:
                    slime->startMovingLeft();
                    break;
                
                case SDLK_RIGHT:
                    slime->startMovingRight();
                    break;
                
                case SDLK_d:
                    knight->startMovingRight();
                    break;

                case SDLK_a:
                    knight->startMovingLeft();
                    break;
                
                case SDLK_w:
                    knight->startJump();
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

                case SDLK_LEFT:
                    slime->stopMovingLeft();
                    break;
                
                case SDLK_RIGHT:
                    slime->stopMovingRight();
                    break;

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

        // Rendering.
        window.clearWindow();

        // Render environment.
        renderBackground(window, bg);
        renderGround(window, tileTexture);

        // Render Characters.
        for (Enemy*& enemy : enemies) {
            enemy->renderEnemy(window);
        }
        knight->renderPlayer(window);

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

void renderBackground(RenderWindow& window, std::vector<SDL_Texture*> bg) {
    for (SDL_Texture*& texture : bg) {
        SDL_RenderCopy(window.getRenderer(), texture, NULL, NULL);
    }
}

void renderGround(RenderWindow& window, SDL_Texture* texture) {

    size_t groundHeight = 150;

    size_t numTiles = window.getWidth() / 32;

    for (size_t i = 0; i < numTiles; i++) {
        Vector2i tilePosition(i * 32, groundHeight);
        window.renderStatic(Block(tilePosition, texture));
    }
}
