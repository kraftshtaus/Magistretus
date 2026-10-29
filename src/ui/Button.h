
#pragma once

#include <raylib.h>

#include <string>
#include <memory>

using namespace std;


// Доступные стили кнопок.
enum class ButtonStyle
{
    Classic,
    PixelArt
};


// Общие текстуры пиксельных кнопок.
// Определение находится в Button.cpp.
struct ButtonSkin;


// Универсальная кнопка интерфейса.
class Button
{
public:
    Button(
        Rectangle bounds,
        const string& text,
        Font* font
    );

    // Проверка нажатия.
    bool IsPressed() const;

    // Отрисовка.
    void Draw() const;

    // Изменение текста.
    void SetText(const string& newText);

    // Изменение внешнего вида.
    void SetStyle(ButtonStyle newStyle);

private:
    Rectangle bounds;

    string text;

    Font* font = nullptr;

    ButtonStyle style = ButtonStyle::Classic;

    // Текстуры автоматически освобождаются,
    // когда больше ни одна кнопка их не использует.
    shared_ptr<ButtonSkin> skin;
};
