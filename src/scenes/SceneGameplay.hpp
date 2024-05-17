#pragma once

#include <cstdint>

#include <glm/glm.hpp>

#include "Scene.hpp"
#include "ChartReader.hpp"
#include "AudioSource.hpp"

class SceneGameplay : public Scene {
private:
    enum class GameplaySequence {
        FadeIn,
        Loaded,
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

    struct NoteData {
        NoteType type;
        enum class LaneType {
            LaneA,
            LaneB,
            LaneC,
            LaneD,
            LaneFXL,
            LaneFXR,
            LaserL,
            LaserR
        } lane;
        float expectedTime;
        float hitTime;
        bool hitPass;
        Object* noteObject;
    };

    struct GameplayOptions {
        enum class ArrangementOptions {
            Default,
            Mirror,
            Random,
            MirrorRandom,
            SRandom,
        } ArrangementOptions;

        struct laneArrangement {
            
        };

        struct PlayOptions {
            bool autoPlay;
            bool autoRetry;
            enum class AutoFailOptions {
                None,
                AAPlus,
                AAA,
                AAAPlus,
                S,
                UC,
                PUC,
            } autoFailObjective;
        } PlayOptions;
    } GameplayOptions;

    struct {
        std::vector<NoteData*> notes;
    } gameplayData;

    GameplaySequence currentStage;

    // gameplay objects construction

    AudioSource* bgmSource;
    AudioSource* autoClickButton;
    AudioSource* autoClickFX;

    const float trackWidth = 12.0f;
    const float trackHeight = 60.0f;
    const float laneHighlightHeight = -3.995;
    const float measureDividerHeight = -3.9995;

    const uint8_t lanes = 6;
    const uint8_t buttonLanes = 4;
    const uint8_t fxLanes = 2;

    const float laneWidth = trackWidth / ((float)lanes);
    const float buttonChipHeight = trackHeight / 40.0f;
    const float buttonChipHeightPos = -3.9f;
    const float fxChipHeightPos = -3.95f;

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
    Texture* laneHighlightButtonTexture, *laneHighlightFXTexture;

    Model* measureDivider, *buttonChip, *buttonChipKeysound, *buttonLong, *fxChip, *fxChipKeysound, *fxLong;
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
    int fadeInTimer;
    const unsigned int fadeInTimerMax = 1000;

    void buildButtonDisplay();
    void buildJudgmentCounters(glm::ivec2 windowResolution);
    void buildJudgmentDisplay(glm::ivec2 windowResolution);
    void buildInterface();

    // gameplay timers
    int loadedIntersticeTimer;
    const unsigned int loadedIntersticeTimerMax = 3000;
    const unsigned int buttonDebounceMax = 4;
    double runTimer;
    unsigned int songPlaybackDelayTimer;
    const unsigned int songPlaybackDelayTimerMax = 2000;
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
    float score;
    float previousScore;
    float scoreIncrement;
    unsigned int exscore;
    float BPM;
    float laneSpeed;
    float basePosition;
    float scrollSpeed;
    float scrollPosition;

    void initialize();
    void tickDownTimers(float deltaTime);
    void updateLaneHighlightVisibility(InputState);
    void updateInputDisplay(InputState);
    void updateScoreDisplay();
    void scrollChart(float deltaTime);
    void testHits(InputState);
    void autoHit();

public:
    SceneGameplay();
    void update(float deltaTime);
    void draw();
};