#pragma once

#include "Renderer.hpp"
#include "Renderable.hpp"
#include "UIElement.hpp"
#include "Input.hpp"

enum class SceneList;

class Scene {
public:
    static Scene* currentScene;
    static void setScene(SceneList);

    Scene();
    ~Scene();
    virtual void update(unsigned int deltaTime);
};

#include "../scenes/SceneList.hpp"