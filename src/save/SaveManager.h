
#pragma once

#include "SaveData.h"

#include <string>

using namespace std;

class SaveManager
{
public:

    // Существует ли сохранение в слоте.
    static bool SlotExists(int slot);

    // Старый способ создания сохранения.
    // Сохраняем для совместимости.
    static SaveData CreateNewSave(int slot);

    // Создание королевства через New Game.
    //
    // overwriteExisting = false:
    // занятый слот нельзя перезаписать.
    //
    // overwriteExisting = true:
    // разрешается заменить существующий слот
    // после подтверждения игроком.
    static bool CreateKingdomSave(
        int slot,
        KingdomType kingdom,
        int difficulty,
        const string& kingdomName,
        bool overwriteExisting,
        SaveData& createdSave
    );

    // Загрузка данных.
    static SaveData LoadSave(int slot);

    // Сохранение всех данных.
    static bool Save(const SaveData& data);

private:

    static bool IsValidSlot(int slot);

    static string GetSavePath(int slot);

    static void EnsureSaveDirectory();

    static SaveData MakeInitialSave(int slot);
};
