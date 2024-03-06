#include "Enemy.hpp"
#include "Slime.hpp"

void createSlime(RenderWindow& window, std::string color, int groundHeight, int posX) {
    Enemy* slime = new Enemy(true, 50, 1, initializeInfo(55, 20, 1, Vector2i(128, 128), slimeHitbox, slimeAttackBox, 1, 5, groundHeight, posX));

    // Load and initialize slime textures.
    // (start, N, rowLength, loopFrames)
    std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t, size_t>> textures;
    textures.push_back(std::make_tuple(window.loadTexture(("../graphics/enemies/slimes/" + color + "_Slime/Idle.png").c_str()), 0, 8, 8, 8, 0));
    textures.push_back(std::make_tuple(window.loadTexture(("../graphics/enemies/slimes/" + color + "_Slime/Attack_3.png").c_str()), 0, 4, 4, 5, 3));
    textures.push_back(std::make_tuple(window.loadTexture(("../graphics/enemies/slimes/" + color + "_Slime/Run.png").c_str()), 0, 7, 7, 5, 0));
    textures.push_back(std::make_tuple(window.loadTexture(("../graphics/enemies/slimes/" + color + "_Slime/Hurt.png").c_str()), 0, 6, 6, 5, 0));
    textures.push_back(std::make_tuple(window.loadTexture(("../graphics/enemies/slimes/" + color + "_Slime/Dead.png").c_str()), 0, 3, 3, 7, 0));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/death_effect.png"), 0, 8, 8, 4, 0));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/birth_effect.png"), 0, 11, 11, 4, 0));
    slime->initializeMovementLoops(textures);
    enemies[slime] = true;

    slime->birthEffect.start();
}

