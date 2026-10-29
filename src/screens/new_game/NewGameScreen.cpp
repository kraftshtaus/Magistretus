
#include "NewGameScreen.h"

#include "../../game/kingdom/KingdomDatabase.h"

#include <algorithm>
#include <cmath>

using namespace std;


namespace
{
    const Color backgroundColor = {24, 28, 37, 255};
    const Color panelColor = {37, 43, 55, 255};
    const Color panelLight = {48, 55, 69, 255};
    const Color goldColor = {220, 181, 91, 255};

    const Color bonusColor = {128, 211, 151, 255};
    const Color penaltyColor = {224, 129, 129, 255};

    const Color kingdomColors[5] =
    {
        {190, 70, 75, 255},
        {75, 126, 207, 255},
        {76, 170, 111, 255},
        {222, 176, 72, 255},
        {40, 40, 40, 255}
    };

    const char* kingdomNames[5] =
    {
        "RED KINGDOM",
        "BLUE KINGDOM",
        "GREEN KINGDOM",
        "GOLD KINGDOM",
        "BLACK KINGDOM"
    };

    const char* crownPaths[5] =
    {
        "assets/kingdoms/crowns/red.png",
        "assets/kingdoms/crowns/blue.png",
        "assets/kingdoms/crowns/green.png",
        "assets/kingdoms/crowns/gold.png",
        "assets/kingdoms/crowns/black.png"
    };

    const char* difficultyPaths[2] =
    {
        "assets/ui/difficulty/difficulty-null.png",
        "assets/ui/difficulty/difficulty.png"
    };

    const char* difficultyNames[3] =
    {
        "EASY - 1 CROWN",
        "NORMAL - 2 CROWNS",
        "HARD - 3 CROWNS"
    };

    const Rectangle leftArrow = {77, 198, 64, 82};
    const Rectangle rightArrow = {519, 198, 64, 82};

    const Rectangle backButton = {1080, 32, 150, 48};
    const Rectangle nameField = {685, 239, 480, 62};
    const Rectangle startButton = {740, 509, 390, 70};

    // Расположение строки сложности.
    const float difficultyStartX = 797.0f;
    const float difficultyY = 381.0f;
    const float difficultySizeX = 64.0f;
    const float difficultySizeY = 48.0f;
    const float difficultyStep = 96.0f;


    bool IsClicked(Rectangle area)
    {
        return IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
            && CheckCollisionPointRec(
                GetMousePosition(), area
            );
    }


    bool IsHovered(Rectangle area)
    {
        return CheckCollisionPointRec(
            GetMousePosition(), area
        );
    }


    string ModifierLabel(const string& id)
    {
        if (id == "unit_damage") return "Unit damage";
        if (id == "unit_health") return "Unit health";
        if (id == "research_speed") return "Research speed";
        if (id == "food_production") return "Food production";
        if (id == "stone_production") return "Stone production";
        if (id == "gold_production") return "Gold production";
        if (id == "building_cost") return "Building cost";

        return id;
    }


    string FormatModifier(const KingdomModifier& modifier)
    {
        int percent = static_cast<int>(
            round(modifier.value * 100.0f)
        );

        return ModifierLabel(modifier.id)
            + ": "
            + (percent >= 0 ? "+" : "")
            + to_string(percent)
            + "%";
    }


    void RemoveLastCharacter(string& text)
    {
        if (text.empty())
            return;

        size_t index = text.size() - 1;

        while (index > 0)
        {
            unsigned char c =
                static_cast<unsigned char>(text[index]);

            if ((c & 0xC0) != 0x80)
                break;

            index--;
        }

        text.erase(index);
    }
}


// ========================================
// CONSTRUCTOR / DESTRUCTOR
// ========================================

NewGameScreen::NewGameScreen(Font* uiFont)
    : font(uiFont)
{
    LoadCrownTextures();
    LoadDifficultyTextures();
    Reset();
}


NewGameScreen::~NewGameScreen()
{
    for (int i = 0; i < 4; i++)
    {
        if (crownLoaded[i])
            UnloadTexture(crownTextures[i]);
    }

    for (int i = 0; i < 2; i++)
    {
        if (difficultyLoaded[i])
            UnloadTexture(difficultyTextures[i]);
    }
}


// ========================================
// TEXTURE LOADING
// ========================================

void NewGameScreen::LoadCrownTextures()
{
    for (int i = 0; i < 4; i++)
    {
        if (!FileExists(crownPaths[i]))
            continue;

        Texture2D texture = LoadTexture(crownPaths[i]);

        if (texture.id == 0)
            continue;

        SetTextureFilter(texture, TEXTURE_FILTER_POINT);

        crownTextures[i] = texture;
        crownLoaded[i] = true;
    }
}


void NewGameScreen::LoadDifficultyTextures()
{
    for (int i = 0; i < 2; i++)
    {
        if (!FileExists(difficultyPaths[i]))
            continue;

        Texture2D texture = LoadTexture(difficultyPaths[i]);

        if (texture.id == 0)
            continue;

        SetTextureFilter(texture, TEXTURE_FILTER_POINT);

        difficultyTextures[i] = texture;
        difficultyLoaded[i] = true;
    }
}


// ========================================
// RESET
// ========================================

void NewGameScreen::Reset()
{
    settings = NewGameSettings{};

    editingName = false;
    continueRequested = false;
    backRequested = false;

    crownTransition = 0.0f;
}


// ========================================
// KINGDOM SELECTION
// ========================================

void NewGameScreen::ChangeKingdom(int direction)
{
    int index = static_cast<int>(settings.kingdom);

    index = (index + direction + 4) % 4;

    settings.kingdom = static_cast<KingdomType>(index);

    crownTransition = 1.0f;
}


bool NewGameScreen::HasValidName() const
{
    return settings.kingdomName.find_first_not_of(" \t")
        != string::npos;
}


// ========================================
// NAME INPUT
// ========================================

void NewGameScreen::UpdateNameInput()
{
    bool ctrl =
        IsKeyDown(KEY_LEFT_CONTROL) ||
        IsKeyDown(KEY_RIGHT_CONTROL);

    if (ctrl && IsKeyPressed(KEY_A))
        settings.kingdomName.clear();

    if (!ctrl)
    {
        int codepoint = GetCharPressed();

        while (codepoint > 0)
        {
            bool latin = codepoint >= 32 && codepoint <= 126;
            bool cyrillic =
                codepoint >= 0x0400 && codepoint <= 0x052F;

            if (latin || cyrillic)
            {
                int byteCount = 0;

                const char* utf8 =
                    CodepointToUTF8(codepoint, &byteCount);

                string candidate = settings.kingdomName;
                candidate.append(utf8, byteCount);

                bool fits = MeasureTextEx(
                    *font,
                    candidate.c_str(),
                    28.0f,
                    1.0f
                ).x < 435.0f;

                if (fits &&
                    GetCodepointCount(candidate.c_str()) <= 24)
                {
                    settings.kingdomName = candidate;
                }
            }

            codepoint = GetCharPressed();
        }
    }

    if (IsKeyPressed(KEY_BACKSPACE))
        RemoveLastCharacter(settings.kingdomName);
}


// ========================================
// UPDATE
// ========================================

void NewGameScreen::Update()
{
    crownTransition = max(
        0.0f,
        crownTransition - GetFrameTime() * 3.5f
    );

    if (IsClicked(backButton) || IsKeyPressed(KEY_ESCAPE))
    {
        backRequested = true;
        return;
    }

    if (
        IsClicked(leftArrow) ||
        (!editingName && IsKeyPressed(KEY_LEFT))
    )
    {
        ChangeKingdom(-1);
    }

    if (
        IsClicked(rightArrow) ||
        (!editingName && IsKeyPressed(KEY_RIGHT))
    )
    {
        ChangeKingdom(1);
    }

    // Нажатие на корону выбирает сложность.
    // Все предыдущие короны заполняются.
    for (int i = 0; i < 3; i++)
    {
        Rectangle area =
        {
            difficultyStartX + i * difficultyStep - 9,
            difficultyY - 9,
            difficultySizeX + 18,
            difficultySizeY + 18
        };

        if (IsClicked(area))
        {
            settings.difficulty = i + 1;
            editingName = false;
        }
    }

    // Работа с полем названия.
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        editingName = IsHovered(nameField);

    if (editingName)
        UpdateNameInput();

    if (IsKeyPressed(KEY_ENTER))
    {
        if (editingName)
        {
            editingName = false;
        }
        else if (HasValidName())
        {
            continueRequested = true;
        }
    }

    if (IsClicked(startButton) && HasValidName())
    {
        editingName = false;
        continueRequested = true;
    }
}


// ========================================
// TEXT HELPERS
// ========================================

void NewGameScreen::DrawLabel(
    const string& text,
    float x, float y, float size,
    Color color
) const
{
    DrawTextEx(
        *font,
        text.c_str(),
        {x, y},
        size,
        1.0f,
        color
    );
}


void NewGameScreen::DrawCenteredLabel(
    const string& text,
    float centerX, float y, float size,
    Color color
) const
{
    float width = MeasureTextEx(
        *font, text.c_str(), size, 1.0f
    ).x;

    DrawLabel(
        text, centerX - width / 2.0f,
        y, size, color
    );
}


// ========================================
// LARGE CROWN PLACEHOLDER
// ========================================

void NewGameScreen::DrawCrownPlaceholder(
    float centerX,
    float y,
    Color color
) const
{
    float x = centerX - 135.0f;

    DrawTriangle(
        {x + 18, y + 125},
        {x + 1, y + 23},
        {x + 94, y + 115},
        color
    );

    DrawTriangle(
        {x + 87, y + 125},
        {x + 135, y + 5},
        {x + 183, y + 125},
        color
    );

    DrawTriangle(
        {x + 176, y + 115},
        {x + 269, y + 23},
        {x + 252, y + 125},
        color
    );

    DrawRectangle(
        static_cast<int>(x + 22),
        static_cast<int>(y + 110),
        226, 42,
        color
    );

    DrawRectangleLinesEx(
        {x + 22, y + 110, 226, 42},
        3.0f, goldColor
    );

    DrawCircle(
        static_cast<int>(centerX),
        static_cast<int>(y + 131),
        11, goldColor
    );
}


// ========================================
// DRAW KINGDOM
// ========================================

void NewGameScreen::DrawKingdomSelection() const
{
    int index = static_cast<int>(settings.kingdom);
    Color color = kingdomColors[index];

    DrawCenteredLabel(
        "CHOOSE YOUR KINGDOM",
        330, 94, 26, LIGHTGRAY
    );

    float offset = crownTransition * 14.0f;
    float opacity = 1.0f - crownTransition * 0.4f;

    if (crownLoaded[index])
    {
        const Texture2D& texture = crownTextures[index];

        float scale = min(
            320.0f / texture.width,
            192.0f / texture.height
        );

        float width = texture.width * scale;
        float height = texture.height * scale;

        DrawTexturePro(
            texture,
            {
                0, 0,
                static_cast<float>(texture.width),
                static_cast<float>(texture.height)
            },
            {
                330 - width / 2.0f + offset,
                140 + (192 - height) / 2.0f,
                width, height
            },
            {0, 0},
            0,
            Fade(WHITE, opacity)
        );
    }
    else
    {
        DrawCrownPlaceholder(
            330 + offset,
            155,
            Fade(color, opacity)
        );
    }

    Color leftColor =
        IsHovered(leftArrow) ? goldColor : LIGHTGRAY;

    Color rightColor =
        IsHovered(rightArrow) ? goldColor : LIGHTGRAY;

    DrawTriangle(
        {92, 239}, {128, 215}, {128, 263},
        leftColor
    );

    DrawTriangle(
        {568, 239}, {532, 215}, {532, 263},
        rightColor
    );

    DrawCenteredLabel(
        kingdomNames[index],
        330, 341, 32, color
    );
}


// ========================================
// DRAW MODIFIERS
// ========================================

void NewGameScreen::DrawModifiers() const
{
    const KingdomData& kingdom =
        KingdomDatabase::Get(settings.kingdom);

    int index = static_cast<int>(settings.kingdom);

    Rectangle panel = {78, 390, 500, 245};

    DrawRectangleRounded(panel, 0.08f, 8, panelColor);

    DrawRectangleLinesEx(
        panel, 2.0f, kingdomColors[index]
    );

    DrawLabel(
        "KINGDOM TRAITS",
        104, 405, 28, RAYWHITE
    );

    DrawRectangle(
        104, 447, 446, 2,
        Fade(kingdomColors[index], 0.7f)
    );

    DrawLabel("BONUS", 106, 461, 18, bonusColor);

    if (!kingdom.bonuses.empty())
    {
        DrawLabel(
            FormatModifier(kingdom.bonuses[0]),
            106, 490, 24, RAYWHITE
        );
    }

    DrawLabel(
        "PENALTY", 106, 536, 18, penaltyColor
    );

    if (!kingdom.penalties.empty())
    {
        DrawLabel(
            FormatModifier(kingdom.penalties[0]),
            106, 565, 24, RAYWHITE
        );
    }
}


// ========================================
// DRAW ONE DIFFICULTY CROWN
// ========================================

void NewGameScreen::DrawDifficultyCrown(
    int index,
    float x,
    float y
) const
{
    // Заполнены все короны до выбранного уровня.
    bool filled = index < settings.difficulty;

    int textureIndex = filled ? 1 : 0;

    Rectangle area =
    {
        x - 9, y - 9,
        difficultySizeX + 18,
        difficultySizeY + 18
    };

    bool hovered = IsHovered(area);

    // Светлая подложка позволяет видеть
    // чёрный контур пустой короны.
    Color base = hovered
        ? Color{145, 149, 157, 255}
        : Color{105, 112, 123, 255};

    DrawRectangleRounded(
        area,
        0.12f,
        6,
        base
    );

    if (index + 1 == settings.difficulty)
    {
        DrawRectangleLinesEx(
            area, 2.0f, goldColor
        );
    }

    // Рисуем оригинальный PNG в масштабе 2x.
    if (difficultyLoaded[textureIndex])
    {
        const Texture2D& texture =
            difficultyTextures[textureIndex];

        DrawTexturePro(
            texture,
            {
                0, 0,
                static_cast<float>(texture.width),
                static_cast<float>(texture.height)
            },
            {
                x, y,
                difficultySizeX,
                difficultySizeY
            },
            {0, 0},
            0.0f,
            WHITE
        );
    }
    else
    {
        // Временная заглушка, если PNG отсутствует.
        Color color = filled ? goldColor : DARKGRAY;

        DrawRectangleLinesEx(
            {x + 8, y + 14, 48, 27},
            3.0f, color
        );

        DrawTriangle(
            {x + 8, y + 24},
            {x + 15, y + 5},
            {x + 28, y + 24},
            color
        );

        DrawTriangle(
            {x + 24, y + 24},
            {x + 32, y + 2},
            {x + 40, y + 24},
            color
        );

        DrawTriangle(
            {x + 36, y + 24},
            {x + 49, y + 5},
            {x + 56, y + 24},
            color
        );
    }
}


// ========================================
// DRAW DIFFICULTY ROW
// ========================================

void NewGameScreen::DrawDifficulty() const
{
    DrawLabel(
        "DIFFICULTY",
        685, 327, 25, LIGHTGRAY
    );

    for (int i = 0; i < 3; i++)
    {
        DrawDifficultyCrown(
            i,
            difficultyStartX + i * difficultyStep,
            difficultyY
        );
    }

    DrawCenteredLabel(
        difficultyNames[settings.difficulty - 1],
        925, 456, 20, goldColor
    );
}


// ========================================
// DRAW START PANEL
// ========================================

void NewGameScreen::DrawStartPanel() const
{
    Rectangle panel = {650, 115, 550, 520};

    DrawRectangleRounded(
        panel, 0.045f, 8, panelColor
    );

    DrawRectangleLinesEx(
        panel, 2.0f, Fade(goldColor, 0.7f)
    );

    Rectangle banner = {638, 107, 575, 72};

    DrawRectangleRounded(
        banner, 0.07f, 8, panelLight
    );

    DrawRectangleLinesEx(
        banner, 2.0f, goldColor
    );

    DrawCenteredLabel(
        "YOUR REIGN BEGINS",
        925, 123, 35, RAYWHITE
    );

    DrawLabel(
        "KINGDOM NAME",
        685, 198, 24, LIGHTGRAY
    );

    DrawRectangleRec(nameField, backgroundColor);

    DrawRectangleLinesEx(
        nameField,
        editingName ? 3.0f : 1.0f,
        editingName ? goldColor : GRAY
    );

    string displayedName = settings.kingdomName.empty()
        ? "Name your kingdom..."
        : settings.kingdomName;

    DrawLabel(
        displayedName,
        701, 254, 28,
        settings.kingdomName.empty() ? GRAY : RAYWHITE
    );

    if (editingName)
    {
        float width = MeasureTextEx(
            *font,
            settings.kingdomName.c_str(),
            28.0f,
            1.0f
        ).x;

        if (static_cast<int>(GetTime() * 2) % 2 == 0)
        {
            DrawRectangle(
                static_cast<int>(703 + width),
                253, 2, 34,
                goldColor
            );
        }
    }

    // Новая строка из трёх корон.
    DrawDifficulty();

    bool enabled = HasValidName();

    DrawRectangleRounded(
        startButton,
        0.10f,
        8,
        enabled ? goldColor : GRAY
    );

    DrawCenteredLabel(
        "START",
        935, 521, 37,
        backgroundColor
    );

    DrawCenteredLabel(
        "NEXT: CHOOSE A SAVE SLOT",
        925, 590, 19,
        LIGHTGRAY
    );
}


// ========================================
// DRAW
// ========================================

void NewGameScreen::Draw() const
{
    DrawRectangle(
        0, 0, 1280, 720,
        backgroundColor
    );

    DrawLabel(
        "NEW KINGDOM",
        44, 30, 43, RAYWHITE
    );

    DrawRectangleRec(
        backButton, panelColor
    );

    DrawRectangleLinesEx(
        backButton, 1.0f, GRAY
    );

    DrawCenteredLabel(
        "BACK",
        1155, 42, 25, RAYWHITE
    );

    DrawKingdomSelection();
    DrawModifiers();
    DrawStartPanel();
}


// ========================================
// EVENTS
// ========================================

bool NewGameScreen::TakeContinue(
    NewGameSettings& result
)
{
    if (!continueRequested)
        return false;

    result = settings;
    continueRequested = false;

    return true;
}


bool NewGameScreen::TakeBack()
{
    if (!backRequested)
        return false;

    backRequested = false;
    return true;
}
