#include "SettingsMenu.h"

using namespace std;


SettingsMenu::SettingsMenu(Font* uiFont)
    : englishButton(
        Rectangle{ 440, 280, 400, 60 },
        "English",
        uiFont
    ),
      russianButton(
        Rectangle{ 440, 360, 400, 60 },
        "Russian",
        uiFont
    ),
      backButton(
        Rectangle{ 40, 620, 180, 55 },
        "Back",
        uiFont
    ),
      uiFont(uiFont)
{
}


void SettingsMenu::Update(
    AppState& currentState,
    LocalizationManager& localization
)
{
    if (englishButton.IsPressed())
    {
        localization.LoadLanguage("en");
    }

    if (russianButton.IsPressed())
    {
        localization.LoadLanguage("ru");
    }


    if (
        backButton.IsPressed() ||
        IsKeyPressed(KEY_ESCAPE)
    )
    {
        currentState =
            AppState::MainMenu;
    }
}


void SettingsMenu::Draw(
    LocalizationManager& localization
)
{
    englishButton.SetText(
        localization.Get(
            "settings.english"
        )
    );

    russianButton.SetText(
        localization.Get(
            "settings.russian"
        )
    );

    backButton.SetText(
        localization.Get(
            "save.back"
        )
    );


    string title =
        localization.Get(
            "settings.title"
        );


    const float titleSize = 48.0f;
    const float spacing = 1.0f;


    Vector2 titleMeasure =
        MeasureTextEx(
            *uiFont,
            title.c_str(),
            titleSize,
            spacing
        );


    DrawTextEx(
        *uiFont,
        title.c_str(),
        Vector2{
            GetScreenWidth() / 2.0f -
            titleMeasure.x / 2.0f,

            130.0f
        },
        titleSize,
        spacing,
        RAYWHITE
    );


    string languageText =
        localization.Get(
            "settings.language"
        );


    Vector2 languageMeasure =
        MeasureTextEx(
            *uiFont,
            languageText.c_str(),
            26.0f,
            spacing
        );


    DrawTextEx(
        *uiFont,
        languageText.c_str(),
        Vector2{
            GetScreenWidth() / 2.0f -
            languageMeasure.x / 2.0f,

            220.0f
        },
        26.0f,
        spacing,
        LIGHTGRAY
    );


    englishButton.Draw();
    russianButton.Draw();
    backButton.Draw();
}