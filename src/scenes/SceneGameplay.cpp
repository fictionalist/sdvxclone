#include "Scene.hpp"

#include <cstdint>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Audio.hpp"
#include "AudioSource.hpp"
#include "Logging.hpp"
#include "UIElement.hpp"
#include "Object.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "ChartReader.hpp"

const unsigned int errorWindow = 133;
const unsigned int nearWindow = 67;
const unsigned int critWindow = 33;
const unsigned int scritWindow = 16;

const glm::vec4 colorNoHit = glm::vec4(0.1f, 0.1f, 0.8f, 0.5f);
const glm::vec4 colorErrorHit = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
const glm::vec4 colorNearHit = glm::vec4(0.8f, 0.1f, 0.8f, 0.5f);
const glm::vec4 colorCritHit = glm::vec4(0.8f, 0.8f, 0.1f, 0.5f);
const glm::vec4 colorSCritHit = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
const glm::vec4 colorLate = glm::vec4(1.0f, 0.4f, 0.4f, 0.9f);
const glm::vec4 colorEarly = glm::vec4(0.5f, 0.5f, 1.0f, 0.9f);

void SceneGameplay::buildTrack() {
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

    laneHighlightButtonTexture = new Texture();
    laneHighlightButtonTexture->loadImage("./data/textures/lane_highlight.png");
    laneHighlightFXTexture = new Texture();
    laneHighlightFXTexture->loadImage("./data/textures/lane_highlight_fx.png");

    laneHighlightButton = new Model();
    laneHighlightButton->addVertex(Vertex({0.0f,      0.0f, 0.0f},        {0.0f, 1.0f}));
    laneHighlightButton->addVertex(Vertex({laneWidth, 0.0f, 0.0f},        {1.0f, 1.0f}));
    laneHighlightButton->addVertex(Vertex({laneWidth, 0.0f, trackHeight}, {1.0f, 0.0f}));
    laneHighlightButton->addVertex(Vertex({0.0f,      0.0f, 0.0f},        {0.0f, 1.0f}));
    laneHighlightButton->addVertex(Vertex({laneWidth, 0.0f, trackHeight}, {1.0f, 0.0f}));
    laneHighlightButton->addVertex(Vertex({0.0f,      0.0f, trackHeight}, {0.0f, 0.0f}));
    laneHighlightButton->buildModel();
    laneHighlightButton->setTexture(laneHighlightButtonTexture);

    laneHighlightFX = new Model();
    laneHighlightFX->addVertex(Vertex({0.0f,             0.0f, 0.0f},        {0.0f, 1.0f}));
    laneHighlightFX->addVertex(Vertex({laneWidth * 2.0f, 0.0f, 0.0f},        {1.0f, 1.0f}));
    laneHighlightFX->addVertex(Vertex({laneWidth * 2.0f, 0.0f, trackHeight}, {1.0f, 0.0f}));
    laneHighlightFX->addVertex(Vertex({0.0f,             0.0f, 0.0f},        {0.0f, 1.0f}));
    laneHighlightFX->addVertex(Vertex({laneWidth * 2.0f, 0.0f, trackHeight}, {1.0f, 0.0f}));
    laneHighlightFX->addVertex(Vertex({0.0f,             0.0f, trackHeight}, {0.0f, 0.0f}));
    laneHighlightFX->buildModel();
    laneHighlightFX->setTexture(laneHighlightFXTexture);

    i = 1;
    laneHighlights.laneA = new Object();
    laneHighlights.laneA->model = laneHighlightButton;
    laneHighlights.laneA->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * (float)i), laneHighlightHeight, -(trackHeight)));
    laneHighlights.laneA->setVisibility(false);
    laneHighlights.laneA->setColor(colorNoHit);
    addObject(laneHighlights.laneA);

    i += 1;
    laneHighlights.laneB = new Object();
    laneHighlights.laneB->model = laneHighlightButton;
    laneHighlights.laneB->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * (float)i), laneHighlightHeight, -(trackHeight)));
    laneHighlights.laneB->setVisibility(false);
    laneHighlights.laneB->setColor(colorNoHit);
    addObject(laneHighlights.laneB);

    i += 1;
    laneHighlights.laneC = new Object();
    laneHighlights.laneC->model = laneHighlightButton;
    laneHighlights.laneC->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * (float)i), laneHighlightHeight, -(trackHeight)));
    laneHighlights.laneC->setVisibility(false);
    laneHighlights.laneC->setColor(colorNoHit);
    addObject(laneHighlights.laneC);

    i += 1;
    laneHighlights.laneD = new Object();
    laneHighlights.laneD->model = laneHighlightButton;
    laneHighlights.laneD->setPosition(glm::vec3(-(trackWidth / 2.0f) + (laneWidth * (float)i), laneHighlightHeight, -(trackHeight)));
    laneHighlights.laneD->setVisibility(false);
    laneHighlights.laneD->setColor(colorNoHit);
    addObject(laneHighlights.laneD);

    i = 0;
    laneHighlights.laneFXL = new Object();
    laneHighlights.laneFXL->model = laneHighlightFX;
    laneHighlights.laneFXL->setPosition(glm::vec3(-(trackWidth / 2.0f) + laneWidth + (laneWidth * (float)i * 2.0f), laneHighlightHeight, -(trackHeight)));
    laneHighlights.laneFXL->setVisibility(false);
    laneHighlights.laneFXL->setColor(colorNoHit);
    addObject(laneHighlights.laneFXL);

    i += 1;
    laneHighlights.laneFXR = new Object();
    laneHighlights.laneFXR->model = laneHighlightFX;
    laneHighlights.laneFXR->setPosition(glm::vec3(-(trackWidth / 2.0f) + laneWidth + (laneWidth * (float)i * 2.0f), laneHighlightHeight, -(trackHeight)));
    laneHighlights.laneFXR->setVisibility(false);
    laneHighlights.laneFXR->setColor(colorNoHit);
    addObject(laneHighlights.laneFXR);

    measureDivider = new Model();
    measureDivider->addVertex(Vertex({0.0f,             0.0f, -0.2f}, {0.0f, 0.0f}));
    measureDivider->addVertex(Vertex({laneWidth * 4.0f, 0.0f, -0.2f}, {0.0f, 1.0f}));
    measureDivider->addVertex(Vertex({laneWidth * 4.0f, 0.0f,  0.2f}, {1.0f, 1.0f}));
    measureDivider->addVertex(Vertex({0.0f,             0.0f, -0.2f}, {0.0f, 0.0f}));
    measureDivider->addVertex(Vertex({laneWidth * 4.0f, 0.0f,  0.2f}, {1.0f, 1.0f}));
    measureDivider->addVertex(Vertex({0.0f,             0.0f,  0.2f}, {0.0f, 1.0f}));
    measureDivider->buildModel();
    measureDivider->setTexture(measureDividerTexture);

    buttonChip = new Model();
    buttonChip->addVertex(Vertex({0.0f,      0.0f, -buttonChipHeight}, {0.0f, 0.0f}));
    buttonChip->addVertex(Vertex({laneWidth, 0.0f, -buttonChipHeight}, {1.0f, 0.0f}));
    buttonChip->addVertex(Vertex({laneWidth, 0.0f, 0.0f},              {1.0f, 1.0f}));
    buttonChip->addVertex(Vertex({0.0f,      0.0f, -buttonChipHeight}, {0.0f, 0.0f}));
    buttonChip->addVertex(Vertex({laneWidth, 0.0f, 0.0f},              {1.0f, 1.0f}));
    buttonChip->addVertex(Vertex({0.0f,      0.0f, 0.0f},              {0.0f, 1.0f}));
    buttonChip->buildModel();
    buttonChip->setTexture(buttonChipTexture);

    fxChip = new Model();
    fxChip->addVertex(Vertex({0.0f,             0.0f, -buttonChipHeight}, {0.0f, 0.0f}));
    fxChip->addVertex(Vertex({laneWidth * 2.0f, 0.0f, -buttonChipHeight}, {1.0f, 0.0f}));
    fxChip->addVertex(Vertex({laneWidth * 2.0f, 0.0f, 0.0f},              {1.0f, 1.0f}));
    fxChip->addVertex(Vertex({0.0f,             0.0f, -buttonChipHeight}, {0.0f, 0.0f}));
    fxChip->addVertex(Vertex({laneWidth * 2.0f, 0.0f, 0.0f},              {1.0f, 1.0f}));
    fxChip->addVertex(Vertex({0.0f,             0.0f, 0.0f},              {0.0f, 1.0f}));
    fxChip->buildModel();
    fxChip->setTexture(fxChipTexture);
}

void SceneGameplay::buildNotes(ChartData* data) {
    const unsigned int tempoNumerator = 4;
    const unsigned int tempoDenominator = 4;

    double currentTime = (double)songPlaybackDelayTimerMax;
    float defaultMeasureWidth = (trackHeight / 4.0f) * laneSpeed;
    float currentDrawScroll = -defaultMeasureWidth * 2.0f;

    float currentBPM = (float)atof(data->metadata.find("t")->second.c_str());
    BPM = currentBPM;

    for (auto& measure : data->measures) {
        Object* divider = new Object();
        divider->model = measureDivider;
        divider->setPosition(glm::vec3(-laneWidth * 2.0f, measureDividerHeight, currentDrawScroll));
        divider->setScrollability(true);
        addObject(divider);
        
        float beatSpacing = defaultMeasureWidth / (float)measure->beats.size();

        const float measureTime = ((float)tempoNumerator) / (currentBPM / 60.0f);
        const float beatTime = (measureTime / (float)measure->beats.size());
        unsigned int beatNum = 0;
        for (auto& beat : measure->beats) {
            if (beat->A == NoteType::Chip) {
                Object* noteObject = new Object();
                noteObject->model = buttonChip;
                noteObject->setPosition(glm::vec3((-trackWidth / 2.0f) + (laneWidth * 1.0f), buttonChipHeightPos, currentDrawScroll));
                noteObject->setScrollability(true);
                addObject(noteObject);

                NoteData* note = new NoteData();
                note->type = NoteType::Chip;
                note->expectedTime = currentTime;
                note->hitTime = 0;
                note->hitPass = false;
                note->lane = NoteData::LaneType::LaneA;
                note->noteObject = noteObject;
                gameplayData.notes.push_back(note);
            }
            if (beat->B == NoteType::Chip) {
                Object* noteObject = new Object();
                noteObject->model = buttonChip;
                noteObject->setPosition(glm::vec3((-trackWidth / 2.0f) + (laneWidth * 2.0f), buttonChipHeightPos, currentDrawScroll));
                noteObject->setScrollability(true);
                addObject(noteObject);

                NoteData* note = new NoteData();
                note->type = NoteType::Chip;
                note->expectedTime = currentTime;
                note->hitTime = 0;
                note->hitPass = false;
                note->lane = NoteData::LaneType::LaneB;
                note->noteObject = noteObject;
                gameplayData.notes.push_back(note);
            }
            if (beat->C == NoteType::Chip) {
                Object* noteObject = new Object();
                noteObject->model = buttonChip;
                noteObject->setPosition(glm::vec3((-trackWidth / 2.0f) + (laneWidth * 3.0f), buttonChipHeightPos, currentDrawScroll));
                noteObject->setScrollability(true);
                addObject(noteObject);
                
                NoteData* note = new NoteData();
                note->type = NoteType::Chip;
                note->expectedTime = currentTime;
                note->hitTime = 0;
                note->hitPass = false;
                note->lane = NoteData::LaneType::LaneC;
                note->noteObject = noteObject;
                gameplayData.notes.push_back(note);
            }
            if (beat->D == NoteType::Chip) {
                Object* noteObject = new Object();
                noteObject->model = buttonChip;
                noteObject->setPosition(glm::vec3((-trackWidth / 2.0f) + (laneWidth * 4.0f), buttonChipHeightPos, currentDrawScroll));
                noteObject->setScrollability(true);
                addObject(noteObject);

                NoteData* note = new NoteData();
                note->type = NoteType::Chip;
                note->expectedTime = currentTime;
                note->hitTime = 0;
                note->hitPass = false;
                note->lane = NoteData::LaneType::LaneD;
                note->noteObject = noteObject;
                gameplayData.notes.push_back(note);
            }
            if (beat->FXL == NoteType::Chip) {
                Object* noteObject = new Object();
                noteObject->model = fxChip;
                noteObject->setPosition(glm::vec3((-trackWidth / 2.0f) + (laneWidth * 1.0f), fxChipHeightPos, currentDrawScroll));
                noteObject->setScrollability(true);
                addObject(noteObject);

                NoteData* note = new NoteData();
                note->type = NoteType::Chip;
                note->expectedTime = currentTime;
                note->hitTime = 0;
                note->hitPass = false;
                note->lane = NoteData::LaneType::LaneFXL;
                note->noteObject = noteObject;
                gameplayData.notes.push_back(note);
            }
            if (beat->FXR == NoteType::Chip) {
                Object* noteObject = new Object();
                noteObject->model = fxChip;
                noteObject->setPosition(glm::vec3((-trackWidth / 2.0f) + (laneWidth * 3.0f), fxChipHeightPos, currentDrawScroll));
                noteObject->setScrollability(true);
                addObject(noteObject);

                NoteData* note = new NoteData();
                note->type = NoteType::Chip;
                note->expectedTime = currentTime;
                note->hitTime = 0;
                note->hitPass = false;
                note->lane = NoteData::LaneType::LaneFXR;
                note->noteObject = noteObject;
                gameplayData.notes.push_back(note);
            }
            currentDrawScroll -= beatSpacing;
            currentTime += beatTime * 1000.0;
        }
    }
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
    //addInterface(lifeMeter);

    buildButtonDisplay();
    buildJudgmentDisplay(windowResolution);
    //buildJudgmentCounters(windowResolution);

    fadeIn = new UIElement();
    fadeIn->setSize(windowResolution);
    fadeIn->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    fadeIn->setPosition(glm::ivec2(0));
    fadeIn->setVisibility(true);
    addInterface(fadeIn);
}

void SceneGameplay::initialize() {
    buttonDisplayElements = {};
    trackLaneObjects = {};

    currentStage = GameplaySequence::FadeIn;
    fadeInTimer = fadeInTimerMax;
    loadedIntersticeTimer = loadedIntersticeTimerMax;
    runTimer = 0;

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

    hitDelta = 0;
    score = 0;
    previousScore = 0;
    exscore = 0;
    BPM = 120.0f;
    laneSpeed = 5.0f;
    basePosition = 0.0f;
    scrollPosition = 0.0f;
    scoreIncrement = 0.0;

    Logging::info("Gameplay: Initializing gameplay data.");

    judgmentData = {};

    Logging::info("Gameplay: Initializing sounds.");

    bgmSource = new AudioSource();
    autoClickButton = new AudioSource();
    unsigned int clickButtonBuffer = Audio::loadFile("./data/sound/click.wav");
    autoClickButton->attachBuffer(clickButtonBuffer);

    autoClickFX = new AudioSource();
    autoClickFX->attachBuffer(clickButtonBuffer);
}

SceneGameplay::SceneGameplay() {
    Logging::info("Gameplay: Initializing.");
    initialize();

    Logging::info("Gameplay: Loading chart.");
    std::optional<ChartData*> data = ChartReader::readFromFile("./songs/test/test.ksh");

    Logging::info("Gameplay: Loading song.");
    unsigned int bgmBuffer = Audio::loadFile("./songs/test/mxm.ogg");
    bgmSource->attachBuffer(bgmBuffer);

    Logging::info("Gameplay: Building base gameplay objects.");

    buildTrack();

    Logging::info("Gameplay: Building notes and chart data.");
    buildNotes(data.value_or(new ChartData()));

    Logging::info("Gameplay: Building interface.");
    buildInterface();
    
    Camera::setPosition(glm::vec3(0.0f, 15.0f, 25.0f));
    Camera::lookAt(glm::vec3(0.0f, 0.0f, -20.0f));

    fadeInTimer = fadeInTimerMax;

    GameplayOptions = {
        .ArrangementOptions = GameplayOptions::ArrangementOptions::Default,
        .PlayOptions = {
            .autoPlay = false,
            .autoRetry = false,
            .autoFailObjective = GameplayOptions::PlayOptions::AutoFailOptions::None,
        },
    };

    scoreIncrement = 10000000.0f / ((float)gameplayData.notes.size() * 2.0f);

    Logging::info("Gameplay: Initialized.");
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

void SceneGameplay::tickDownTimers(float deltaTime) {
    if (laneHighlightATimer > deltaTime) {
        laneHighlightATimer -= deltaTime;
    } else {
        laneHighlightATimer = 0;
    }
    if (laneHighlightBTimer > deltaTime) {
        laneHighlightBTimer -= deltaTime;
    } else {
        laneHighlightBTimer = 0;
    }
    if (laneHighlightCTimer > deltaTime) {
        laneHighlightCTimer -= deltaTime;
    } else {
        laneHighlightCTimer = 0;
    }
    if (laneHighlightDTimer > deltaTime) {
        laneHighlightDTimer -= deltaTime;
    } else {
        laneHighlightDTimer = 0;
    }
    if (laneHighlightFXLTimer > deltaTime) {
        laneHighlightFXLTimer -= deltaTime;
    } else {
        laneHighlightFXLTimer = 0;
    }
    if (laneHighlightFXRTimer > deltaTime) {
        laneHighlightFXRTimer -= deltaTime;
    } else {
        laneHighlightFXRTimer = 0;
    }

    if (earlyIndicatorTimer > deltaTime) {
        earlyIndicatorTimer -= deltaTime;
    } else {
        earlyIndicatorTimer = 0;
    }

    if (lateIndicatorTimer > deltaTime) {
        lateIndicatorTimer -= deltaTime;
    } else {
        lateIndicatorTimer = 0;
    }

    if (hitDeltaDisplayTimer > deltaTime) {
        hitDeltaDisplayTimer -= deltaTime;
    } else {
        hitDeltaDisplayTimer = 0;
    }

    if (buttonADebounce > deltaTime) {
        buttonADebounce -= deltaTime;
    } else {
        buttonADebounce = 0;
    }
    if (buttonBDebounce > deltaTime) {
        buttonBDebounce -= deltaTime;
    } else {
        buttonBDebounce = 0;
    }
    if (buttonCDebounce > deltaTime) {
        buttonCDebounce -= deltaTime;
    } else {
        buttonCDebounce = 0;
    }
    if (buttonDDebounce > deltaTime) {
        buttonDDebounce -= deltaTime;
    } else {
        buttonDDebounce = 0;
    }
    if (buttonFXLDebounce > deltaTime) {
        buttonFXLDebounce -= deltaTime;
    } else {
        buttonFXLDebounce = 0;
    }
    if (buttonFXRDebounce > deltaTime) {
        buttonFXRDebounce -= deltaTime;
    } else {
        buttonFXRDebounce = 0;
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

    if (lateIndicatorTimer == 0) {
        lateDisplay->setVisibility(false);
    }
    if (earlyIndicatorTimer == 0) {
        earlyDisplay->setVisibility(false);
    }
    if (hitDeltaDisplayTimer == 0) {
        hitDeltaDisplay->setVisibility(false);
    }

    if (state.bits.buttonA) {
        laneHighlightATimer = 33;
        laneHighlights.laneA->setVisibility(true);
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
        
        itoa((int)score, scoreChar, 10);
        glm::ivec2 scoreSize = scoreTexture->setFontString(Renderer::defaultFont, scoreChar);
        scoreDisplay->setSize(scoreSize);
        scoreDisplay->setTexture(scoreTexture);
        scoreDisplay->setPosition(glm::ivec2(Renderer::getCurrentResolution().x - scoreSize.x - 100, 80));
    }
}

void SceneGameplay::scrollChart(float deltaTime) {
    runTimer += deltaTime;

    const float measureWidth = trackHeight / 4.0f * laneSpeed;
    const float measureTime = BPM / 60.0f;
    const float beatsPerFrame = (deltaTime * measureTime / 1000.0f);
    const float beatWidth = measureWidth / 4.0f;

    const float scrollDistance = beatsPerFrame * beatWidth;

    scrollPosition += scrollDistance;

    glm::mat4 scrollMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, scrollPosition));
    Renderer::defaultShader->use();
    Renderer::defaultShader->setMat4("uScroll", scrollMatrix);
}

void SceneGameplay::testHits(InputState state) {
    struct {
        bool buttonA = false;
        bool buttonB = false;
        bool buttonC = false;
        bool buttonD = false;
        bool buttonFXL = false;
        bool buttonFXR = false;
    } judgedButtons;

    unsigned int* buttonDebounce;
    bool* button;
    Object** laneHighlight;
    bool* judgeButton;

    unsigned int currentNote = 0;
    for (auto& note : gameplayData.notes) {
        currentNote++;
        if (note->hitPass == true) {
            continue;
        }

        if (runTimer < (note->expectedTime - errorWindow)) {
            continue;
        } else if (runTimer >= (note->expectedTime + errorWindow)) {
            // note error for not hitting
            note->hitPass = true;
            judgmentData.chipNotes.lateErrors += 1;
        } else {
            // test for hit during the active judgment windows
            switch (note->lane) {
                case NoteData::LaneType::LaneA:
                    buttonDebounce = &buttonADebounce;
                    button = &state.bits.buttonA;
                    laneHighlight = &laneHighlights.laneA;
                    judgeButton = &judgedButtons.buttonA;
                    break;
                case NoteData::LaneType::LaneB:
                    buttonDebounce = &buttonBDebounce;
                    button = &state.bits.buttonB;
                    laneHighlight = &laneHighlights.laneB;
                    judgeButton = &judgedButtons.buttonB;
                    break;
                case NoteData::LaneType::LaneC:
                    buttonDebounce = &buttonCDebounce;
                    button = &state.bits.buttonC;
                    laneHighlight = &laneHighlights.laneC;
                    judgeButton = &judgedButtons.buttonC;
                    break;
                case NoteData::LaneType::LaneD:
                    buttonDebounce = &buttonDDebounce;
                    button = &state.bits.buttonD;
                    laneHighlight = &laneHighlights.laneD;
                    judgeButton = &judgedButtons.buttonD;
                    break;
                case NoteData::LaneType::LaneFXL:
                    buttonDebounce = &buttonFXLDebounce;
                    button = &state.bits.buttonFXL;
                    laneHighlight = &laneHighlights.laneFXL;
                    judgeButton = &judgedButtons.buttonFXL;
                    break;
                case NoteData::LaneType::LaneFXR:
                    buttonDebounce = &buttonFXRDebounce;
                    button = &state.bits.buttonFXR;
                    laneHighlight = &laneHighlights.laneFXR;
                    judgeButton = &judgedButtons.buttonFXR;
                    break;
                default:
                    continue;
            }
            if (*buttonDebounce > 0) continue;
            if (!*button) continue;
            if (*judgeButton) continue;

            note->hitPass = true;
            note->hitTime = runTimer;
            float hitDelta = note->hitTime - note->expectedTime;
            *buttonDebounce = buttonDebounceMax;
            *judgeButton = true;

            if (note->hitTime > (note->expectedTime - scritWindow) && note->hitTime <= (note->expectedTime + scritWindow)) {
                note->noteObject->setVisibility(false);
                (*laneHighlight)->setColor(colorSCritHit);
                judgmentData.chipNotes.scrits += 1;
                score += (scoreIncrement * 2.0);
                autoClickButton->play();
            } else if (note->hitTime > (note->expectedTime - critWindow) && note->hitTime <= (note->expectedTime + critWindow)) {
                note->noteObject->setVisibility(false);
                (*laneHighlight)->setColor(colorCritHit);
                if (hitDelta >= 0.0) {
                    judgmentData.chipNotes.lateCrits += 1;
                } else {
                    judgmentData.chipNotes.earlyCrits += 1;
                }
                score += (scoreIncrement * 2.0);
                autoClickButton->play();
            } else if (note->hitTime > (note->expectedTime - nearWindow) && note->hitTime <= (note->expectedTime + nearWindow)) {
                note->noteObject->setVisibility(false);
                (*laneHighlight)->setColor(colorNearHit);
                if (hitDelta >= 0.0) {
                    judgmentData.chipNotes.lateNears += 1; 
                } else {
                    judgmentData.chipNotes.earlyNears += 1;
                }
                score += scoreIncrement;
                autoClickButton->play();
            } else {
                note->noteObject->setVisibility(false);
                (*laneHighlight)->setColor(colorNoHit);
                if (hitDelta >= 0.0) {
                    judgmentData.chipNotes.lateErrors += 1;
                } else {
                    judgmentData.chipNotes.earlyErrors += 1;
                }
            }
        }
    }
}

void SceneGameplay::autoHit() {

}

void SceneGameplay::update(float deltaTime) {
    InputState state = Input::getState();
    updateInputDisplay(state);
    updateLaneHighlightVisibility(state);
    tickDownTimers(deltaTime);

    switch (currentStage) {
        case GameplaySequence::FadeIn: {
            if (fadeInTimer > deltaTime) {
                fadeInTimer -= deltaTime;
            } else {
                fadeInTimer = 0;
            }

            fadeIn->setColor(glm::vec4(0.0f, 0.0f, 0.0f, glm::clamp((float)(fadeInTimer) / (float)(fadeInTimerMax), 0.0f, 1.0f)));

            if (fadeInTimer <= 0) {
                currentStage = GameplaySequence::Loaded;
                loadedIntersticeTimer = loadedIntersticeTimerMax;
            }
            break;
        }
        case GameplaySequence::Loaded: {
            if (state.bits.buttonStart == false) { // let the player adjust settings before the song starts by holding START between three seconds
                if (loadedIntersticeTimer > deltaTime) {
                    loadedIntersticeTimer -= deltaTime;
                } else {
                    loadedIntersticeTimer = 0;
                }
            }

            if (loadedIntersticeTimer <= 0) {
                currentStage = GameplaySequence::Playing;
            }
            break;
        }
        case GameplaySequence::Playing: {
            if (runTimer >= 2000.0) {
                bgmSource->play();
            }
            scrollChart(deltaTime);
            testHits(state);
            updateScoreDisplay();
            break;
        }
        case GameplaySequence::FadeOut:
        break;
    }
}

void SceneGameplay::draw() {
    for (auto& obj : objectList) {
        obj->draw();
    }
    for (auto& element : interfaceList) {
        element->draw();
    }
}