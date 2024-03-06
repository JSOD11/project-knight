#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <cstdlib>

#include "RenderWindow.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Slime.hpp"
#include "Skeleton.hpp"
#include "FireWizard.hpp"
#include "Block.hpp"
#include "Utils.hpp"

Player* knight;
std::map<Enemy*, bool> enemies;

void renderBackground(RenderWindow& window, std::vector<SDL_Texture*> bg);
void renderGround(RenderWindow& window, SDL_Texture* texture);

int main(int argc, char* argv[]) {

    (void) argc;
    (void) argv;

    if (SDL_Init(SDL_INIT_VIDEO) > 0 || !(IMG_Init(IMG_INIT_PNG))) {
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    RenderWindow window("Project Knight (Beta 0.0)", 1280, 720);
    
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

    std::vector<SDL_Texture*> hudTextures;
    for (size_t i = 0; i < 10; i++) {
        hudTextures.push_back(window.loadTexture(("../graphics/numbers/" + std::to_string(i) + ".png").c_str()));
    }
    hudTextures.push_back(window.loadTexture("../graphics/player/knight/HUD/bar_background.png"));
    hudTextures.push_back(window.loadTexture("../graphics/player/knight/HUD/bar.png"));
    hudTextures.push_back(window.loadTexture("../graphics/player/knight/HUD/health_bar.png"));
    hudTextures.push_back(window.loadTexture("../graphics/player/knight/HUD/HUDbox.png"));
    hudTextures.push_back(window.loadTexture("../graphics/player/knight/HUD/gems/gem.png"));
    hudTextures.push_back(window.loadTexture("../graphics/background/vignette_big.png"));
    hudTextures.push_back(window.loadTexture("../graphics/player/knight/HUD/corrupt_bar.png"));

    int groundHeight = (window.getHeight() - 2 * 64 - 110);

    createPlayer(window, groundHeight, 200);
    Player* player = knight;
    (void) player;

    // createSlime(window, "Blue", groundHeight, 500);
    // createSlime(window, "Blue", groundHeight, 700);
    // createSlime(window, "Red", groundHeight, 350);
    // createSlime(window, "Green", groundHeight, 800);

    // createSkeleton(window, groundHeight, 500);

    createFireWizard(window, groundHeight, 600);

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
            processInput(window, event, engineRunning);
        }

        // Rendering.
        window.clearWindow();

        // Render environment.
        renderBackground(window, bg);
        renderGround(window, tileTexture);

        // Render Characters.
        std::vector<Enemy*> deadEnemies;
        for (auto& pair : enemies) {
            Enemy* enemy = pair.first;
            enemy->renderEnemy(window);
            if (enemy->isDead()) deadEnemies.push_back(enemy);
        }
        for (auto& enemy : deadEnemies) {
            enemies.erase(enemy);
            free(enemy);
        }
        
        knight->renderPlayer(window);

        knight->renderHUD(window, hudTextures);

        window.display();

        if (std::rand() % 800 == 0) {
            std::vector<std::string> colors = { "Blue", "Red", "Green" };
            size_t index = std::rand() % 3;
            int position = std::rand() % window.getWidth();
            if (std::abs(position - knight->info.currentFrame.x) >= 50) createSlime(window, colors[index], groundHeight, position);
        }

        if (std::rand() % 1000 == 0) {
            int position = std::rand() % window.getWidth();
            createSkeleton(window, groundHeight, position);
        }

        if (std::rand() % 1200 == 0) {
            int position = std::rand() % window.getWidth();
            createFireWizard(window, groundHeight, position);
        }

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
