#pragma once

#include <cstdint>

#include <glm/glm.hpp>

#include "Scene.hpp"
#include "ChartReader.hpp"

class SceneGameplay : public Scene {
private:
    enum class GameplaySequence {
        FadeIn,
        Playing,
        FadeOut
    };

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

    GameplaySequence currentStage = GameplaySequence::FadeIn;

    // gameplay objects construction
    struct LaneObjects {
        Object* laneVolL;
        Object* laneA;
        Object* laneB;
        Object* laneC;
        Object* laneD;
        Object* laneVolR;
    } trackLaneObjects;
    Model* trackLaneModel;
    Texture* laneTexture;
    
    struct LaneHighlightObjects {
        Object* laneA;
        Object* laneB;
        Object* laneC;
        Object* laneD;
        Object* laneFXL;
        Object* laneFXR;
    } laneHighlights;
    Model* laneHighlightButton, *laneHighlightFX;

    Model* buttonChip, *buttonChipKeysound, *buttonLong, *fxChip, *fxChipKeysound, *fxLong;
    Texture *measureDividerTexture, *buttonChipTexture, *buttonChipKeysoundTexture, *buttonLongTexture, *fxChipTexture, *fxChipKeysoundTexture, *fxLongTexture;

    std::vector<Object*> noteList;

    void buildTrack();
    void buildNotes(ChartData* data);
    void buildGameObjects();

    // interface construction

    struct {
        UIElement* buttonA;
        UIElement* buttonB;
        UIElement* buttonC;
        UIElement* buttonD;
        UIElement* buttonFXL;
        UIElement* buttonFXR;
    } buttonDisplayElements;

    UIElement* lateDisplay, *earlyDisplay;
    UIElement* hitDeltaDisplay;
    Texture* hitDeltaTexture;

    UIElement* scoreDisplay;
    Texture* scoreTexture;
    UIElement* judgmentDisplayCounter, *judgmentErrorCounter, *judgmentNearCounter, *judgmentCritCounter;

    UIElement* fadeIn;
    unsigned int fadeInTimer;
    const unsigned int fadeInTimerMax = 1000;

    void buildButtonDisplay();
    void buildJudgmentCounters(glm::ivec2 windowResolution);
    void buildJudgmentDisplay(glm::ivec2 windowResolution);
    void buildInterface();

    // gameplay timers
    uint64_t runTimer;
    unsigned int laneHighlightATimer;
    unsigned int laneHighlightBTimer;
    unsigned int laneHighlightCTimer;
    unsigned int laneHighlightDTimer;
    unsigned int laneHighlightFXLTimer;
    unsigned int laneHighlightFXRTimer;

    unsigned int lateIndicatorTimer;
    unsigned int earlyIndicatorTimer;
    unsigned int hitDeltaDisplayTimer;

    unsigned int buttonADebounce;
    unsigned int buttonBDebounce;
    unsigned int buttonCDebounce;
    unsigned int buttonDDebounce;
    unsigned int buttonFXLDebounce;
    unsigned int buttonFXRDebounce;

    // gameplay variables

    int hitDelta;
    unsigned int score;
    unsigned int previousScore;
    unsigned int exscore;
    float BPM;
    float laneSpeed;
    float basePosition;
    float scrollSpeed;

    void tickDownTimers(unsigned int tickDelta);
    void updateLaneHighlightVisibility(InputState);
    void updateInputDisplay(InputState);
    void updateScoreDisplay();
    void scrollChart(unsigned int tickDelta);
    void testHits();

public:
    SceneGameplay();
    void update(unsigned int tickDelta);
    void draw();
};