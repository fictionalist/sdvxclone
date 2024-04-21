#include "Scene.hpp"

#include "UIElement.hpp"
#include "Model.hpp"
#include "Camera.hpp"

void buildTrack() {
    const float trackWidth = 12.0f;
    const float trackHeight = 40.0f;

    const unsigned int lanes = 6;
    const float offset = -(trackWidth / (((float)lanes)));
    const float laneWidth = trackWidth / ((float)lanes);
    const float buttonChipHeight = trackHeight / 40.0f;

    Texture* laneTexture = new Texture();
    laneTexture->loadImage("./data/textures/lane.png");
    Texture* buttonChipTexture = new Texture();
    buttonChipTexture->loadImage("./data/textures/button_chip.png");
    Texture* buttonChipKeysoundTexture = new Texture();
    buttonChipKeysoundTexture->loadImage("./data/textures/button_chip_keysound.png");
    Texture* fxChipTexture = new Texture();
    fxChipTexture->loadImage("./data/textures/fx_chip.png");
    Texture* fxChipKeysoundTexture = new Texture();
    fxChipKeysoundTexture->loadImage("./data/textures/fx_chip_keysound.png");

    for (unsigned int i = 0; i < lanes; i++) {
        Model* trackModel = new Model();
        trackModel->addVertex(Vertex({offset + (laneWidth * (float)i),       0.0f, -(trackHeight / 2.0f)}, {(1.0f / (float)lanes) * i,        1.0f}));
        trackModel->addVertex(Vertex({offset + (laneWidth * (float)(i + 1)), 0.0f, -(trackHeight / 2.0f)}, {(1.0f / (float)lanes) * (i + 1),  1.0f}));
        trackModel->addVertex(Vertex({offset + (laneWidth * (float)(i + 1)), 0.0f,  (trackHeight / 2.0f)}, {(1.0f / (float)lanes) * (i + 1),  0.0f}));
        trackModel->addVertex(Vertex({offset + (laneWidth * (float)i),       0.0f, -(trackHeight / 2.0f)}, {(1.0f / (float)lanes) * i,        1.0f}));
        trackModel->addVertex(Vertex({offset + (laneWidth * (float)(i + 1)), 0.0f,  (trackHeight / 2.0f)}, {(1.0f / (float)lanes) * (i + 1),  0.0f}));
        trackModel->addVertex(Vertex({offset + (laneWidth * (float)i),       0.0f,  (trackHeight / 2.0f)}, {(1.0f / (float)lanes) * i,        0.0f}));
        trackModel->buildModel();
        trackModel->setPosition(glm::vec3(-(trackWidth / 2.0f) + (2.0f * laneWidth), -4.0f, -30.0f));
        trackModel->setTexture(laneTexture);
        Renderer::addRenderable(trackModel);
    }

    Model* buttonChip = new Model();
    buttonChip->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
    buttonChip->addVertex(Vertex({laneWidth, 0.0f, 0.0f},             {1.0f, 0.0f}));
    buttonChip->addVertex(Vertex({laneWidth, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
    buttonChip->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
    buttonChip->addVertex(Vertex({laneWidth, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
    buttonChip->addVertex(Vertex({0.0f,      0.0f, buttonChipHeight}, {0.0f, 1.0f}));
    buttonChip->buildModel();
    buttonChip->setTexture(buttonChipTexture);
    buttonChip->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    Renderer::addRenderable(buttonChip);

    Model* buttonChipKeysound = new Model();
    buttonChipKeysound->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
    buttonChipKeysound->addVertex(Vertex({laneWidth, 0.0f, 0.0f},             {1.0f, 0.0f}));
    buttonChipKeysound->addVertex(Vertex({laneWidth, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
    buttonChipKeysound->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
    buttonChipKeysound->addVertex(Vertex({laneWidth, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
    buttonChipKeysound->addVertex(Vertex({0.0f,      0.0f, buttonChipHeight}, {0.0f, 1.0f}));
    buttonChipKeysound->buildModel();
    buttonChipKeysound->setTexture(buttonChipKeysoundTexture);
    buttonChipKeysound->setPosition(glm::vec3(laneWidth, 0.0f, 0.0f));
    Renderer::addRenderable(buttonChipKeysound);
    
    Model* fxChip = new Model();
    fxChip->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
    fxChip->addVertex(Vertex({laneWidth * 2.0f, 0.0f, 0.0f},             {1.0f, 0.0f}));
    fxChip->addVertex(Vertex({laneWidth * 2.0f, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
    fxChip->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
    fxChip->addVertex(Vertex({laneWidth * 2.0f, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
    fxChip->addVertex(Vertex({0.0f,      0.0f, buttonChipHeight}, {0.0f, 1.0f}));
    fxChip->buildModel();
    fxChip->setTexture(fxChipTexture);
    fxChip->setPosition(glm::vec3(0.0f, -0.1f, 0.0f));
    Renderer::addRenderable(fxChip);

    Model* fxChipKeysound = new Model();
    fxChipKeysound->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
    fxChipKeysound->addVertex(Vertex({laneWidth * 2.0f, 0.0f, 0.0f},             {1.0f, 0.0f}));
    fxChipKeysound->addVertex(Vertex({laneWidth * 2.0f, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
    fxChipKeysound->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
    fxChipKeysound->addVertex(Vertex({laneWidth * 2.0f, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
    fxChipKeysound->addVertex(Vertex({0.0f,      0.0f, buttonChipHeight}, {0.0f, 1.0f}));
    fxChipKeysound->buildModel();
    fxChipKeysound->setTexture(fxChipKeysoundTexture);
    fxChipKeysound->setPosition(glm::vec3(laneWidth * 2.0f, -0.1f, 0.0f));
    Renderer::addRenderable(fxChipKeysound);

    Camera::setPosition(glm::vec3(6.0f, 10.0f, -20.0f));
    Camera::lookAt(glm::vec3(6.0f, 0.0f, (trackWidth / 4.0f)));
}

void buildInterface() {
    Texture* lifeMeterTexture = new Texture();
    glm::ivec2 lifeMeterSize = lifeMeterTexture->loadImage("./data/textures/life_meter.png");
    UIElement* lifeMeter = new UIElement();
    lifeMeter->setSize(lifeMeterSize);
    lifeMeter->setPosition(glm::ivec2(1280 - 200, 720 - (lifeMeterSize.y)) - (lifeMeterSize.y / 4));
    lifeMeter->setTexture(lifeMeterTexture);
    Renderer::addRenderable(lifeMeter);

    UIElement* score = new UIElement();
    Texture* scoreTexture = new Texture();
    glm::ivec2 scoreSize = scoreTexture->setFontString(Renderer::defaultFont, "10000000");
    score->setSize(scoreSize);
    score->setTexture(scoreTexture);
    score->setPosition(glm::ivec2(1280 - scoreSize.x - 100, 80));
    Renderer::addRenderable(score);
}

SceneGameplay::SceneGameplay() {
    buildTrack();
    buildInterface();
}

void SceneGameplay::update(unsigned int tickDelta) {
}