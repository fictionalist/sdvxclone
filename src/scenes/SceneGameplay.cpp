#include "Scene.hpp"

#include <cstdint>
#include <sstream>

#include "Logging.hpp"
#include "UIElement.hpp"
#include "Object.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "ChartReader.hpp"

const float trackWidth = 12.0f;
const float trackHeight = 60.0f;
const float laneHighlightHeight = -3.995;
const float measureDividerHeight = -3.9995;

const unsigned int errorWindow = 133;
const unsigned int nearWindow = 67;
const unsigned int critWindow = 33;
const unsigned int scritWindow = 16;

const glm::vec4 colorNoHit = glm::vec4(0.1f, 0.1f, 0.8f, 0.2f);
const glm::vec4 colorErrorHit = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
const glm::vec4 colorNearHit = glm::vec4(0.8f, 0.1f, 0.8f, 0.4f);
const glm::vec4 colorCritHit = glm::vec4(0.8f, 0.8f, 0.1f, 0.4f);
const glm::vec4 colorSCritHit = glm::vec4(1.0f, 1.0f, 1.0f, 0.8f);
const glm::vec4 colorLate = glm::vec4(1.0f, 0.4f, 0.4f, 0.9f);
const glm::vec4 colorEarly = glm::vec4(0.5f, 0.5f, 1.0f, 0.9f);
/*
unsigned int score = 0;
unsigned int previousScore = 0;
unsigned int exscore = 0;

float BPM = 220.0f;
float laneSpeed = 8.00f;
float basePosOffset = trackHeight * 6.0f;
float basePosition = (trackHeight / 4.0f) * laneSpeed;
float scrollSpeed = ((trackHeight / 4.0f) / 4.0f) * (BPM / 60.0f) * laneSpeed;]
const unsigned int expectedTime = (unsigned int)(BPM / 60.0f) * 1000;
*/

float progress = 0.0f;

void SceneGameplay::buildTrack() {
    const uint8_t lanes = 6;
    const uint8_t buttonLanes = 4;
    const uint8_t fxLanes = 2;

    const float laneWidth = trackWidth / ((float)lanes);
    const float buttonChipHeight = trackHeight / 40.0f;
    const float buttonChipHeightPos = -3.9f;

    laneTexture = new Texture();
    laneTexture->loadImage("./data/textures/lane.png");
    measureDividerTexture = new Texture();
    measureDividerTexture->loadImage("./data/textures/measure_divider.png");
    buttonChipTexture = new Texture();
    buttonChipTexture->loadImage("./data/textures/button_chip.png");
    buttonChipKeysoundTexture = new Texture();
    buttonChipKeysoundTexture->loadImage("./data/textures/button_chip_keysound.png");
    buttonLongTexture = new Texture();
    buttonLongTexture->loadImage("./data/textures/button_long.png");
    fxChipTexture = new Texture();
    fxChipTexture->loadImage("./data/textures/fx_chip.png");
    fxChipKeysoundTexture = new Texture();
    fxChipKeysoundTexture->loadImage("./data/textures/fx_chip_keysound.png");
    fxLongTexture = new Texture();
    fxLongTexture->loadImage("./data/textures/fx_long.png");

    uint8_t i = 0;
    trackLaneObjects.laneVolL = new Object();
    trackLaneObjects.laneVolL->model = new Model();
    trackLaneObjects.laneVolL->model->addVertex(Vertex({(laneWidth * (float)i),       0.0f, 0.0f},        {(1.0f / (float)lanes) * i,       1.0f}));
    trackLaneObjects.laneVolL->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, 0.0f},        {(1.0f / (float)lanes) * (i + 1), 1.0f}));
    trackLaneObjects.laneVolL->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, trackHeight}, {(1.0f / (float)lanes) * (i + 1), 0.0f}));
    trackLaneObjects.laneVolL->model->addVertex(Vertex({(laneWidth * (float)i),       0.0f, 0.0f},        {(1.0f / (float)lanes) * i,       1.0f}));
    trackLaneObjects.laneVolL->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, trackHeight}, {(1.0f / (float)lanes) * (i + 1), 0.0f}));
    trackLaneObjects.laneVolL->model->addVertex(Vertex({(laneWidth * (float)(i)),     0.0f, trackHeight}, {(1.0f / (float)lanes) * (i),     0.0f}));
    trackLaneObjects.laneVolL->model->buildModel();
    trackLaneObjects.laneVolL->setTexture(laneTexture);
    trackLaneObjects.laneVolL->setPosition(glm::vec3(-(trackWidth / 2.0f), -4.0f, -trackHeight));
    addObject(trackLaneObjects.laneVolL);

    i += 1;
    trackLaneObjects.laneA = new Object();
    trackLaneObjects.laneA->model = new Model();
    trackLaneObjects.laneA->model->addVertex(Vertex({(laneWidth * (float)i),       0.0f, 0.0f},        {(1.0f / (float)lanes) * i,       1.0f}));
    trackLaneObjects.laneA->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, 0.0f},        {(1.0f / (float)lanes) * (i + 1), 1.0f}));
    trackLaneObjects.laneA->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, trackHeight}, {(1.0f / (float)lanes) * (i + 1), 0.0f}));
    trackLaneObjects.laneA->model->addVertex(Vertex({(laneWidth * (float)i),       0.0f, 0.0f},        {(1.0f / (float)lanes) * i,       1.0f}));
    trackLaneObjects.laneA->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, trackHeight}, {(1.0f / (float)lanes) * (i + 1), 0.0f}));
    trackLaneObjects.laneA->model->addVertex(Vertex({(laneWidth * (float)(i)),     0.0f, trackHeight}, {(1.0f / (float)lanes) * (i),     0.0f}));
    trackLaneObjects.laneA->model->buildModel();
    trackLaneObjects.laneA->setTexture(laneTexture);
    trackLaneObjects.laneA->setPosition(glm::vec3(-(trackWidth / 2.0f), -4.0f, -trackHeight));
    addObject(trackLaneObjects.laneA);

    i += 1;
    trackLaneObjects.laneB = new Object();
    trackLaneObjects.laneB->model = new Model();
    trackLaneObjects.laneB->model->addVertex(Vertex({(laneWidth * (float)i),       0.0f, 0.0f},        {(1.0f / (float)lanes) * i,       1.0f}));
    trackLaneObjects.laneB->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, 0.0f},        {(1.0f / (float)lanes) * (i + 1), 1.0f}));
    trackLaneObjects.laneB->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, trackHeight}, {(1.0f / (float)lanes) * (i + 1), 0.0f}));
    trackLaneObjects.laneB->model->addVertex(Vertex({(laneWidth * (float)i),       0.0f, 0.0f},        {(1.0f / (float)lanes) * i,       1.0f}));
    trackLaneObjects.laneB->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, trackHeight}, {(1.0f / (float)lanes) * (i + 1), 0.0f}));
    trackLaneObjects.laneB->model->addVertex(Vertex({(laneWidth * (float)(i)),     0.0f, trackHeight}, {(1.0f / (float)lanes) * (i),     0.0f}));
    trackLaneObjects.laneB->model->buildModel();
    trackLaneObjects.laneB->setTexture(laneTexture);
    trackLaneObjects.laneB->setPosition(glm::vec3(-(trackWidth / 2.0f), -4.0f, -trackHeight));
    addObject(trackLaneObjects.laneB);

    i += 1;
    trackLaneObjects.laneC = new Object();
    trackLaneObjects.laneC->model = new Model();
    trackLaneObjects.laneC->model->addVertex(Vertex({(laneWidth * (float)i),       0.0f, 0.0f},        {(1.0f / (float)lanes) * i,       1.0f}));
    trackLaneObjects.laneC->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, 0.0f},        {(1.0f / (float)lanes) * (i + 1), 1.0f}));
    trackLaneObjects.laneC->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, trackHeight}, {(1.0f / (float)lanes) * (i + 1), 0.0f}));
    trackLaneObjects.laneC->model->addVertex(Vertex({(laneWidth * (float)i),       0.0f, 0.0f},        {(1.0f / (float)lanes) * i,       1.0f}));
    trackLaneObjects.laneC->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, trackHeight}, {(1.0f / (float)lanes) * (i + 1), 0.0f}));
    trackLaneObjects.laneC->model->addVertex(Vertex({(laneWidth * (float)(i)),     0.0f, trackHeight}, {(1.0f / (float)lanes) * (i),     0.0f}));
    trackLaneObjects.laneC->model->buildModel();
    trackLaneObjects.laneC->setTexture(laneTexture);
    trackLaneObjects.laneC->setPosition(glm::vec3(-(trackWidth / 2.0f), -4.0f, -trackHeight));
    addObject(trackLaneObjects.laneC);

    i += 1;
    trackLaneObjects.laneD = new Object();
    trackLaneObjects.laneD->model = new Model();
    trackLaneObjects.laneD->model->addVertex(Vertex({(laneWidth * (float)i),       0.0f, 0.0f},        {(1.0f / (float)lanes) * i,       1.0f}));
    trackLaneObjects.laneD->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, 0.0f},        {(1.0f / (float)lanes) * (i + 1), 1.0f}));
    trackLaneObjects.laneD->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, trackHeight}, {(1.0f / (float)lanes) * (i + 1), 0.0f}));
    trackLaneObjects.laneD->model->addVertex(Vertex({(laneWidth * (float)i),       0.0f, 0.0f},        {(1.0f / (float)lanes) * i,       1.0f}));
    trackLaneObjects.laneD->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, trackHeight}, {(1.0f / (float)lanes) * (i + 1), 0.0f}));
    trackLaneObjects.laneD->model->addVertex(Vertex({(laneWidth * (float)(i)),     0.0f, trackHeight}, {(1.0f / (float)lanes) * (i),     0.0f}));
    trackLaneObjects.laneD->model->buildModel();
    trackLaneObjects.laneD->setTexture(laneTexture);
    trackLaneObjects.laneD->setPosition(glm::vec3(-(trackWidth / 2.0f), -4.0f, -trackHeight));
    addObject(trackLaneObjects.laneD);

    i += 1;
    trackLaneObjects.laneVolR = new Object();
    trackLaneObjects.laneVolR->model = new Model();
    trackLaneObjects.laneVolR->model->addVertex(Vertex({(laneWidth * (float)i),       0.0f, 0.0f},        {(1.0f / (float)lanes) * i,       1.0f}));
    trackLaneObjects.laneVolR->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, 0.0f},        {(1.0f / (float)lanes) * (i + 1), 1.0f}));
    trackLaneObjects.laneVolR->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, trackHeight}, {(1.0f / (float)lanes) * (i + 1), 0.0f}));
    trackLaneObjects.laneVolR->model->addVertex(Vertex({(laneWidth * (float)i),       0.0f, 0.0f},        {(1.0f / (float)lanes) * i,       1.0f}));
    trackLaneObjects.laneVolR->model->addVertex(Vertex({(laneWidth * (float)(i + 1)), 0.0f, trackHeight}, {(1.0f / (float)lanes) * (i + 1), 0.0f}));
    trackLaneObjects.laneVolR->model->addVertex(Vertex({(laneWidth * (float)(i)),     0.0f, trackHeight}, {(1.0f / (float)lanes) * (i),     0.0f}));
    trackLaneObjects.laneVolR->model->buildModel();
    trackLaneObjects.laneVolR->setTexture(laneTexture);
    trackLaneObjects.laneVolR->setPosition(glm::vec3(-(trackWidth / 2.0f), -4.0f, -trackHeight));
    addObject(trackLaneObjects.laneVolR);

    laneHighlightButton = new Model();
    laneHighlightButton->addVertex(Vertex({0.0f,      0.0f, 0.0f},        {0.0f, 0.0f}));
    laneHighlightButton->addVertex(Vertex({laneWidth, 0.0f, 0.0f},        {1.0f, 0.0f}));
    laneHighlightButton->addVertex(Vertex({laneWidth, 0.0f, trackHeight}, {1.0f, 1.0f}));
    laneHighlightButton->addVertex(Vertex({0.0f,      0.0f, 0.0f},        {0.0f, 0.0f}));
    laneHighlightButton->addVertex(Vertex({laneWidth, 0.0f, trackHeight}, {1.0f, 1.0f}));
    laneHighlightButton->addVertex(Vertex({0.0f,      0.0f, trackHeight}, {0.0f, 1.0f}));
    laneHighlightButton->buildModel();

    laneHighlightFX = new Model();
    laneHighlightFX->addVertex(Vertex({0.0f,             0.0f, 0.0f},        {0.0f, 0.0f}));
    laneHighlightFX->addVertex(Vertex({laneWidth * 2.0f, 0.0f, 0.0f},        {1.0f, 0.0f}));
    laneHighlightFX->addVertex(Vertex({laneWidth * 2.0f, 0.0f, trackHeight}, {1.0f, 1.0f}));
    laneHighlightFX->addVertex(Vertex({0.0f,             0.0f, 0.0f},        {0.0f, 0.0f}));
    laneHighlightFX->addVertex(Vertex({laneWidth * 2.0f, 0.0f, trackHeight}, {1.0f, 1.0f}));
    laneHighlightFX->addVertex(Vertex({0.0f,             0.0f, trackHeight}, {0.0f, 1.0f}));
    laneHighlightFX->buildModel();

    i = 1;
    laneHighlights.laneA = new Object();
    laneHighlights.laneA->model = laneHighlightButton;
    laneHighlights.laneA->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * (float)i), laneHighlightHeight, -(trackHeight)));
    laneHighlights.laneA->setVisibility(false);
    addObject(laneHighlights.laneA);

    i += 1;
    laneHighlights.laneB = new Object();
    laneHighlights.laneB->model = laneHighlightButton;
    laneHighlights.laneB->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * (float)i), laneHighlightHeight, -(trackHeight)));
    laneHighlights.laneB->setVisibility(false);
    addObject(laneHighlights.laneB);

    i += 1;
    laneHighlights.laneC = new Object();
    laneHighlights.laneC->model = laneHighlightButton;
    laneHighlights.laneC->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * (float)i), laneHighlightHeight, -(trackHeight)));
    laneHighlights.laneC->setVisibility(false);
    addObject(laneHighlights.laneC);

    i += 1;
    laneHighlights.laneD = new Object();
    laneHighlights.laneD->model = laneHighlightButton;
    laneHighlights.laneD->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * (float)i), laneHighlightHeight, -(trackHeight)));
    laneHighlights.laneD->setVisibility(false);
    addObject(laneHighlights.laneD);

    i = 0;
    laneHighlights.laneFXL = new Object();
    laneHighlights.laneFXL->model = laneHighlightFX;
    laneHighlights.laneFXL->setPosition(glm::vec3(-(trackWidth / 2.0f) + laneWidth + (laneWidth * (float)i * 2.0f), laneHighlightHeight, -(trackHeight)));
    laneHighlights.laneFXL->setVisibility(false);
    addObject(laneHighlights.laneFXL);

    i += 1;
    laneHighlights.laneFXR = new Object();
    laneHighlights.laneFXR->model = laneHighlightFX;
    laneHighlights.laneFXR->setPosition(glm::vec3(-(trackWidth / 2.0f) + laneWidth + (laneWidth * (float)i * 2.0f), laneHighlightHeight, -(trackHeight)));
    laneHighlights.laneFXR->setVisibility(false);
    addObject(laneHighlights.laneFXR);

    laneHighlightATimer = 0;
    laneHighlightBTimer = 0;
    laneHighlightCTimer = 0;
    laneHighlightDTimer = 0;
    laneHighlightFXLTimer = 0;
    laneHighlightFXRTimer = 0;

    buttonADebounce = 0;
    buttonBDebounce = 0;
    buttonCDebounce = 0;
    buttonDDebounce = 0;
    buttonFXLDebounce = 0;
    buttonFXRDebounce = 0;

}

void SceneGameplay::buildNotes(ChartData* data) {
    // currently a quick attempt at getting notes on the screen
    // SHOULD be optimized later to just reuse a few models and redraw it on different positions
    float currentPosition = -basePosition;
    const float defaultMeasureWidth = (trackHeight / 4.0f) * laneSpeed;
    /*for (auto& measure : data->measures) {
        float beatHeight = defaultMeasureWidth / measure->beats.size();
        for (auto& beat : measure->beats) {
            if (beat->A == NoteType::Chip) {
                Model* noteModel = new Model();
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, 0.0f},             {1.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, buttonChipHeight}, {0.0f, 1.0f}));
                noteModel->buildModel();
                noteModel->setTexture(buttonChipTexture);
                noteModel->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 1), buttonChipHeightPos, currentPosition));
                Renderer::addRenderable(noteModel);
                noteList.push_back(noteModel);
            } else if (beat->A == NoteType::Hold) {
                Model* noteModel = new Model();
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},      {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, 0.0f},      {1.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, beatHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},      {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, beatHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, beatHeight}, {0.0f, 1.0f}));
                noteModel->buildModel();
                noteModel->setTexture(buttonLongTexture);
                noteModel->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 1), buttonChipHeightPos, currentPosition));
                Renderer::addRenderable(noteModel);
                noteList.push_back(noteModel);
            }
            if (beat->B == NoteType::Chip) {
                Model* noteModel = new Model();
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, 0.0f},             {1.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, buttonChipHeight}, {0.0f, 1.0f}));
                noteModel->buildModel();
                noteModel->setTexture(buttonChipTexture);
                noteModel->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 2), buttonChipHeightPos, currentPosition));
                Renderer::addRenderable(noteModel);
                noteList.push_back(noteModel);
            } else if (beat->B == NoteType::Hold) {
                Model* noteModel = new Model();
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},      {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, 0.0f},      {1.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, beatHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},      {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, beatHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, beatHeight}, {0.0f, 1.0f}));
                noteModel->buildModel();
                noteModel->setTexture(buttonLongTexture);
                noteModel->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 2), buttonChipHeightPos, currentPosition));
                Renderer::addRenderable(noteModel);
                noteList.push_back(noteModel);
            }
            if (beat->C == NoteType::Chip) {
                Model* noteModel = new Model();
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, 0.0f},             {1.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, buttonChipHeight}, {0.0f, 1.0f}));
                noteModel->buildModel();
                noteModel->setTexture(buttonChipTexture);
                noteModel->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 3), buttonChipHeightPos, currentPosition));
                Renderer::addRenderable(noteModel);
                noteList.push_back(noteModel);
            } else if (beat->C == NoteType::Hold) {
                Model* noteModel = new Model();
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},      {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, 0.0f},      {1.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, beatHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},      {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, beatHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, beatHeight}, {0.0f, 1.0f}));
                noteModel->buildModel();
                noteModel->setTexture(buttonLongTexture);
                noteModel->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 3), buttonChipHeightPos, currentPosition));
                Renderer::addRenderable(noteModel);
                noteList.push_back(noteModel);
            }
            if (beat->D == NoteType::Chip) {
                Model* noteModel = new Model();
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, 0.0f},             {1.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, buttonChipHeight}, {0.0f, 1.0f}));
                noteModel->buildModel();
                noteModel->setTexture(buttonChipTexture);
                noteModel->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 4), buttonChipHeightPos, currentPosition));
                Renderer::addRenderable(noteModel);
                noteList.push_back(noteModel);
            } else if (beat->D == NoteType::Hold) {
                Model* noteModel = new Model();
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},      {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, 0.0f},      {1.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, beatHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},      {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth, 0.0f, beatHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, beatHeight}, {0.0f, 1.0f}));
                noteModel->buildModel();
                noteModel->setTexture(buttonLongTexture);
                noteModel->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 4), buttonChipHeightPos, currentPosition));
                Renderer::addRenderable(noteModel);
                noteList.push_back(noteModel);
            }
            if (beat->FXL == NoteType::Chip) {
                Model* noteModel = new Model();
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth * 2.0f, 0.0f, 0.0f},             {1.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth * 2.0f, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth * 2.0f, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, buttonChipHeight}, {0.0f, 1.0f}));
                noteModel->buildModel();
                noteModel->setTexture(fxChipTexture);
                noteModel->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 1), buttonChipHeightPos, currentPosition));
                Renderer::addRenderable(noteModel);
                noteList.push_back(noteModel);
            } else if (beat->FXL == NoteType::Hold) {
                Model* noteModel = new Model();
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth * 2.0f, 0.0f, 0.0f},             {1.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth * 2.0f, 0.0f, beatHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth * 2.0f, 0.0f, beatHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, beatHeight}, {0.0f, 1.0f}));
                noteModel->buildModel();
                noteModel->setTexture(fxLongTexture);
                noteModel->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 1), buttonChipHeightPos, currentPosition));
                Renderer::addRenderable(noteModel);
                noteList.push_back(noteModel);
            }
            if (beat->FXR == NoteType::Chip) {
                Model* noteModel = new Model();
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth * 2.0f, 0.0f, 0.0f},             {1.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth * 2.0f, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth * 2.0f, 0.0f, buttonChipHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, buttonChipHeight}, {0.0f, 1.0f}));
                noteModel->buildModel();
                noteModel->setTexture(fxChipTexture);
                noteModel->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 3), buttonChipHeightPos, currentPosition));
                Renderer::addRenderable(noteModel);
                noteList.push_back(noteModel);
            } else if (beat->FXR == NoteType::Hold) {
                Model* noteModel = new Model();
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth * 2.0f, 0.0f, 0.0f},             {1.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth * 2.0f, 0.0f, beatHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, 0.0f},             {0.0f, 0.0f}));
                noteModel->addVertex(Vertex({laneWidth * 2.0f, 0.0f, beatHeight}, {1.0f, 1.0f}));
                noteModel->addVertex(Vertex({0.0f,      0.0f, beatHeight}, {0.0f, 1.0f}));
                noteModel->buildModel();
                noteModel->setTexture(fxLongTexture);
                noteModel->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 3), buttonChipHeightPos, currentPosition));
                Renderer::addRenderable(noteModel);
                noteList.push_back(noteModel);
            }
            currentPosition -= beatHeight;
        }
    }*/
}

void SceneGameplay::buildButtonDisplay() {
    UIElement* controllerDisplayBackground = new UIElement();
    controllerDisplayBackground->setSize(glm::ivec2(150, 80));
    controllerDisplayBackground->setPosition(glm::ivec2(40, 40));
    controllerDisplayBackground->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.1f));
    addInterface(controllerDisplayBackground);

    buttonDisplayElements.buttonA = new UIElement();
    buttonDisplayElements.buttonA->setSize(glm::ivec2(20, 20));
    buttonDisplayElements.buttonA->setPosition(glm::ivec2(60, 68));
    buttonDisplayElements.buttonA->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    addInterface(buttonDisplayElements.buttonA);

    buttonDisplayElements.buttonB = new UIElement();
    buttonDisplayElements.buttonB->setSize(glm::ivec2(20, 20));
    buttonDisplayElements.buttonB->setPosition(glm::ivec2(90, 68));
    buttonDisplayElements.buttonB->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    addInterface(buttonDisplayElements.buttonB);

    buttonDisplayElements.buttonC = new UIElement();
    buttonDisplayElements.buttonC->setSize(glm::ivec2(20, 20));
    buttonDisplayElements.buttonC->setPosition(glm::ivec2(120, 68));
    buttonDisplayElements.buttonC->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    addInterface(buttonDisplayElements.buttonC);

    buttonDisplayElements.buttonD = new UIElement();
    buttonDisplayElements.buttonD->setSize(glm::ivec2(20, 20));
    buttonDisplayElements.buttonD->setPosition(glm::ivec2(150, 68));
    buttonDisplayElements.buttonD->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    addInterface(buttonDisplayElements.buttonD);

    buttonDisplayElements.buttonFXL = new UIElement();
    buttonDisplayElements.buttonFXL->setSize(glm::ivec2(20, 10));
    buttonDisplayElements.buttonFXL->setPosition(glm::ivec2(75, 94));
    buttonDisplayElements.buttonFXL->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    addInterface(buttonDisplayElements.buttonFXL);

    buttonDisplayElements.buttonFXR = new UIElement();
    buttonDisplayElements.buttonFXR->setSize(glm::ivec2(20, 10));
    buttonDisplayElements.buttonFXR->setPosition(glm::ivec2(135, 94));
    buttonDisplayElements.buttonFXR->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    addInterface(buttonDisplayElements.buttonFXR);
}

void SceneGameplay::buildJudgmentCounters(glm::ivec2 windowResolution) {
    UIElement* judgmentCounterBackground = new UIElement();
    judgmentCounterBackground->setSize(glm::ivec2(200));
    judgmentCounterBackground->setPosition(glm::ivec2(windowResolution.x - 200, (windowResolution.y / 2)));
    judgmentCounterBackground->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.1f));
    addInterface(judgmentCounterBackground);

    glm::ivec2 labelSize;

    Texture* labelEarlyErrors = new Texture();
    labelEarlyErrors->setFontString(Renderer::defaultFont, "ERROR-");
    Texture* labelEarlyNears = new Texture();
    labelEarlyNears->setFontString(Renderer::defaultFont, "NEAR-");
    Texture* labelEarlyCrits = new Texture();
    labelEarlyCrits->setFontString(Renderer::defaultFont, "CRITICAL-");
    Texture* labelSCrits = new Texture();
    labelSize = labelSCrits->setFontString(Renderer::defaultFont, "S-CRITICAL");
    Texture* labelLateCrits = new Texture();
    labelLateCrits->setFontString(Renderer::defaultFont, "CRITICAL+");
    Texture* labelLateNears = new Texture();
    labelLateNears->setFontString(Renderer::defaultFont, "NEAR+");
    Texture* labelLateErrors = new Texture();
    labelLateErrors->setFontString(Renderer::defaultFont, "ERROR+");

    labelSize = labelSize * 2 / 3;

    UIElement* judgmentLabelEarlyErrors = new UIElement();
    judgmentLabelEarlyErrors->setSize(labelSize);
    judgmentLabelEarlyErrors->setPosition(glm::ivec2(windowResolution.x - 200, (windowResolution.y / 2)));
    judgmentLabelEarlyErrors->setTexture(labelEarlyErrors);
    addInterface(judgmentLabelEarlyErrors);

    UIElement* judgmentLabelEarlyNears = new UIElement();
    judgmentLabelEarlyNears->setSize(labelSize);
    judgmentLabelEarlyNears->setPosition(glm::ivec2(windowResolution.x - 200, (windowResolution.y / 2) + labelSize.y));
    judgmentLabelEarlyNears->setTexture(labelEarlyNears);
    addInterface(judgmentLabelEarlyNears);

    UIElement* judgmentLabelEarlyCrits = new UIElement();
    judgmentLabelEarlyCrits->setSize(labelSize);
    judgmentLabelEarlyCrits->setPosition(glm::ivec2(windowResolution.x - 200, (windowResolution.y / 2) + (labelSize.y * 2)));
    judgmentLabelEarlyCrits->setTexture(labelEarlyCrits);
    addInterface(judgmentLabelEarlyCrits);

    UIElement* judgmentLabelSCrits = new UIElement();
    judgmentLabelSCrits->setSize(labelSize);
    judgmentLabelSCrits->setPosition(glm::ivec2(windowResolution.x - 200, (windowResolution.y / 2) + (labelSize.y * 3)));
    judgmentLabelSCrits->setTexture(labelSCrits);
    addInterface(judgmentLabelSCrits);

    UIElement* judgmentLabelLateCrits = new UIElement();
    judgmentLabelLateCrits->setSize(labelSize);
    judgmentLabelLateCrits->setPosition(glm::ivec2(windowResolution.x - 200, (windowResolution.y / 2) + (labelSize.y * 4)));
    judgmentLabelLateCrits->setTexture(labelLateCrits);
    addInterface(judgmentLabelLateCrits);

    UIElement* judgmentLabelLateNears = new UIElement();
    judgmentLabelLateNears->setSize(labelSize);
    judgmentLabelLateNears->setPosition(glm::ivec2(windowResolution.x - 200, (windowResolution.y / 2) + (labelSize.y * 5)));
    judgmentLabelLateNears->setTexture(labelLateNears);
    addInterface(judgmentLabelLateNears);

    UIElement* judgmentLabelLateErrors = new UIElement();
    judgmentLabelLateErrors->setSize(labelSize);
    judgmentLabelLateErrors->setPosition(glm::ivec2(windowResolution.x - 200, (windowResolution.y / 2) + (labelSize.y * 6)));
    judgmentLabelLateErrors->setTexture(labelLateErrors);
    addInterface(judgmentLabelLateErrors);

}

void SceneGameplay::buildJudgmentDisplay(glm::ivec2 windowResolution) {
    lateDisplay = new UIElement();
    Texture* lateDisplayTexture = new Texture();
    glm::ivec2 lateDisplaySize = lateDisplayTexture->setFontString(Renderer::defaultFont, "LATE");
    lateDisplay->setTexture(lateDisplayTexture);
    lateDisplay->setSize(lateDisplaySize);
    lateDisplay->setPosition(glm::ivec2((windowResolution.x / 2) - (lateDisplaySize.x / 2) - (lateDisplaySize.x), (windowResolution.y / 2) - lateDisplaySize.y / 2));
    lateDisplay->setColor(colorLate);
    lateDisplay->setVisibility(false);
    addInterface(lateDisplay);

    earlyDisplay = new UIElement();
    Texture* earlyDisplayTexture = new Texture();
    glm::ivec2 earlyDisplaySize = earlyDisplayTexture->setFontString(Renderer::defaultFont, "EARLY");
    earlyDisplay->setTexture(earlyDisplayTexture);
    earlyDisplay->setSize(earlyDisplaySize);
    earlyDisplay->setPosition(glm::ivec2((windowResolution.x / 2) - (earlyDisplaySize.x / 2) - (earlyDisplaySize.x), (windowResolution.y / 2) - earlyDisplaySize.y / 2));
    earlyDisplay->setColor(colorEarly);
    earlyDisplay->setVisibility(false);
    addInterface(earlyDisplay);
    
    hitDeltaDisplay = new UIElement();
    hitDeltaTexture = new Texture();
    glm::ivec2 hitDeltaTextureSize = hitDeltaTexture->setFontString(Renderer::defaultFont, "0 ms");
    hitDeltaDisplay->setTexture(hitDeltaTexture);
    hitDeltaDisplay->setSize(hitDeltaTextureSize);
    hitDeltaDisplay->setPosition(glm::ivec2((Renderer::getCurrentResolution().x / 2) + (hitDeltaTextureSize.x / 2), (Renderer::getCurrentResolution().y / 2) - (hitDeltaTextureSize.y / 2)));
    hitDeltaDisplay->setColor(colorEarly);
    hitDeltaDisplay->setVisibility(false);
    addInterface(hitDeltaDisplay);
}

void SceneGameplay::buildInterface() {
    glm::ivec2 windowResolution = Renderer::getCurrentResolution();

    scoreDisplay = new UIElement();
    scoreTexture = new Texture();
    glm::ivec2 scoreSize = scoreTexture->setFontString(Renderer::defaultFont, "0");
    scoreDisplay->setSize(scoreSize);
    scoreDisplay->setTexture(scoreTexture);
    scoreDisplay->setPosition(glm::ivec2(windowResolution.x - scoreSize.x - 100, 80));
    addInterface(scoreDisplay);
    
    Texture* lifeMeterTexture = new Texture();
    glm::ivec2 lifeMeterSize = lifeMeterTexture->loadImage("./data/textures/life_meter.png");
    UIElement* lifeMeter = new UIElement();
    lifeMeter->setSize(lifeMeterSize);
    lifeMeter->setPosition(glm::ivec2(windowResolution.x - 200, windowResolution.y - (lifeMeterSize.y)) - (lifeMeterSize.y / 4));
    lifeMeter->setTexture(lifeMeterTexture);
    addInterface(lifeMeter);

    buildButtonDisplay();
    buildJudgmentDisplay(windowResolution);
    buildJudgmentCounters(windowResolution);

    fadeIn = new UIElement();
    fadeIn->setSize(windowResolution);
    fadeIn->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    fadeIn->setPosition(glm::ivec2(0));
    fadeIn->setVisibility(true);
    addInterface(fadeIn);
}

SceneGameplay::SceneGameplay() {
    std::optional<ChartData*> data = ChartReader::readFromFile("./songs/test/absurd_gaff_metalize_mxm.ksh");
    buildTrack();
    buildNotes(data.value_or(new ChartData()));
    buildInterface();
    
    Camera::setPosition(glm::vec3(0.0f, 15.0f, 25.0f));
    Camera::lookAt(glm::vec3(0.0f, 0.0f, -20.0f));

    fadeInTimer = fadeInTimerMax;
}

void SceneGameplay::updateInputDisplay(InputState state) {
    if (state.bits.buttonA) {
        buttonDisplayElements.buttonA->setColor(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));
    } else {
        buttonDisplayElements.buttonA->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    }
    if (state.bits.buttonB) {
        buttonDisplayElements.buttonB->setColor(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));
    } else {
        buttonDisplayElements.buttonB->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    }
    if (state.bits.buttonC) {
        buttonDisplayElements.buttonC->setColor(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));
    } else {
        buttonDisplayElements.buttonC->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    }
    if (state.bits.buttonD) {
        buttonDisplayElements.buttonD->setColor(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));
    } else {
        buttonDisplayElements.buttonD->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    }
    if (state.bits.buttonFXL) {
        buttonDisplayElements.buttonFXL->setColor(glm::vec4(0.8f, 0.0f, 0.0f, 1.0f));
    } else {
        buttonDisplayElements.buttonFXL->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    }
    if (state.bits.buttonFXR) {
        buttonDisplayElements.buttonFXR->setColor(glm::vec4(0.8f, 0.0f, 0.0f, 1.0f));
    } else {
        buttonDisplayElements.buttonFXR->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    }
}

void SceneGameplay::tickDownTimers(unsigned int tickDelta) {
    if (laneHighlightATimer > tickDelta) {
        laneHighlightATimer -= tickDelta;
    } else {
        laneHighlightATimer = 0;
    }
    if (laneHighlightBTimer > tickDelta) {
        laneHighlightBTimer -= tickDelta;
    } else {
        laneHighlightBTimer = 0;
    }
    if (laneHighlightCTimer > tickDelta) {
        laneHighlightCTimer -= tickDelta;
    } else {
        laneHighlightCTimer = 0;
    }
    if (laneHighlightDTimer > tickDelta) {
        laneHighlightDTimer -= tickDelta;
    } else {
        laneHighlightDTimer = 0;
    }
    if (laneHighlightFXLTimer > tickDelta) {
        laneHighlightFXLTimer -= tickDelta;
    } else {
        laneHighlightFXLTimer = 0;
    }
    if (laneHighlightFXRTimer > tickDelta) {
        laneHighlightFXRTimer -= tickDelta;
    } else {
        laneHighlightFXRTimer = 0;
    }

    if (earlyIndicatorTimer > tickDelta) {
        earlyIndicatorTimer -= tickDelta;
    } else {
        earlyIndicatorTimer = 0;
    }

    if (lateIndicatorTimer > tickDelta) {
        lateIndicatorTimer -= tickDelta;
    } else {
        lateIndicatorTimer = 0;
    }

    if (hitDeltaDisplayTimer > tickDelta) {
        hitDeltaDisplayTimer -= tickDelta;
    } else {
        hitDeltaDisplayTimer = 0;
    }

    if (buttonADebounce > tickDelta) {
        buttonADebounce -= tickDelta;
    } else {
        buttonADebounce = 0;
    }
}

void SceneGameplay::updateLaneHighlightVisibility(InputState state) {
    if (laneHighlightATimer == 0) {
        laneHighlights.laneA->setVisibility(false);
    }
    if (laneHighlightBTimer == 0) {
        laneHighlights.laneB->setVisibility(false);
    }
    if (laneHighlightCTimer == 0) {
        laneHighlights.laneC->setVisibility(false);
    }
    if (laneHighlightDTimer == 0) {
        laneHighlights.laneD->setVisibility(false);
    }
    if (laneHighlightFXLTimer == 0) {
        laneHighlights.laneFXL->setVisibility(false);
    }
    if (laneHighlightFXRTimer == 0) {
        laneHighlights.laneFXR->setVisibility(false);
    }

    if (state.bits.buttonA) {
        laneHighlightATimer = 33;
        laneHighlights.laneA->setVisibility(true);
    }

    if (lateIndicatorTimer == 0) {
        lateDisplay->setVisibility(false);
    }
    if (earlyIndicatorTimer == 0) {
        earlyDisplay->setVisibility(false);
    }
    if (hitDeltaDisplayTimer == 0) {
        hitDeltaDisplay->setVisibility(false);
    }

    if (state.bits.buttonB) {
        laneHighlightBTimer = 33;
        laneHighlights.laneB->setVisibility(true);
    }
    if (state.bits.buttonC) {
        laneHighlightCTimer = 33;
        laneHighlights.laneC->setVisibility(true);
    }
    if (state.bits.buttonD) {
        laneHighlightDTimer = 33;
        laneHighlights.laneD->setVisibility(true);
    }
    if (state.bits.buttonFXL) {
        laneHighlightFXLTimer = 33;
        laneHighlights.laneFXL->setVisibility(true);
    }
    if (state.bits.buttonFXR) {
        laneHighlightFXRTimer = 33;
        laneHighlights.laneFXR->setVisibility(true);
    }
}

void SceneGameplay::updateScoreDisplay() {
    if (previousScore != score) {
        previousScore = score;
        delete scoreTexture;
        scoreTexture = new Texture();
        char scoreChar[10] = {'\0'};
        itoa(score, scoreChar, 10);
        glm::ivec2 scoreSize = scoreTexture->setFontString(Renderer::defaultFont, scoreChar);
        scoreDisplay->setSize(scoreSize);
        scoreDisplay->setTexture(scoreTexture);
        scoreDisplay->setPosition(glm::ivec2(Renderer::getCurrentResolution().x - scoreSize.x - 100, 80));
    }
}

void SceneGameplay::scrollChart(unsigned int tickDelta) {
    float scrollDistance = (tickDelta / 1000.0f) * scrollSpeed;
    
    /*for (auto& note : noteList) {
        note->setPosition(note->getPosition() + glm::vec3(0.0f, 0.0f, scrollDistance));
    }*/

    //buttonChip->setPosition(glm::vec3(-(trackWidth / 2.0f) + laneWidth, buttonChipHeightPos, progress));    
}

void SceneGameplay::testHits() {
    runTimer;
    /*
    // no hits
    if (runTimer < (expectedTime - errorWindow) || runTimer > (expectedTime + errorWindow)) {
        laneHighlights.laneA->setColor(colorNoHit);
    } else {    
        if (buttonADebounce == 0 && !handledButton) {
            buttonADebounce += 8;
            handledButton = true;
            hitDelta = runTimer - expectedTime;
            if (runTimer >= (expectedTime - scritWindow) && runTimer < (expectedTime + scritWindow)) {
                laneHighlights.laneA->setColor(colorSCritHit);
                exscore += 5;
                score += 1000;
                buttonChip->setVisibility(false);
            } else if (runTimer >= (expectedTime - critWindow) && runTimer < (expectedTime + critWindow)) {
                laneHighlights.laneA->setColor(colorCritHit);
                exscore += 4;
                score += 1000;
                buttonChip->setVisibility(false);
            } else if (runTimer >= (expectedTime - nearWindow) && runTimer < (expectedTime + nearWindow)) {
                laneHighlights.laneA->setColor(colorNearHit);
                exscore += 1;
                score += 500;
                buttonChip->setVisibility(false);
            } else {
                laneHighlights.laneA->setColor(colorErrorHit);
            }

            if (hitDeltaTexture) {
                delete hitDeltaTexture;
            }
            std::stringstream ss;
            char positive = ' ';
            if (hitDelta >= 0) {
                positive = '+';
            }
            ss << positive << hitDelta << "ms";
            hitDeltaTexture = new Texture();
            glm::ivec2 hitDeltaTextureSize = hitDeltaTexture->setFontString(Renderer::defaultFont, ss.str());
            hitDeltaDisplay->setTexture(hitDeltaTexture);
            hitDeltaDisplay->setSize(hitDeltaTextureSize);
            hitDeltaDisplay->setPosition(glm::ivec2((Renderer::getCurrentResolution().x / 2) + (hitDeltaTextureSize.x / 2), (Renderer::getCurrentResolution().y / 2) - (hitDeltaTextureSize.y / 2)));
            hitDeltaDisplay->setVisibility(true);
            hitDeltaDisplayTimer = 33 * 40;
            if (hitDelta > 0) {
                lateDisplay->setVisibility(true);
                lateIndicatorTimer = 33 * 40;
                hitDeltaDisplay->setColor(colorLate);
            } else {
                earlyDisplay->setVisibility(true);
                earlyIndicatorTimer = 33 * 40;
                hitDeltaDisplay->setColor(colorEarly);
            }
        }   
    }
    */
}

void SceneGameplay::update(unsigned int tickDelta) {
    runTimer += tickDelta;
    InputState state = Input::getState();
    updateInputDisplay(state);
    updateLaneHighlightVisibility(state);
    tickDownTimers(tickDelta);

    switch (currentStage) {
        case GameplaySequence::FadeIn:{
            if (fadeInTimer > tickDelta) {
                fadeInTimer -= tickDelta;
            } else {
                fadeInTimer = 0;
            }

            fadeIn->setColor(glm::vec4(0.0f, 0.0f, 0.0f, (float)(fadeInTimer) / (float)(fadeInTimerMax)));

            if (fadeInTimer == 0) {
                currentStage = GameplaySequence::Playing;
            }
            break;
        }
        case GameplaySequence::Playing: {
            scrollChart(tickDelta);
            testHits();
            break;
        }
        case GameplaySequence::FadeOut:
        break;
    }

    /*
    updateScoreDisplay();

    if (runTimer >= expectedTime * 2) {
        runTimer -= expectedTime * 2;
        buttonChip->setVisibility(true);
    } 
    */
}

void SceneGameplay::draw() {
    for (auto& obj : objectList) {
        obj->draw();
    }
    for (auto& element : interfaceList) {
        element->draw();
    }
}