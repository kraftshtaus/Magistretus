
#include "MainMenu.h"

using namespace std;


// ========================================
// CONSTRUCTOR
// ========================================

MainMenu::MainMenu(Font* uiFont)
    : newGameButton(
        Rectangle{480, 238, 320, 64},
        "NEW GAME",
        uiFont
    ),
      playButton(
        Rectangle{480, 310, 320, 64},
        "PLAY",
        uiFont
    ),
      settingsButton(
        Rectangle{480, 382, 320, 64},
        "SETTINGS",
        uiFont
    ),
      creditsButton(
        Rectangle{480, 454, 320, 64},
        "CREDITS",
        uiFont
    ),
      exitButton(
        Rectangle{480, 526, 320, 64},
        "EXIT",
        uiFont
    )
{
    // Только главное меню использует
    // новые пиксельные кнопки.
    newGameButton.SetStyle(ButtonStyle::PixelArt);
    playButton.SetStyle(ButtonStyle::PixelArt);
    settingsButton.SetStyle(ButtonStyle::PixelArt);
    creditsButton.SetStyle(ButtonStyle::PixelArt);
    exitButton.SetStyle(ButtonStyle::PixelArt);
}


// ========================================
// UPDATE
// ========================================

void MainMenu::Update(AppState& currentState)
{
    if (newGameButton.IsPressed())
    {
        currentState = AppState::NewGame;
        return;
    }

    if (playButton.IsPressed())
    {
        currentState = AppState::SaveMenu;
        return;
    }

    if (settingsButton.IsPressed())
    {
        currentState = AppState::Settings;
        return;
    }

    if (creditsButton.IsPressed())
    {
        currentState = AppState::Credits;
        return;
    }

    if (exitButton.IsPressed())
    {
        currentState = AppState::Exit;
        return;
    }
}


// ========================================
// DRAW
// ========================================

void MainMenu::Draw(
    LocalizationManager& localization
)
{
    // Локализация.
    newGameButton.SetText("NEW GAME");

    playButton.SetText(
        localization.Get("menu.play")
    );

    settingsButton.SetText(
        localization.Get("menu.settings")
    );

    creditsButton.SetText(
        localization.Get("menu.credits")
    );

    exitButton.SetText(
        localization.Get("menu.exit")
    );

    // ------------------------------------
    // TITLE
    // ------------------------------------

    const char* title = "MAGISTRETUS";

    const int titleSize = 64;

    int titleWidth = MeasureText(
        title,
        titleSize
    );

    DrawText(
        title,
        GetScreenWidth() / 2 - titleWidth / 2,
        120,
        titleSize,
        RAYWHITE
    );

    // ------------------------------------
    // BUTTONS
    // ------------------------------------

    newGameButton.Draw();
    playButton.Draw();
    settingsButton.Draw();
    creditsButton.Draw();
    exitButton.Draw();

    // ------------------------------------
    // VERSION
    // ------------------------------------

    DrawText(
        "Prototype Build",
        20,
        GetScreenHeight() - 35,
        18,
        GRAY
    );
}
