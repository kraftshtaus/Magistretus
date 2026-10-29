
#pragma once

#include <raylib.h>
#include <string>

#include "../../game/kingdom/KingdomType.h"

using namespace std;


struct NewGameSettings
{
    KingdomType kingdom = KingdomType::Red;
    int difficulty = 2;
    string kingdomName = "";
};


class NewGameScreen
{
public:
    explicit NewGameScreen(Font* uiFont);
    ~NewGameScreen();

    NewGameScreen(const NewGameScreen&) = delete;
    NewGameScreen& operator=(const NewGameScreen&) = delete;

    void Reset();
    void Update();
    void Draw() const;

    bool TakeContinue(NewGameSettings& result);
    bool TakeBack();

private:
    Font* font = nullptr;

    NewGameSettings settings;

    // Большие короны королевств.
    Texture2D crownTextures[4] = {};
    bool crownLoaded[4] = {};

    // Два состояния короны сложности.
    // 0 - пустая, 1 - заполненная.
    Texture2D difficultyTextures[2] = {};
    bool difficultyLoaded[2] = {};

    bool editingName = false;
    bool continueRequested = false;
    bool backRequested = false;

    float crownTransition = 0.0f;

    void LoadCrownTextures();
    void LoadDifficultyTextures();

    void ChangeKingdom(int direction);
    void UpdateNameInput();

    bool HasValidName() const;

    void DrawLabel(
        const string& text,
        float x,
        float y,
        float size,
        Color color
    ) const;

    void DrawCenteredLabel(
        const string& text,
        float centerX,
        float y,
        float size,
        Color color
    ) const;

    void DrawKingdomSelection() const;
    void DrawCrownPlaceholder(
        float centerX,
        float y,
        Color color
    ) const;

    void DrawModifiers() const;
    void DrawStartPanel() const;
    void DrawDifficulty() const;

    void DrawDifficultyCrown(
        int index,
        float x,
        float y
    ) const;
};
