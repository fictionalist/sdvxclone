#include "Scene.hpp"

Scene* Scene::currentScene = nullptr;

void Scene::setScene(SceneList s) {
    switch (s) {
        case SceneList::Gameplay:
        default:
            currentScene = new SceneGameplay();
            break;
    }
}

Scene::Scene() {}

Scene::~Scene() {}

void Scene::update(unsigned int deltaTime) {
}