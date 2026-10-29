#pragma once

#include "../../ui/Button.h"
#include "../../core/AppState.h"
#include "../../localization/LocalizationManager.h"

using namespace std;


class SettingsMenu
{
public:
    SettingsMenu(Font* uiFont);

    void Update(
        AppState& currentState,
        LocalizationManager& localization
    );

    void Draw(
        LocalizationManager& localization
    );

private:
    Button englishButton;
    Button russianButton;
    Button backButton;

    Font* uiFont;
};