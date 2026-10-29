
#pragma once

#include <raylib.h>

#include "AppState.h"

#include "../localization/LocalizationManager.h"

#include "../screens/main_menu/MainMenu.h"
#include "../screens/save_menu/SaveMenu.h"
#include "../screens/new_game/NewGameScreen.h"
#include "../screens/settings/SettingsMenu.h"
#include "../screens/game/GameScreen.h"

#include "../game/world/TerrainTextures.h"

using namespace std;


// Главный управляющий класс приложения.
class Application
{
public:
    Application();

    void Run();

    ~Application();

private:
    const int screenWidth = 1280;
    const int screenHeight = 720;

    AppState currentState = AppState::MainMenu;

    int selectedSlot = 0;

    Font uiFont;

    LocalizationManager localization;

    TerrainTextures terrainTextures;

    // Экраны приложения.
    MainMenu* mainMenu = nullptr;
    SaveMenu* saveMenu = nullptr;
    NewGameScreen* newGameScreen = nullptr;
    SettingsMenu* settingsMenu = nullptr;
    GameScreen* gameScreen = nullptr;

    void LoadUIFont();

    void Update();

    void Draw();

    void DrawCredits();
};
