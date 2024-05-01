#include "Scene.hpp"

#include <cstdint>
#include <sstream>

#include "Logging.hpp"
#include "UIElement.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "ChartReader.hpp"

enum class GameplaySequence {
    FadeIn,
    Playing,
    FadeOut
};

GameplaySequence currentStage = GameplaySequence::FadeIn;

UIElement* fadeIn, fadeOut;
unsigned int fadeInTimer = 0;
unsigned int fadeInTimerMax = 1000;

Model* buttonChip, *buttonChipKeysound, *buttonLong, *fxChip, *fxChipKeysound, *fxLong;
Texture* laneTexture, *measureDivider, *buttonChipTexture, *buttonChipKeysoundTexture, *buttonLongTexture, *fxChipTexture, *fxChipKeysoundTexture, *fxLongTexture;

std::vector<Model*> noteList;

Model* laneHighlightA, *laneHighlightB, *laneHighlightC, *laneHighlightD;
Model* laneHighlightFXL, *laneHighlightFXR;

UIElement* buttonADisplay, *buttonBDisplay, *buttonCDisplay, *buttonDDisplay, *buttonFXLDisplay, *buttonFXRDisplay, *buttonStartDisplay;

UIElement* lateDisplay, *earlyDisplay;
UIElement* hitDeltaDisplay;
Texture* hitDeltaTexture;

UIElement* judgmentDisplayCounter, *judgmentErrorCounter, *judgmentNearCounter, *judgmentCritCounter;

UIElement* scoreDisplay;
Texture* scoreTexture;
const float trackWidth = 12.0f;
const float trackHeight = 60.0f;

const unsigned int lanes = 6;
const float offset = -(trackWidth / (((float)lanes)));
const float laneWidth = trackWidth / ((float)lanes);
const float buttonChipHeight = trackHeight / 40.0f;
const float buttonChipHeightPos = -3.9f;

const float laneHighlightHeight = -3.995;
const float measureDividerHeight = -3.9995;

uint64_t runTimer = 0;

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

unsigned int score = 0;
unsigned int previousScore = 0;
unsigned int exscore = 0;

float BPM = 220.0f;
float laneSpeed = 8.00f;
float basePosOffset = trackHeight * 6.0f;
float basePosition = (trackHeight / 4.0f) * laneSpeed;
float scrollSpeed = ((trackHeight / 4.0f) / 4.0f) * (BPM / 60.0f) * laneSpeed;

const unsigned int expectedTime = (unsigned int)(BPM / 60.0f) * 1000;

float progress = 0.0f;

unsigned int laneHighlightATimer = 0;
unsigned int laneHighlightBTimer = 0;
unsigned int laneHighlightCTimer = 0;
unsigned int laneHighlightDTimer = 0;
unsigned int laneHighlightFXLTimer = 0;
unsigned int laneHighlightFXRTimer = 0;

unsigned int lateIndicatorTimer = 0;
unsigned int earlyIndicatorTimer = 0;
unsigned int hitDeltaDisplayTimer = 0;
int hitDelta = 0;

unsigned int buttonADebounce = 0;
unsigned int buttonBDebounce = 0;
unsigned int buttonCDebounce = 0;
unsigned int buttonDDebounce = 0;
unsigned int buttonFXLDebounce = 0;
unsigned int buttonFXRDebounce = 0;

struct JudgmentData {
    struct {
        unsigned int earlyErrors;
        unsigned int earlyNears;
        unsigned int earlyCrits;
        unsigned int scrits;
        unsigned int lateCrits;
        unsigned int lateNears;
        unsigned int lateErrors;
    } chipNotes;
    struct {
        unsigned int lateErrors;
        unsigned int scrits;
        unsigned int earlyErrors;
    } longNotes;
    struct {
        unsigned int errors;
        unsigned int scrits;
    } volumeNotes;
} judgmentData;

void buildTrack() {
    laneTexture = new Texture();
    laneTexture->loadImage("./data/textures/lane.png");
    measureDivider = new Texture();
    measureDivider->loadImage("./data/textures/measure_divider.png");
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

    for (unsigned int i = 0; i < lanes; i++) {
        Model* trackModel = new Model();
        trackModel->addVertex(Vertex({offset + (laneWidth * (float)i),       0.0f, -(trackHeight)}, {(1.0f / (float)lanes) * i,        1.0f}));
        trackModel->addVertex(Vertex({offset + (laneWidth * (float)(i + 1)), 0.0f, -(trackHeight)}, {(1.0f / (float)lanes) * (i + 1),  1.0f}));
        trackModel->addVertex(Vertex({offset + (laneWidth * (float)(i + 1)), 0.0f, 0.0f          }, {(1.0f / (float)lanes) * (i + 1),  0.0f}));
        trackModel->addVertex(Vertex({offset + (laneWidth * (float)i),       0.0f, -(trackHeight)}, {(1.0f / (float)lanes) * i,        1.0f}));
        trackModel->addVertex(Vertex({offset + (laneWidth * (float)(i + 1)), 0.0f, 0.0f          }, {(1.0f / (float)lanes) * (i + 1),  0.0f}));
        trackModel->addVertex(Vertex({offset + (laneWidth * (float)i),       0.0f, 0.0f          }, {(1.0f / (float)lanes) * i,        0.0f}));
        trackModel->buildModel();
        trackModel->setPosition(glm::vec3(-(trackWidth / 2.0f) + laneWidth, -4.0f, 0.0f));
        trackModel->setTexture(laneTexture);
        Renderer::addRenderable(trackModel);
    }

    laneHighlightA = new Model();
    laneHighlightA->addVertex(Vertex({0.0f,      0.0f,         0.0f}, {0.0f, 0.0f}));
    laneHighlightA->addVertex(Vertex({laneWidth, 0.0f,         0.0f}, {1.0f, 0.0f}));
    laneHighlightA->addVertex(Vertex({laneWidth, 0.0f, -trackHeight}, {1.0f, 1.0f}));
    laneHighlightA->addVertex(Vertex({0.0f,      0.0f,         0.0f}, {0.0f, 0.0f}));
    laneHighlightA->addVertex(Vertex({laneWidth, 0.0f, -trackHeight}, {1.0f, 1.0f}));
    laneHighlightA->addVertex(Vertex({0.0f,      0.0f, -trackHeight}, {0.0f, 1.0f}));
    laneHighlightA->buildModel();
    laneHighlightA->setColor(colorNoHit);
    laneHighlightA->setPosition(glm::vec3(-(trackWidth / 2.0f) + laneWidth, laneHighlightHeight, 0.0f));
    laneHighlightA->setVisibility(false);
    Renderer::addRenderable(laneHighlightA);

    laneHighlightB = new Model();
    laneHighlightB->addVertex(Vertex({0.0f,      0.0f,         0.0f}, {0.0f, 0.0f}));
    laneHighlightB->addVertex(Vertex({laneWidth, 0.0f,         0.0f}, {1.0f, 0.0f}));
    laneHighlightB->addVertex(Vertex({laneWidth, 0.0f, -trackHeight}, {1.0f, 1.0f}));
    laneHighlightB->addVertex(Vertex({0.0f,      0.0f,         0.0f}, {0.0f, 0.0f}));
    laneHighlightB->addVertex(Vertex({laneWidth, 0.0f, -trackHeight}, {1.0f, 1.0f}));
    laneHighlightB->addVertex(Vertex({0.0f,      0.0f, -trackHeight}, {0.0f, 1.0f}));
    laneHighlightB->buildModel();
    laneHighlightB->setColor(colorNoHit);
    laneHighlightB->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 2), laneHighlightHeight, 0.0f));
    laneHighlightB->setVisibility(false);
    Renderer::addRenderable(laneHighlightB);

    laneHighlightC = new Model();
    laneHighlightC->addVertex(Vertex({0.0f,      0.0f,         0.0f}, {0.0f, 0.0f}));
    laneHighlightC->addVertex(Vertex({laneWidth, 0.0f,         0.0f}, {1.0f, 0.0f}));
    laneHighlightC->addVertex(Vertex({laneWidth, 0.0f, -trackHeight}, {1.0f, 1.0f}));
    laneHighlightC->addVertex(Vertex({0.0f,      0.0f,         0.0f}, {0.0f, 0.0f}));
    laneHighlightC->addVertex(Vertex({laneWidth, 0.0f, -trackHeight}, {1.0f, 1.0f}));
    laneHighlightC->addVertex(Vertex({0.0f,      0.0f, -trackHeight}, {0.0f, 1.0f}));
    laneHighlightC->buildModel();
    laneHighlightC->setColor(colorNoHit);
    laneHighlightC->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 3), laneHighlightHeight, 0.0f));
    laneHighlightC->setVisibility(false);
    Renderer::addRenderable(laneHighlightC);

    laneHighlightD = new Model();
    laneHighlightD->addVertex(Vertex({0.0f,      0.0f,         0.0f}, {0.0f, 0.0f}));
    laneHighlightD->addVertex(Vertex({laneWidth, 0.0f,         0.0f}, {1.0f, 0.0f}));
    laneHighlightD->addVertex(Vertex({laneWidth, 0.0f, -trackHeight}, {1.0f, 1.0f}));
    laneHighlightD->addVertex(Vertex({0.0f,      0.0f,         0.0f}, {0.0f, 0.0f}));
    laneHighlightD->addVertex(Vertex({laneWidth, 0.0f, -trackHeight}, {1.0f, 1.0f}));
    laneHighlightD->addVertex(Vertex({0.0f,      0.0f, -trackHeight}, {0.0f, 1.0f}));
    laneHighlightD->buildModel();
    laneHighlightD->setColor(colorNoHit);
    laneHighlightD->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 4), laneHighlightHeight, 0.0f));
    laneHighlightD->setVisibility(false);
    Renderer::addRenderable(laneHighlightD);

    laneHighlightFXL = new Model();
    laneHighlightFXL->addVertex(Vertex({0.0f,             0.0f,         0.0f}, {0.0f, 0.0f}));
    laneHighlightFXL->addVertex(Vertex({laneWidth * 2.0f, 0.0f,         0.0f}, {1.0f, 0.0f}));
    laneHighlightFXL->addVertex(Vertex({laneWidth * 2.0f, 0.0f, -trackHeight}, {1.0f, 1.0f}));
    laneHighlightFXL->addVertex(Vertex({0.0f,             0.0f,         0.0f}, {0.0f, 0.0f}));
    laneHighlightFXL->addVertex(Vertex({laneWidth * 2.0f, 0.0f, -trackHeight}, {1.0f, 1.0f}));
    laneHighlightFXL->addVertex(Vertex({0.0f,             0.0f, -trackHeight}, {0.0f, 1.0f}));
    laneHighlightFXL->buildModel();
    laneHighlightFXL->setColor(colorNoHit);
    laneHighlightFXL->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 1), laneHighlightHeight, 0.0f));
    laneHighlightFXL->setVisibility(false);
    Renderer::addRenderable(laneHighlightFXL);

    laneHighlightFXR = new Model();
    laneHighlightFXR->addVertex(Vertex({0.0f,             0.0f,         0.0f}, {0.0f, 0.0f}));
    laneHighlightFXR->addVertex(Vertex({laneWidth * 2.0f, 0.0f,         0.0f}, {1.0f, 0.0f}));
    laneHighlightFXR->addVertex(Vertex({laneWidth * 2.0f, 0.0f, -trackHeight}, {1.0f, 1.0f}));
    laneHighlightFXR->addVertex(Vertex({0.0f,             0.0f,         0.0f}, {0.0f, 0.0f}));
    laneHighlightFXR->addVertex(Vertex({laneWidth * 2.0f, 0.0f, -trackHeight}, {1.0f, 1.0f}));
    laneHighlightFXR->addVertex(Vertex({0.0f,             0.0f, -trackHeight}, {0.0f, 1.0f}));
    laneHighlightFXR->buildModel();
    laneHighlightFXR->setColor(colorNoHit);
    laneHighlightFXR->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * 3), laneHighlightHeight, 0.0f));
    laneHighlightFXR->setVisibility(false);
    Renderer::addRenderable(laneHighlightFXR);
}

void buildNotes(ChartData* data) {
    // currently a quick attempt at getting notes on the screen
    // SHOULD be optimized later to just reuse a few models and redraw it on different positions
    float currentPosition = -basePosition;
    const float defaultMeasureWidth = (trackHeight / 4.0f) * laneSpeed;
    for (auto& measure : data->measures) {
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
    }
}

void buildButtonDisplay() {
    UIElement* controllerDisplayBackground = new UIElement();
    controllerDisplayBackground->setSize(glm::ivec2(150, 80));
    controllerDisplayBackground->setPosition(glm::ivec2(40, 40));
    controllerDisplayBackground->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.1f));
    Renderer::addInterface(controllerDisplayBackground);

    buttonADisplay = new UIElement();
    buttonADisplay->setSize(glm::ivec2(20, 20));
    buttonADisplay->setPosition(glm::ivec2(60, 68));
    buttonADisplay->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    Renderer::addInterface(buttonADisplay);

    buttonBDisplay = new UIElement();
    buttonBDisplay->setSize(glm::ivec2(20, 20));
    buttonBDisplay->setPosition(glm::ivec2(90, 68));
    buttonBDisplay->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    Renderer::addInterface(buttonBDisplay);

    buttonCDisplay = new UIElement();
    buttonCDisplay->setSize(glm::ivec2(20, 20));
    buttonCDisplay->setPosition(glm::ivec2(120, 68));
    buttonCDisplay->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    Renderer::addInterface(buttonCDisplay);

    buttonDDisplay = new UIElement();
    buttonDDisplay->setSize(glm::ivec2(20, 20));
    buttonDDisplay->setPosition(glm::ivec2(150, 68));
    buttonDDisplay->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    Renderer::addInterface(buttonDDisplay);

    buttonFXLDisplay = new UIElement();
    buttonFXLDisplay->setSize(glm::ivec2(20, 10));
    buttonFXLDisplay->setPosition(glm::ivec2(75, 94));
    buttonFXLDisplay->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    Renderer::addInterface(buttonFXLDisplay);

    buttonFXRDisplay = new UIElement();
    buttonFXRDisplay->setSize(glm::ivec2(20, 10));
    buttonFXRDisplay->setPosition(glm::ivec2(135, 94));
    buttonFXRDisplay->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    Renderer::addInterface(buttonFXRDisplay);
}

void buildJudgmentCounters(glm::ivec2 windowResolution) {
    UIElement* judgmentCounterBackground = new UIElement();
    judgmentCounterBackground->setSize(glm::ivec2(200));
    judgmentCounterBackground->setPosition(glm::ivec2(windowResolution.x - 200, (windowResolution.y / 2)));
    judgmentCounterBackground->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.1f));
    Renderer::addInterface(judgmentCounterBackground);

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
    Renderer::addInterface(judgmentLabelEarlyErrors);

    UIElement* judgmentLabelEarlyNears = new UIElement();
    judgmentLabelEarlyNears->setSize(labelSize);
    judgmentLabelEarlyNears->setPosition(glm::ivec2(windowResolution.x - 200, (windowResolution.y / 2) + labelSize.y));
    judgmentLabelEarlyNears->setTexture(labelEarlyNears);
    Renderer::addInterface(judgmentLabelEarlyNears);

    UIElement* judgmentLabelEarlyCrits = new UIElement();
    judgmentLabelEarlyCrits->setSize(labelSize);
    judgmentLabelEarlyCrits->setPosition(glm::ivec2(windowResolution.x - 200, (windowResolution.y / 2) + (labelSize.y * 2)));
    judgmentLabelEarlyCrits->setTexture(labelEarlyCrits);
    Renderer::addInterface(judgmentLabelEarlyCrits);

    UIElement* judgmentLabelSCrits = new UIElement();
    judgmentLabelSCrits->setSize(labelSize);
    judgmentLabelSCrits->setPosition(glm::ivec2(windowResolution.x - 200, (windowResolution.y / 2) + (labelSize.y * 3)));
    judgmentLabelSCrits->setTexture(labelSCrits);
    Renderer::addInterface(judgmentLabelSCrits);

    UIElement* judgmentLabelLateCrits = new UIElement();
    judgmentLabelLateCrits->setSize(labelSize);
    judgmentLabelLateCrits->setPosition(glm::ivec2(windowResolution.x - 200, (windowResolution.y / 2) + (labelSize.y * 4)));
    judgmentLabelLateCrits->setTexture(labelLateCrits);
    Renderer::addInterface(judgmentLabelLateCrits);

    UIElement* judgmentLabelLateNears = new UIElement();
    judgmentLabelLateNears->setSize(labelSize);
    judgmentLabelLateNears->setPosition(glm::ivec2(windowResolution.x - 200, (windowResolution.y / 2) + (labelSize.y * 5)));
    judgmentLabelLateNears->setTexture(labelLateNears);
    Renderer::addInterface(judgmentLabelLateNears);

    UIElement* judgmentLabelLateErrors = new UIElement();
    judgmentLabelLateErrors->setSize(labelSize);
    judgmentLabelLateErrors->setPosition(glm::ivec2(windowResolution.x - 200, (windowResolution.y / 2) + (labelSize.y * 6)));
    judgmentLabelLateErrors->setTexture(labelLateErrors);
    Renderer::addInterface(judgmentLabelLateErrors);

}

void buildJudgmentDisplay(glm::ivec2 windowResolution) {
    lateDisplay = new UIElement();
    Texture* lateDisplayTexture = new Texture();
    glm::ivec2 lateDisplaySize = lateDisplayTexture->setFontString(Renderer::defaultFont, "LATE");
    lateDisplay->setTexture(lateDisplayTexture);
    lateDisplay->setSize(lateDisplaySize);
    lateDisplay->setPosition(glm::ivec2((windowResolution.x / 2) - (lateDisplaySize.x / 2) - (lateDisplaySize.x), (windowResolution.y / 2) - lateDisplaySize.y / 2));
    lateDisplay->setColor(colorLate);
    lateDisplay->setVisibility(false);
    Renderer::addInterface(lateDisplay);

    earlyDisplay = new UIElement();
    Texture* earlyDisplayTexture = new Texture();
    glm::ivec2 earlyDisplaySize = earlyDisplayTexture->setFontString(Renderer::defaultFont, "EARLY");
    earlyDisplay->setTexture(earlyDisplayTexture);
    earlyDisplay->setSize(earlyDisplaySize);
    earlyDisplay->setPosition(glm::ivec2((windowResolution.x / 2) - (earlyDisplaySize.x / 2) - (earlyDisplaySize.x), (windowResolution.y / 2) - earlyDisplaySize.y / 2));
    earlyDisplay->setColor(colorEarly);
    earlyDisplay->setVisibility(false);
    Renderer::addInterface(earlyDisplay);
    
    hitDeltaDisplay = new UIElement();
    hitDeltaTexture = new Texture();
    glm::ivec2 hitDeltaTextureSize = hitDeltaTexture->setFontString(Renderer::defaultFont, "0 ms");
    hitDeltaDisplay->setTexture(hitDeltaTexture);
    hitDeltaDisplay->setSize(hitDeltaTextureSize);
    hitDeltaDisplay->setPosition(glm::ivec2((Renderer::getCurrentResolution().x / 2) + (hitDeltaTextureSize.x / 2), (Renderer::getCurrentResolution().y / 2) - (hitDeltaTextureSize.y / 2)));
    hitDeltaDisplay->setColor(colorEarly);
    Renderer::addInterface(hitDeltaDisplay);
}

void buildInterface() {
    glm::ivec2 windowResolution = Renderer::getCurrentResolution();

    scoreDisplay = new UIElement();
    scoreTexture = new Texture();
    glm::ivec2 scoreSize = scoreTexture->setFontString(Renderer::defaultFont, "0");
    scoreDisplay->setSize(scoreSize);
    scoreDisplay->setTexture(scoreTexture);
    scoreDisplay->setPosition(glm::ivec2(windowResolution.x - scoreSize.x - 100, 80));
    Renderer::addInterface(scoreDisplay);
    
    Texture* lifeMeterTexture = new Texture();
    glm::ivec2 lifeMeterSize = lifeMeterTexture->loadImage("./data/textures/life_meter.png");
    UIElement* lifeMeter = new UIElement();
    lifeMeter->setSize(lifeMeterSize);
    lifeMeter->setPosition(glm::ivec2(windowResolution.x - 200, windowResolution.y - (lifeMeterSize.y)) - (lifeMeterSize.y / 4));
    lifeMeter->setTexture(lifeMeterTexture);
    Renderer::addInterface(lifeMeter);

    buildButtonDisplay();
    buildJudgmentDisplay(windowResolution);
    buildJudgmentCounters(windowResolution);

    fadeIn = new UIElement();
    fadeIn->setSize(windowResolution);
    fadeIn->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    fadeIn->setPosition(glm::ivec2(0));
    fadeIn->setVisibility(true);
    Renderer::addInterface(fadeIn);
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

void updateInputDisplay(InputState state) {
    if (state.bits.buttonA) {
        buttonADisplay->setColor(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));
    } else {
        buttonADisplay->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    }
    if (state.bits.buttonB) {
        buttonBDisplay->setColor(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));
    } else {
        buttonBDisplay->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    }
    if (state.bits.buttonC) {
        buttonCDisplay->setColor(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));
    } else {
        buttonCDisplay->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    }
    if (state.bits.buttonD) {
        buttonDDisplay->setColor(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));
    } else {
        buttonDDisplay->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    }
    if (state.bits.buttonFXL) {
        buttonFXLDisplay->setColor(glm::vec4(0.8f, 0.0f, 0.0f, 1.0f));
    } else {
        buttonFXLDisplay->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    }
    if (state.bits.buttonFXR) {
        buttonFXRDisplay->setColor(glm::vec4(0.8f, 0.0f, 0.0f, 1.0f));
    } else {
        buttonFXRDisplay->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
    }
}

void tickDownTimers(unsigned int tickDelta) {
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

void updateLaneHighlightVisibility(InputState state) {
    if (laneHighlightATimer == 0) {
        laneHighlightA->setVisibility(false);
    }
    if (laneHighlightBTimer == 0) {
        laneHighlightB->setVisibility(false);
    }
    if (laneHighlightCTimer == 0) {
        laneHighlightC->setVisibility(false);
    }
    if (laneHighlightDTimer == 0) {
        laneHighlightD->setVisibility(false);
    }
    if (laneHighlightFXLTimer == 0) {
        laneHighlightFXL->setVisibility(false);
    }
    if (laneHighlightFXRTimer == 0) {
        laneHighlightFXR->setVisibility(false);
    }

    if (state.bits.buttonA) {
        laneHighlightATimer = 33;
        laneHighlightA->setVisibility(true);
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
        laneHighlightB->setVisibility(true);
    }
    if (state.bits.buttonC) {
        laneHighlightCTimer = 33;
        laneHighlightC->setVisibility(true);
    }
    if (state.bits.buttonD) {
        laneHighlightDTimer = 33;
        laneHighlightD->setVisibility(true);
    }
    if (state.bits.buttonFXL) {
        laneHighlightFXLTimer = 33;
        laneHighlightFXL->setVisibility(true);
    }
    if (state.bits.buttonFXR) {
        laneHighlightFXRTimer = 33;
        laneHighlightFXR->setVisibility(true);
    }
}

void updateScoreDisplay() {
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

void scrollChart(unsigned int tickDelta) {
    float scrollDistance = (tickDelta / 1000.0f) * scrollSpeed;
    
    for (auto& note : noteList) {
        note->setPosition(note->getPosition() + glm::vec3(0.0f, 0.0f, scrollDistance));
    }

    //buttonChip->setPosition(glm::vec3(-(trackWidth / 2.0f) + laneWidth, buttonChipHeightPos, progress));    
}

void testHits() {
    runTimer;
    /*
    // no hits
    if (runTimer < (expectedTime - errorWindow) || runTimer > (expectedTime + errorWindow)) {
        laneHighlightA->setColor(colorNoHit);
    } else {    
        if (buttonADebounce == 0 && !handledButton) {
            buttonADebounce += 8;
            handledButton = true;
            hitDelta = runTimer - expectedTime;
            if (runTimer >= (expectedTime - scritWindow) && runTimer < (expectedTime + scritWindow)) {
                laneHighlightA->setColor(colorSCritHit);
                exscore += 5;
                score += 1000;
                buttonChip->setVisibility(false);
            } else if (runTimer >= (expectedTime - critWindow) && runTimer < (expectedTime + critWindow)) {
                laneHighlightA->setColor(colorCritHit);
                exscore += 4;
                score += 1000;
                buttonChip->setVisibility(false);
            } else if (runTimer >= (expectedTime - nearWindow) && runTimer < (expectedTime + nearWindow)) {
                laneHighlightA->setColor(colorNearHit);
                exscore += 1;
                score += 500;
                buttonChip->setVisibility(false);
            } else {
                laneHighlightA->setColor(colorErrorHit);
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