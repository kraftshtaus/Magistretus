
#pragma once

#include "../../ui/Button.h"
#include "../../core/AppState.h"
#include "../../localization/LocalizationManager.h"

using namespace std;

// Главное меню Magistretus.
// Позволяет создать новую игру,
// загрузить сохранение и открыть настройки.
class MainMenu
{
public:
    MainMenu(Font* uiFont);

    void Update(
        AppState& currentState
    );

    void Draw(
        LocalizationManager& localization
    );

private:
    Button newGameButton;
    Button playButton;
    Button settingsButton;
    Button creditsButton;
    Button exitButton;
};
