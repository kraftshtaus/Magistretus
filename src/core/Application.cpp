
#include "Application.h"

using namespace std;


// ========================================
// CONSTRUCTOR
// ========================================

Application::Application()
{
    InitWindow(
        screenWidth,
        screenHeight,
        "Magistretus"
    );

    SetTargetFPS(60);

    SetExitKey(KEY_NULL);

    // Общий UI-шрифт.
    LoadUIFont();

    // Текстуры игрового мира.
    terrainTextures.Load();

    // Язык по умолчанию.
    localization.LoadLanguage("en");

    // Создаём экраны приложения.
    mainMenu = new MainMenu(
        &uiFont
    );

    saveMenu = new SaveMenu(
        &uiFont
    );

    newGameScreen = new NewGameScreen(
        &uiFont
    );

    settingsMenu = new SettingsMenu(
        &uiFont
    );

    gameScreen = new GameScreen(
        &terrainTextures
    );
}


// ========================================
// UI FONT
// ========================================

void Application::LoadUIFont()
{
    const char* uiCharacters =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
        "абвгдеёжзийклмнопрстуфхцчшщъыьэюя"
        " .,:;!?()-+/";

    int codepointCount = 0;

    int* codepoints = LoadCodepoints(
        uiCharacters,
        &codepointCount
    );

    uiFont = LoadFontEx(
        "assets/fonts/ui_font.otf",
        48,
        codepoints,
        codepointCount
    );

    UnloadCodepoints(codepoints);
}


// ========================================
// MAIN LOOP
// ========================================

void Application::Run()
{
    while (
        !WindowShouldClose() &&
        currentState != AppState::Exit
    )
    {
        Update();

        BeginDrawing();

        ClearBackground(
            Color{25, 29, 38, 255}
        );

        Draw();

        EndDrawing();
    }
}


// ========================================
// UPDATE
// ========================================

void Application::Update()
{
    switch (currentState)
    {
        // ----------------------------
        // MAIN MENU
        // ----------------------------

        case AppState::MainMenu:
        {
            mainMenu->Update(
                currentState
            );

            // Начинаем создание королевства.
            if (currentState == AppState::NewGame)
            {
                newGameScreen->Reset();
            }

            // Открываем существующие слоты.
            if (currentState == AppState::SaveMenu)
            {
                saveMenu->BeginLoad();
            }

            break;
        }

        // ----------------------------
        // NEW GAME
        // ----------------------------

        case AppState::NewGame:
        {
            newGameScreen->Update();

            // Возврат в главное меню.
            if (newGameScreen->TakeBack())
            {
                currentState = AppState::MainMenu;
                break;
            }

            // Игрок завершил выбор королевства.
            NewGameSettings settings;

            if (
                newGameScreen->TakeContinue(settings)
            )
            {
                // Передаём настройки в SaveMenu.
                saveMenu->BeginCreate(
                    settings.kingdom,
                    settings.difficulty,
                    settings.kingdomName
                );

                currentState = AppState::SaveMenu;
            }

            break;
        }

        // ----------------------------
        // SAVE MENU
        // ----------------------------

        case AppState::SaveMenu:
        {
            saveMenu->Update(
                currentState,
                selectedSlot
            );

            // Если создано новое королевство,
            // сбрасываем старую карту из памяти.
            if (saveMenu->TakeNewSaveCreated())
            {
                gameScreen->InvalidateLoadedSave();
            }

            break;
        }

        // ----------------------------
        // SETTINGS
        // ----------------------------

        case AppState::Settings:
        {
            settingsMenu->Update(
                currentState,
                localization
            );

            break;
        }

        // ----------------------------
        // CREDITS
        // ----------------------------

        case AppState::Credits:
        {
            if (IsKeyPressed(KEY_ESCAPE))
            {
                currentState = AppState::MainMenu;
            }

            break;
        }

        // ----------------------------
        // GAME
        // ----------------------------

        case AppState::Game:
        {
            // OpenSave самостоятельно проверяет,
            // загружен ли уже выбранный слот.
            gameScreen->OpenSave(
                selectedSlot
            );

            gameScreen->Update();

            // Возвращаемся к списку сохранений.
            if (IsKeyPressed(KEY_ESCAPE))
            {
                saveMenu->BeginLoad();

                currentState = AppState::SaveMenu;
            }

            break;
        }

        // ----------------------------
        // EXIT
        // ----------------------------

        case AppState::Exit:
        {
            break;
        }
    }
}


// ========================================
// DRAW
// ========================================

void Application::Draw()
{
    switch (currentState)
    {
        case AppState::MainMenu:
        {
            mainMenu->Draw(
                localization
            );

            break;
        }

        case AppState::NewGame:
        {
            newGameScreen->Draw();

            break;
        }

        case AppState::SaveMenu:
        {
            saveMenu->Draw(
                localization
            );

            break;
        }

        case AppState::Settings:
        {
            settingsMenu->Draw(
                localization
            );

            break;
        }

        case AppState::Credits:
        {
            DrawCredits();

            break;
        }

        case AppState::Game:
        {
            gameScreen->Draw();

            break;
        }

        case AppState::Exit:
        {
            break;
        }
    }
}


// ========================================
// CREDITS
// ========================================

void Application::DrawCredits()
{
    string title =
        localization.Get("credits.title");

    DrawTextEx(
        uiFont,
        title.c_str(),
        Vector2{500.0f, 250.0f},
        48.0f,
        1.0f,
        RAYWHITE
    );

    DrawTextEx(
        uiFont,
        "Press ESC to return",
        Vector2{500.0f, 330.0f},
        22.0f,
        1.0f,
        GRAY
    );
}


// ========================================
// DESTRUCTOR
// ========================================

Application::~Application()
{
    delete mainMenu;
    delete saveMenu;
    delete newGameScreen;
    delete settingsMenu;
    delete gameScreen;

    terrainTextures.Unload();

    UnloadFont(uiFont);

    CloseWindow();
}
