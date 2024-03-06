#include "Enemy.hpp"
#include "FireWizard.hpp"

void createFireWizard(RenderWindow& window, int groundHeight, int posX) {
    Enemy* fireWizard = new Enemy(false, 300, 1, initializeInfo(150, 40, 1.5, Vector2i(128, 128), FireWizardHitbox, FireWizardAttackBox, 2, 5, groundHeight - FireWizardHitbox.h + 35, posX));

    // Load and initialize slime textures.
    // (start, N, rowLength, loopFrames)
    std::vector<std::tuple<SDL_Texture*, size_t, size_t, size_t, size_t, size_t>> textures;
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/wizard/Fire-wizard/Idle.png"), 0, 7, 7, 8, 0));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/wizard/Fire-wizard/Flame_jet.png"), 0, 14, 14, 4, 5));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/wizard/Fire-wizard/Walk.png"), 0, 6, 6, 4, 0));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/wizard/Fire-wizard/Hurt.png"), 0, 3, 3, 5, 0));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/wizard/Fire-wizard/Dead.png"), 0, 6, 6, 12, 0));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/death_effect.png"), 0, 8, 8, 3, 0));
    textures.push_back(std::make_tuple(window.loadTexture("../graphics/enemies/birth_effect.png"), 0, 8, 11, 3, 0));
    fireWizard->initializeMovementLoops(textures);
    enemies[fireWizard] = true;

    fireWizard->birthEffect.start();
}
