
#pragma once

#include <raylib.h>

#include <array>
#include <string>

#include "../../ui/Button.h"
#include "../../core/AppState.h"
#include "../../save/SaveData.h"
#include "../../localization/LocalizationManager.h"

using namespace std;


// Два режима работы меню сохранений.
enum class SaveMenuMode
{
    Load,
    Create
};


class SaveMenu
{
public:
    SaveMenu(Font* uiFont);

    // Обычная загрузка сохранений.
    void BeginLoad();

    // Выбор слота для нового королевства.
    void BeginCreate(
        KingdomType kingdom,
        int difficulty,
        const string& kingdomName
    );

    void Update(
        AppState& currentState,
        int& selectedSlot
    );

    void Draw(
        LocalizationManager& localization
    );

    // Сообщает Application, что была
    // создана новая игра.
    // После чтения флаг сбрасывается.
    bool TakeNewSaveCreated();

private:
    Button slot1Button;
    Button slot2Button;
    Button slot3Button;
    Button slot4Button;

    Button backButton;

    Font* uiFont = nullptr;

    SaveMenuMode mode = SaveMenuMode::Load;

    // Настройки выбранного королевства.
    KingdomType newKingdom = KingdomType::Red;

    int newDifficulty = 2;

    string newKingdomName = "";

    // Кэш информации о слотах.
    array<SaveData, 4> cachedSlots;

    // 0 - окно подтверждения закрыто.
    // 1-4 - номер слота для перезаписи.
    int pendingOverwriteSlot = 0;

    // Новое сохранение успешно создано.
    bool newSaveCreated = false;

    // Сообщения об ошибках и подсказки.
    string statusMessage = "";

    // Перечитать информацию о слотах.
    void RefreshSlots();

    // Обработка выбранного слота.
    void OpenSlot(
        int slot,
        AppState& currentState,
        int& selectedSlot
    );

    // Создать королевство в слоте.
    void CreateInSlot(
        int slot,
        bool overwrite,
        AppState& currentState,
        int& selectedSlot
    );

    // Обработка подтверждения перезаписи.
    void UpdateOverwriteConfirmation(
        AppState& currentState,
        int& selectedSlot
    );

    // Отрисовка информации о слоте.
    void DrawSlotInfo(
        int slot,
        int x,
        int y,
        LocalizationManager& localization
    ) const;

    // Отрисовка окна перезаписи.
    void DrawOverwriteConfirmation() const;

    // Вспомогательная функция текста.
    void DrawLabel(
        const string& text,
        float x,
        float y,
        float size,
        Color color
    ) const;
};
