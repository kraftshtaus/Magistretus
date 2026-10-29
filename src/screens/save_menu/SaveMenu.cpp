
#include "SaveMenu.h"

#include "../../save/SaveManager.h"

using namespace std;


namespace
{
    const Color panelColor =
    {
        37, 43, 55, 255
    };

    const Color goldColor =
    {
        220, 181, 91, 255
    };

    const Color redColor =
    {
        175, 65, 65, 255
    };


    bool IsClicked(Rectangle rectangle)
    {
        return
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            CheckCollisionPointRec(
                GetMousePosition(),
                rectangle
            );
    }


    const Rectangle confirmButton =
    {
        690, 410, 200, 55
    };

    const Rectangle cancelButton =
    {
        390, 410, 200, 55
    };
}


// ----------------------------------
// CONSTRUCTOR
// ----------------------------------

SaveMenu::SaveMenu(Font* uiFont)
    : slot1Button(
        Rectangle{390, 190, 500, 70},
        "SAVE SLOT 1",
        uiFont
    ),
      slot2Button(
        Rectangle{390, 285, 500, 70},
        "SAVE SLOT 2",
        uiFont
    ),
      slot3Button(
        Rectangle{390, 380, 500, 70},
        "SAVE SLOT 3",
        uiFont
    ),
      slot4Button(
        Rectangle{390, 475, 500, 70},
        "SAVE SLOT 4",
        uiFont
    ),
      backButton(
        Rectangle{40, 620, 180, 55},
        "BACK",
        uiFont
    ),
      uiFont(uiFont)
{
    RefreshSlots();
}


// ----------------------------------
// REFRESH SLOTS
// ----------------------------------

void SaveMenu::RefreshSlots()
{
    for (int slot = 1; slot <= 4; slot++)
    {
        SaveData data;

        if (SaveManager::SlotExists(slot))
        {
            data = SaveManager::LoadSave(slot);
        }

        cachedSlots[slot - 1] = data;
    }
}


// ----------------------------------
// LOAD MODE
// ----------------------------------

void SaveMenu::BeginLoad()
{
    mode = SaveMenuMode::Load;

    pendingOverwriteSlot = 0;

    statusMessage = "";

    newSaveCreated = false;

    RefreshSlots();
}


// ----------------------------------
// CREATE MODE
// ----------------------------------

void SaveMenu::BeginCreate(
    KingdomType kingdom,
    int difficulty,
    const string& kingdomName
)
{
    mode = SaveMenuMode::Create;

    newKingdom = kingdom;
    newDifficulty = difficulty;
    newKingdomName = kingdomName;

    pendingOverwriteSlot = 0;

    statusMessage = "";

    newSaveCreated = false;

    RefreshSlots();
}


// ----------------------------------
// OPEN SLOT
// ----------------------------------

void SaveMenu::OpenSlot(
    int slot,
    AppState& currentState,
    int& selectedSlot
)
{
    if (slot < 1 || slot > 4)
    {
        return;
    }

    // Режим загрузки существующей игры.
    if (mode == SaveMenuMode::Load)
    {
        if (!SaveManager::SlotExists(slot))
        {
            statusMessage =
                "EMPTY SLOT - USE NEW GAME";

            RefreshSlots();
            return;
        }

        SaveData save =
            SaveManager::LoadSave(slot);

        if (!save.exists)
        {
            statusMessage =
                "FAILED TO LOAD SAVE";

            return;
        }

        selectedSlot = slot;

        currentState = AppState::Game;

        return;
    }

    // Режим создания нового королевства.
    if (SaveManager::SlotExists(slot))
    {
        // Слот занят.
        // Сначала спрашиваем разрешение.
        pendingOverwriteSlot = slot;

        statusMessage = "";

        return;
    }

    // Пустой слот можно использовать сразу.
    CreateInSlot(
        slot,
        false,
        currentState,
        selectedSlot
    );
}


// ----------------------------------
// CREATE KINGDOM SAVE
// ----------------------------------

void SaveMenu::CreateInSlot(
    int slot,
    bool overwrite,
    AppState& currentState,
    int& selectedSlot
)
{
    SaveData createdSave;

    bool success =
        SaveManager::CreateKingdomSave(
            slot,
            newKingdom,
            newDifficulty,
            newKingdomName,
            overwrite,
            createdSave
        );

    if (!success)
    {
        statusMessage =
            "FAILED TO CREATE SAVE";

        pendingOverwriteSlot = 0;

        RefreshSlots();

        return;
    }

    // Новое сохранение создано.
    newSaveCreated = true;

    selectedSlot = slot;

    pendingOverwriteSlot = 0;

    statusMessage = "";

    RefreshSlots();

    mode = SaveMenuMode::Load;

    currentState = AppState::Game;
}


// ----------------------------------
// OVERWRITE CONFIRMATION
// ----------------------------------

void SaveMenu::UpdateOverwriteConfirmation(
    AppState& currentState,
    int& selectedSlot
)
{
    if (pendingOverwriteSlot == 0)
    {
        return;
    }

    // Отмена перезаписи.
    if (
        IsClicked(cancelButton) ||
        IsKeyPressed(KEY_ESCAPE) ||
        IsKeyPressed(KEY_N)
    )
    {
        pendingOverwriteSlot = 0;
        return;
    }

    // Подтверждение перезаписи.
    if (
        IsClicked(confirmButton) ||
        IsKeyPressed(KEY_Y)
    )
    {
        int slot = pendingOverwriteSlot;

        pendingOverwriteSlot = 0;

        CreateInSlot(
            slot,
            true,
            currentState,
            selectedSlot
        );

        return;
    }
}


// ----------------------------------
// UPDATE
// ----------------------------------

void SaveMenu::Update(
    AppState& currentState,
    int& selectedSlot
)
{
    // Открыто модальное окно.
    // Остальные кнопки временно недоступны.
    if (pendingOverwriteSlot != 0)
    {
        UpdateOverwriteConfirmation(
            currentState,
            selectedSlot
        );

        return;
    }

    if (slot1Button.IsPressed())
    {
        OpenSlot(
            1,
            currentState,
            selectedSlot
        );

        return;
    }

    if (slot2Button.IsPressed())
    {
        OpenSlot(
            2,
            currentState,
            selectedSlot
        );

        return;
    }

    if (slot3Button.IsPressed())
    {
        OpenSlot(
            3,
            currentState,
            selectedSlot
        );

        return;
    }

    if (slot4Button.IsPressed())
    {
        OpenSlot(
            4,
            currentState,
            selectedSlot
        );

        return;
    }

    // Возвращение назад.
    if (
        backButton.IsPressed() ||
        IsKeyPressed(KEY_ESCAPE)
    )
    {
        if (mode == SaveMenuMode::Create)
        {
            // Возвращаемся к настройкам
            // создаваемого королевства.
            currentState = AppState::NewGame;

            mode = SaveMenuMode::Load;
        }
        else
        {
            currentState = AppState::MainMenu;
        }

        pendingOverwriteSlot = 0;

        statusMessage = "";

        return;
    }
}


// ----------------------------------
// DRAW LABEL
// ----------------------------------

void SaveMenu::DrawLabel(
    const string& text,
    float x,
    float y,
    float size,
    Color color
) const
{
    DrawTextEx(
        *uiFont,
        text.c_str(),
        Vector2{x, y},
        size,
        1.0f,
        color
    );
}


// ----------------------------------
// DRAW SLOT INFO
// ----------------------------------

void SaveMenu::DrawSlotInfo(
    int slot,
    int x,
    int y,
    LocalizationManager& localization
) const
{
    if (slot < 1 || slot > 4)
    {
        return;
    }

    // Используем кэш вместо
    // постоянного чтения с диска.
    const SaveData& data =
        cachedSlots[slot - 1];

    string info;

    if (!data.exists)
    {
        info = localization.Get("save.empty");

        if (mode == SaveMenuMode::Create)
        {
            info += " | AVAILABLE";
        }
    }
    else
    {
        if (
            data.hasKingdomSettings &&
            !data.kingdomName.empty()
        )
        {
            info += data.kingdomName;
            info += " | ";
        }

        info += localization.Get("save.day");

        info += " ";
        info += to_string(data.day);

        info += " | ";
        info += data.createdAt;

        if (mode == SaveMenuMode::Create)
        {
            info += " | OCCUPIED";
        }
    }

    // Не допускаем выхода текста
    // за границы области слота.
    BeginScissorMode(
        x,
        y,
        480,
        25
    );

    DrawLabel(
        info,
        static_cast<float>(x),
        static_cast<float>(y),
        18.0f,
        data.exists ? LIGHTGRAY : GRAY
    );

    EndScissorMode();
}


// ----------------------------------
// DRAW OVERWRITE CONFIRMATION
// ----------------------------------

void SaveMenu::DrawOverwriteConfirmation() const
{
    // Затемняем меню на заднем плане.
    DrawRectangle(
        0,
        0,
        GetScreenWidth(),
        GetScreenHeight(),
        Fade(BLACK, 0.75f)
    );

    Rectangle panel =
    {
        325, 210, 630, 290
    };

    DrawRectangleRec(
        panel,
        panelColor
    );

    DrawRectangleLinesEx(
        panel,
        2.0f,
        redColor
    );

    DrawLabel(
        "OVERWRITE SAVE?",
        400, 240,
        34.0f,
        RAYWHITE
    );

    string slotText =
        "SAVE SLOT " +
        to_string(pendingOverwriteSlot);

    DrawLabel(
        slotText,
        400, 300,
        25.0f,
        goldColor
    );

    DrawLabel(
        "The existing kingdom will be deleted.",
        400, 342,
        20.0f,
        LIGHTGRAY
    );

    DrawLabel(
        "This action cannot be undone.",
        400, 370,
        18.0f,
        RED
    );

    // Кнопка отмены.
    DrawRectangleRec(
        cancelButton,
        Color{65, 72, 84, 255}
    );

    DrawRectangleLinesEx(
        cancelButton,
        1.0f,
        GRAY
    );

    DrawLabel(
        "CANCEL",
        437, 423,
        24.0f,
        RAYWHITE
    );

    // Кнопка подтверждения.
    DrawRectangleRec(
        confirmButton,
        redColor
    );

    DrawLabel(
        "OVERWRITE",
        714, 423,
        24.0f,
        RAYWHITE
    );
}


// ----------------------------------
// DRAW
// ----------------------------------

void SaveMenu::Draw(
    LocalizationManager& localization
)
{
    slot1Button.SetText(
        localization.Get("save.slot1")
    );

    slot2Button.SetText(
        localization.Get("save.slot2")
    );

    slot3Button.SetText(
        localization.Get("save.slot3")
    );

    slot4Button.SetText(
        localization.Get("save.slot4")
    );

    backButton.SetText(
        localization.Get("save.back")
    );

    string title;

    if (mode == SaveMenuMode::Create)
    {
        title = "CHOOSE A SAVE SLOT";
    }
    else
    {
        title = localization.Get("save.title");
    }

    float titleSize = 44.0f;

    Vector2 size = MeasureTextEx(
        *uiFont,
        title.c_str(),
        titleSize,
        1.0f
    );

    DrawLabel(
        title,
        GetScreenWidth() / 2.0f - size.x / 2.0f,
        80.0f,
        titleSize,
        RAYWHITE
    );

    // При создании отображаем название
    // выбранного королевства.
    if (mode == SaveMenuMode::Create)
    {
        string description =
            newKingdomName +
            " | Difficulty: " +
            to_string(newDifficulty);

        Vector2 descriptionSize =
            MeasureTextEx(
                *uiFont,
                description.c_str(),
                20.0f,
                1.0f
            );

        DrawLabel(
            description,
            GetScreenWidth() / 2.0f -
                descriptionSize.x / 2.0f,
            145.0f,
            20.0f,
            goldColor
        );
    }

    // Четыре слота.
    slot1Button.Draw();
    slot2Button.Draw();
    slot3Button.Draw();
    slot4Button.Draw();

    backButton.Draw();

    // Подписи под слотами.
    DrawSlotInfo(
        1, 410, 263, localization
    );

    DrawSlotInfo(
        2, 410, 358, localization
    );

    DrawSlotInfo(
        3, 410, 453, localization
    );

    DrawSlotInfo(
        4, 410, 548, localization
    );

    // Сообщение пользователю.
    if (!statusMessage.empty())
    {
        DrawLabel(
            statusMessage,
            390, 590,
            20.0f,
            goldColor
        );
    }

    // Модальное окно поверх меню.
    if (pendingOverwriteSlot != 0)
    {
        DrawOverwriteConfirmation();
    }
}


// ----------------------------------
// NEW SAVE EVENT
// ----------------------------------

bool SaveMenu::TakeNewSaveCreated()
{
    if (!newSaveCreated)
    {
        return false;
    }

    newSaveCreated = false;

    return true;
}
