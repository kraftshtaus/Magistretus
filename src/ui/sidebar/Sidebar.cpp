
#include "Sidebar.h"

using namespace std;

namespace
{
    constexpr float sidebarWidth = 225.0f;
    constexpr float sidebarY = 125.0f;

    const Color panelColor = {37, 43, 55, 255};
    const Color hoverColor = {55, 65, 82, 255};
    const Color selectedColor = {77, 73, 52, 255};
    const Color goldColor = {220, 181, 91, 255};

    const char* sectionNames[4] =
    {
        "MARKET",
        "PERKS",
        "JOURNEY",
        "ARTIFACTS"
    };

    const SidebarSection sections[4] =
    {
        SidebarSection::Market,
        SidebarSection::Perks,
        SidebarSection::Journey,
        SidebarSection::Artifacts
    };

    bool IsClicked(Rectangle area)
    {
        return IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
            && CheckCollisionPointRec(
                GetMousePosition(),
                area
            );
    }
}


// =====================================
// BUTTON POSITIONS
// =====================================

Rectangle Sidebar::GetButtonBounds(int index) const
{
    float x = GetScreenWidth()
        - sidebarWidth - 18.0f;

    return Rectangle
    {
        x + 14.0f,
        sidebarY + 56.0f + index * 66.0f,
        sidebarWidth - 28.0f,
        54.0f
    };
}


// =====================================
// UPDATE
// =====================================

void Sidebar::Update()
{
    for (int i = 0; i < 4; i++)
    {
        if (IsClicked(GetButtonBounds(i)))
        {
            // Повторное нажатие закрывает раздел.
            if (activeSection == sections[i])
            {
                activeSection = SidebarSection::None;
            }
            else
            {
                activeSection = sections[i];
            }

            return;
        }
    }

    // Закрытие окна раздела крестиком.
    if (activeSection != SidebarSection::None)
    {
        float x = GetScreenWidth()
            - sidebarWidth - 18.0f
            - 390.0f - 12.0f;

        Rectangle closeButton =
        {
            x + 340.0f,
            sidebarY + 17.0f,
            30.0f,
            30.0f
        };

        if (IsClicked(closeButton))
        {
            Close();
        }
    }
}


// =====================================
// DRAW
// =====================================

void Sidebar::Draw() const
{
    float x = GetScreenWidth()
        - sidebarWidth - 18.0f;

    Rectangle sidebar =
    {
        x, sidebarY,
        sidebarWidth, 350.0f
    };

    // Временная плашка.
    DrawRectangleRec(sidebar, panelColor);

    DrawRectangleLinesEx(
        sidebar, 2.0f, GRAY
    );

    DrawText(
        "KINGDOM",
        static_cast<int>(x + 55),
        static_cast<int>(sidebarY + 17),
        21,
        RAYWHITE
    );

    for (int i = 0; i < 4; i++)
    {
        Rectangle button = GetButtonBounds(i);

        bool selected =
            activeSection == sections[i];

        bool hovered = CheckCollisionPointRec(
            GetMousePosition(),
            button
        );

        Color color = selected
            ? selectedColor
            : hovered ? hoverColor : panelColor;

        DrawRectangleRec(button, color);

        DrawRectangleLinesEx(
            button,
            selected ? 2.0f : 1.0f,
            selected ? goldColor : GRAY
        );

        int width = MeasureText(
            sectionNames[i], 20
        );

        DrawText(
            sectionNames[i],
            static_cast<int>(
                button.x + (button.width - width) / 2
            ),
            static_cast<int>(button.y + 17),
            20,
            RAYWHITE
        );
    }

    if (activeSection != SidebarSection::None)
    {
        DrawPanel();
    }
}


// =====================================
// TEMPORARY PANEL
// =====================================

void Sidebar::DrawPanel() const
{
    float sidebarX = GetScreenWidth()
        - sidebarWidth - 18.0f;

    float x = sidebarX - 390.0f - 12.0f;

    Rectangle panel =
    {
        x, sidebarY,
        390.0f, 350.0f
    };

    DrawRectangleRec(panel, panelColor);

    DrawRectangleLinesEx(
        panel, 2.0f, goldColor
    );

    const char* title = "";

    switch (activeSection)
    {
        case SidebarSection::Market:
            title = "MARKET";
            break;

        case SidebarSection::Perks:
            title = "PERKS";
            break;

        case SidebarSection::Journey:
            title = "JOURNEY";
            break;

        case SidebarSection::Artifacts:
            title = "ARTIFACTS";
            break;

        case SidebarSection::None:
            return;
    }

    DrawText(
        title,
        static_cast<int>(x + 24),
        static_cast<int>(sidebarY + 24),
        28,
        RAYWHITE
    );

    DrawText(
        "X",
        static_cast<int>(x + 348),
        static_cast<int>(sidebarY + 21),
        24,
        LIGHTGRAY
    );

    DrawLine(
        static_cast<int>(x + 24),
        static_cast<int>(sidebarY + 75),
        static_cast<int>(x + 365),
        static_cast<int>(sidebarY + 75),
        GRAY
    );

    DrawText(
        "COMING SOON",
        static_cast<int>(x + 85),
        static_cast<int>(sidebarY + 165),
        24,
        LIGHTGRAY
    );
}


// =====================================
// OTHER METHODS
// =====================================

void Sidebar::Close()
{
    activeSection = SidebarSection::None;
}

SidebarSection Sidebar::GetActiveSection() const
{
    return activeSection;
}
