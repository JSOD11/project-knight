#include "Enemy.hpp"
#include "Skeleton.hpp"

void createSkeleton(RenderWindow& window, int groundHeight, int posX) {
    Enemy* skeleton = new Enemy(false, 150, initializeInfo(85, 20, 1.5, Vector2i(128, 128), skeletonHitbox, skeletonAttackBox, 2, 5, groundHeight - skeletonHitbox.h + 20, posX));

    // Load and initialize slime textures.
    // (start, N, rowLength, loopFrames)
    std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t>> textures;
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/skeleton/Skeleton_Warrior/Idle.png"), 0, 7, 7, 8));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/skeleton/Skeleton_Warrior/Attack_3.png"), 0, 4, 4, 6));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/skeleton/Skeleton_Warrior/Walk.png"), 0, 7, 7, 4));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/skeleton/Skeleton_Warrior/Hurt.png"), 0, 2, 2, 8));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/skeleton/Skeleton_Warrior/Dead.png"), 0, 4, 4, 10));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/death_effect.png"), 0, 8, 8, 3));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/birth_effect.png"), 0, 8, 11, 3));
    skeleton->initializeMovementLoops(textures);
    enemies[skeleton] = true;

    skeleton->birthEffect.start();
}

