
#include "Button.h"

#include <algorithm>

using namespace std;


// ========================================
// SHARED BUTTON TEXTURES
// ========================================

struct ButtonSkin
{
    Texture2D normal = {};
    Texture2D hover = {};
    Texture2D pressed = {};

    ButtonSkin()
    {
        normal = LoadButtonTexture(
            "assets/ui/main_menu/button_normal.png"
        );

        hover = LoadButtonTexture(
            "assets/ui/main_menu/button_hover.png"
        );

        pressed = LoadButtonTexture(
            "assets/ui/main_menu/button_pressed.png"
        );
    }

    ~ButtonSkin()
    {
        if (normal.id != 0)
            UnloadTexture(normal);

        if (hover.id != 0)
            UnloadTexture(hover);

        if (pressed.id != 0)
            UnloadTexture(pressed);
    }

private:

    static Texture2D LoadButtonTexture(
        const char* path
    )
    {
        Texture2D texture = {};

        if (!FileExists(path))
            return texture;

        texture = LoadTexture(path);

        if (texture.id != 0)
        {
            // Сохраняем чёткие пиксели.
            SetTextureFilter(
                texture,
                TEXTURE_FILTER_POINT
            );
        }

        return texture;
    }
};


// Один набор текстур для всех кнопок,
// которые используют PixelArt.
namespace
{
    weak_ptr<ButtonSkin> sharedButtonSkin;
}


// ========================================
// CONSTRUCTOR
// ========================================

Button::Button(
    Rectangle bounds,
    const string& text,
    Font* font
)
    : bounds(bounds),
      text(text),
      font(font)
{
}


// ========================================
// SET STYLE
// ========================================

void Button::SetStyle(ButtonStyle newStyle)
{
    style = newStyle;

    if (style == ButtonStyle::Classic)
    {
        skin.reset();
        return;
    }

    // Если текстуры уже загружены,
    // используем существующий набор.
    skin = sharedButtonSkin.lock();

    if (!skin)
    {
        skin = make_shared<ButtonSkin>();

        sharedButtonSkin = skin;
    }
}


// ========================================
// CLICK CHECK
// ========================================

bool Button::IsPressed() const
{
    Vector2 mousePosition = GetMousePosition();

    bool hovered = CheckCollisionPointRec(
        mousePosition,
        bounds
    );

    return hovered &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}


// ========================================
// DRAW
// ========================================

void Button::Draw() const
{
    Vector2 mousePosition = GetMousePosition();

    bool hovered = CheckCollisionPointRec(
        mousePosition,
        bounds
    );

    bool held = hovered &&
        IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    Color textColor = RAYWHITE;

    bool textureDrawn = false;

    // ------------------------------------
    // PIXEL ART STYLE
    // ------------------------------------

    if (
        style == ButtonStyle::PixelArt &&
        skin
    )
    {
        const Texture2D* texture = nullptr;

        if (held && skin->pressed.id != 0)
        {
            texture = &skin->pressed;
        }
        else if (hovered && skin->hover.id != 0)
        {
            texture = &skin->hover;
        }
        else if (skin->normal.id != 0)
        {
            texture = &skin->normal;
        }

        if (texture != nullptr)
        {
            Rectangle source =
            {
                0,
                0,
                static_cast<float>(texture->width),
                static_cast<float>(texture->height)
            };

            Rectangle destination = bounds;

            DrawTexturePro(
                *texture,
                source,
                destination,
                Vector2{0, 0},
                0.0f,
                WHITE
            );

            textureDrawn = true;

            // На ярких кнопках тёмный текст.
            if (hovered)
            {
                textColor = Color{
                    20, 22, 30, 255
                };
            }
        }
    }

    // ------------------------------------
    // CLASSIC / FALLBACK STYLE
    // ------------------------------------

    if (!textureDrawn)
    {
        Color normalColor =
        {
            52, 58, 72, 255
        };

        Color hoverColor =
        {
            78, 88, 108, 255
        };

        Color currentColor =
            hovered ? hoverColor : normalColor;

        DrawRectangleRec(
            bounds,
            currentColor
        );

        DrawRectangleLinesEx(
            bounds,
            2.0f,
            Color{150, 160, 175, 255}
        );

        textColor = RAYWHITE;
    }

    // ------------------------------------
    // BUTTON TEXT
    // ------------------------------------

    if (font == nullptr)
        return;

    float fontSize = 24.0f;
    float spacing = 1.0f;

    Vector2 textSize = MeasureTextEx(
        *font,
        text.c_str(),
        fontSize,
        spacing
    );

    // Уменьшаем длинные надписи,
    // чтобы они помещались в кнопку.
    float availableWidth = max(
        1.0f,
        bounds.width - 36.0f
    );

    if (textSize.x > availableWidth)
    {
        fontSize *= availableWidth / textSize.x;

        fontSize = max(fontSize, 14.0f);

        textSize = MeasureTextEx(
            *font,
            text.c_str(),
            fontSize,
            spacing
        );
    }

    Vector2 textPosition =
    {
        bounds.x +
            (bounds.width - textSize.x) / 2.0f,

        bounds.y +
            (bounds.height - textSize.y) / 2.0f
    };

    DrawTextEx(
        *font,
        text.c_str(),
        textPosition,
        fontSize,
        spacing,
        textColor
    );
}


// ========================================
// SET TEXT
// ========================================

void Button::SetText(const string& newText)
{
    text = newText;
}
