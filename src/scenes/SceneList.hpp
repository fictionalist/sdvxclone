#pragma once

#include "Scene.hpp"

enum class SceneList {
    Gameplay = 0,
};

class SceneGameplay : public Scene {
public:
    SceneGameplay();
    void update(unsigned int tickDelta);
};