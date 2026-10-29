#include "ResourceHUD.h"

#include <string>
#include <sstream>
#include <iomanip>

using namespace std;


namespace
{
    const char* resourceNames[RESOURCE_COUNT] =
    {
        "MANA",
        "WOOD",
        "FOOD",
        "WATER",
        "STONE",
        "MONEY"
    };

    const char* resourceIcons[RESOURCE_COUNT] =
    {
        "assets/ui/resources/mana.png",
        "assets/ui/resources/wood.png",
        "assets/ui/resources/food.png",
        "assets/ui/resources/water.png",
        "assets/ui/resources/stone.png",
        "assets/ui/resources/money.png"
    };

    const char* placeholderLetters[RESOURCE_COUNT] =
    {
        "M", "W", "F", "W", "S", "$"
    };

    const Color panelColor = {37, 43, 55, 255};
    const Color borderColor = {80, 89, 106, 255};
    const Color incomeColor = {135, 216, 152, 255};

    // Форматирование до двух знаков,
    // но без ненужных нулей.
    string FormatNumber(double value)
    {
        ostringstream stream;

        stream << fixed
               << setprecision(2)
               << value;

        string result = stream.str();

        while (
            result.find('.') != string::npos &&
            result.back() == '0'
        )
        {
            result.pop_back();
        }

        if (!result.empty() && result.back() == '.')
        {
            result.pop_back();
        }

        return result;
    }
}


// =====================================
// CONSTRUCTOR / DESTRUCTOR
// =====================================

ResourceHUD::ResourceHUD()
{
    LoadIcons();
}


ResourceHUD::~ResourceHUD()
{
    for (int i = 0; i < RESOURCE_COUNT; i++)
    {
        if (iconLoaded[i])
        {
            UnloadTexture(icons[i]);
        }
    }
}


// =====================================
// LOAD ICONS
// =====================================

void ResourceHUD::LoadIcons()
{
    for (int i = 0; i < RESOURCE_COUNT; i++)
    {
        if (!FileExists(resourceIcons[i]))
            continue;

        Texture2D texture =
            LoadTexture(resourceIcons[i]);

        if (texture.id == 0)
            continue;

        SetTextureFilter(
            texture,
            TEXTURE_FILTER_POINT
        );

        icons[i] = texture;
        iconLoaded[i] = true;
    }
}


// =====================================
// DRAW HUD
// =====================================

void ResourceHUD::Draw(
    const ResourceSystem& resources
) const
{
    const float margin = 24.0f;
    const float gap = 8.0f;
    const float top = 12.0f;
    const float height = 76.0f;

    float screenWidth =
        static_cast<float>(GetScreenWidth());

    float availableWidth = screenWidth - 2 * margin;

    float cardWidth =
        (availableWidth -
         gap * (RESOURCE_COUNT - 1)) /
        RESOURCE_COUNT;

    if (cardWidth < 1.0f)
        return;

    for (int i = 0; i < RESOURCE_COUNT; i++)
    {
        float x = margin + i * (cardWidth + gap);

        Rectangle panel =
        {
            x, top, cardWidth, height
        };

        // Временная плашка.
        // Позже заменим на твой PNG.
        DrawRectangleRec(panel, panelColor);

        DrawRectangleLinesEx(
            panel,
            1.0f,
            borderColor
        );

        // Иконка ресурса 32x32.
        if (iconLoaded[i])
        {
            const Texture2D& texture = icons[i];

            DrawTexturePro(
                texture,
                {
                    0, 0,
                    static_cast<float>(texture.width),
                    static_cast<float>(texture.height)
                },
                {
                    x + 9, top + 21,
                    32, 32
                },
                {0, 0},
                0.0f,
                WHITE
            );
        }
        else
        {
            // Заглушка, если картинки нет.
            DrawRectangle(
                static_cast<int>(x + 9),
                static_cast<int>(top + 21),
                32, 32,
                Color{66, 76, 91, 255}
            );

            DrawText(
                placeholderLetters[i],
                static_cast<int>(x + 17),
                static_cast<int>(top + 28),
                18,
                LIGHTGRAY
            );
        }

        // Название ресурса.
        DrawText(
            resourceNames[i],
            static_cast<int>(x + 50),
            static_cast<int>(top + 7),
            16,
            LIGHTGRAY
        );

        // Текущее количество.
        string amount =
            FormatNumber(resources.amounts[i]);

        DrawText(
            amount.c_str(),
            static_cast<int>(x + 50),
            static_cast<int>(top + 28),
            20,
            RAYWHITE
        );

        // Прирост в секунду.
        string rate;

        if (i == static_cast<int>(ResourceType::Money))
        {
            rate = "MARKET";
        }
        else
        {
            rate = "+" +
                FormatNumber(resources.rates[i]) +
                "/s";
        }

        DrawText(
            rate.c_str(),
            static_cast<int>(x + 50),
            static_cast<int>(top + 54),
            14,
            incomeColor
        );
    }
}
