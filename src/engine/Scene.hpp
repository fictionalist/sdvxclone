#pragma once

#include <vector>

#include "Renderer.hpp"
#include "Renderable.hpp"
#include "Audio.hpp"
#include "Object.hpp"
#include "UIElement.hpp"
#include "Input.hpp"

enum class SceneList;

class Scene {
protected:
    std::vector<Renderable*> interfaceList;
    std::vector<Object*> objectList;
public:
    static Scene* currentScene;
    static void setScene(SceneList);

    Scene();
    ~Scene();
    virtual void update(float deltaTime);
    virtual void draw();
    
    void addInterface(Renderable*);
    void addObject(Object*);
};

#include "../scenes/SceneList.hpp"