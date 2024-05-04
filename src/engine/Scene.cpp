#include "Scene.hpp"

Scene* Scene::currentScene = nullptr;

void Scene::setScene(SceneList s) {
    //printf("Setting scene\n");
    switch (s) {
        case SceneList::Gameplay:
        default:
            currentScene = new SceneGameplay();
            break;
    }
    //printf("Scene set: %X\n", currentScene);
}

Scene::Scene() {}

Scene::~Scene() {
    for (auto& obj : objectList) {
        delete obj;
    }
    objectList.clear();
    for (auto& element : interfaceList) {
        delete element;
    }
    interfaceList.clear();
}

void Scene::addInterface(Renderable* r) {
    interfaceList.push_back(r);
}

void Scene::addObject(Object* o) {
    objectList.push_back(o);
}

void Scene::update(unsigned int deltaTime) {
}

void Scene::draw() {
    for (auto& obj : objectList) {
        obj->draw();
    }
    for (auto& element : interfaceList) {
        element->draw();
    }
}