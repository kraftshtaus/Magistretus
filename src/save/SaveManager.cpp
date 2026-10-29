
#include "SaveManager.h"

#include <raylib.h>

#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cmath>

using namespace std;


namespace
{
    bool ParseInt(const string& text, int& result)
    {
        try
        {
            size_t used = 0;
            int value = stoi(text, &used);

            if (used != text.size())
                return false;

            result = value;
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    bool IsOneLine(const string& text)
    {
        return text.find_first_of("\r\n")
            == string::npos;
    }

    bool IsKnownKingdom(KingdomType type)
    {
        int id = static_cast<int>(type);
        return id >= 0 && id <= 3;
    }

    bool ValidResources(
        const array<double, RESOURCE_COUNT>& values
    )
    {
        for (double value : values)
        {
            if (!isfinite(value) || value < 0.0)
                return false;
        }

        return true;
    }

    bool ParseResources(
        const string& text,
        array<double, RESOURCE_COUNT>& result
    )
    {
        istringstream stream(text);
        array<double, RESOURCE_COUNT> parsed = {};

        string part;

        for (int i = 0; i < RESOURCE_COUNT; i++)
        {
            if (!getline(stream, part, ','))
                return false;

            try
            {
                size_t used = 0;
                parsed[i] = stod(part, &used);

                if (used != part.size())
                    return false;
            }
            catch (...)
            {
                return false;
            }
        }

        // Лишние значения не допускаются.
        if (getline(stream, part, ','))
            return false;

        if (!ValidResources(parsed))
            return false;

        result = parsed;
        return true;
    }
}


// =====================================
// FILE PATHS
// =====================================

bool SaveManager::IsValidSlot(int slot)
{
    return slot >= 1 && slot <= 4;
}


string SaveManager::GetSavePath(int slot)
{
    return "saves/slot" + to_string(slot) + ".sav";
}


void SaveManager::EnsureSaveDirectory()
{
    if (!DirectoryExists("saves"))
    {
        MakeDirectory("saves");
    }
}


bool SaveManager::SlotExists(int slot)
{
    if (!IsValidSlot(slot))
        return false;

    EnsureSaveDirectory();

    return FileExists(GetSavePath(slot).c_str());
}


// =====================================
// CREATE SAVE
// =====================================

SaveData SaveManager::MakeInitialSave(int slot)
{
    SaveData data;

    data.exists = true;
    data.slot = slot;
    data.day = 1;

    time_t now = time(nullptr);
    const tm* local = localtime(&now);

    if (local != nullptr)
    {
        ostringstream date;

        date << put_time(
            local,
            "%d.%m.%Y %H:%M"
        );

        data.createdAt = date.str();
    }

    return data;
}


SaveData SaveManager::CreateNewSave(int slot)
{
    if (!IsValidSlot(slot))
        return SaveData{};

    if (SlotExists(slot))
        return LoadSave(slot);

    SaveData data = MakeInitialSave(slot);

    if (!Save(data))
        data.exists = false;

    return data;
}


bool SaveManager::CreateKingdomSave(
    int slot,
    KingdomType kingdom,
    int difficulty,
    const string& kingdomName,
    bool overwriteExisting,
    SaveData& createdSave
)
{
    if (
        !IsValidSlot(slot) ||
        !IsKnownKingdom(kingdom) ||
        difficulty < 1 ||
        difficulty > 3 ||
        kingdomName.empty() ||
        kingdomName.size() > 120 ||
        !IsOneLine(kingdomName)
    )
    {
        return false;
    }

    if (SlotExists(slot) && !overwriteExisting)
        return false;

    SaveData data = MakeInitialSave(slot);

    data.hasKingdomSettings = true;
    data.kingdomType = kingdom;
    data.difficulty = difficulty;
    data.kingdomName = kingdomName;

    // Новая игра начинает с нуля.
    data.hasResourceData = true;
    data.resourceAmounts.fill(0.0);

    if (!Save(data))
        return false;

    createdSave = data;
    return true;
}


// =====================================
// SAVE
// =====================================

bool SaveManager::Save(const SaveData& data)
{
    if (
        !IsValidSlot(data.slot) ||
        data.day < 1 ||
        !IsOneLine(data.createdAt) ||
        !IsOneLine(data.worldData)
    )
    {
        return false;
    }

    if (data.hasKingdomSettings)
    {
        if (
            !IsKnownKingdom(data.kingdomType) ||
            data.difficulty < 1 ||
            data.difficulty > 3 ||
            data.kingdomName.empty() ||
            !IsOneLine(data.kingdomName)
        )
        {
            return false;
        }
    }

    if (
        data.hasResourceData &&
        !ValidResources(data.resourceAmounts)
    )
    {
        return false;
    }

    EnsureSaveDirectory();

    ofstream file(GetSavePath(data.slot), ios::trunc);

    if (!file.is_open())
        return false;

    file << "slot=" << data.slot << '\n';
    file << "day=" << data.day << '\n';
    file << "created=" << data.createdAt << '\n';
    file << "world=" << data.worldData << '\n';

    if (data.hasKingdomSettings)
    {
        file << "kingdomType="
             << static_cast<int>(data.kingdomType)
             << '\n';

        file << "difficulty="
             << data.difficulty
             << '\n';

        file << "kingdomName="
             << data.kingdomName
             << '\n';
    }

    if (data.hasResourceData)
    {
        file << "resources=";

        // Сохраняем дробные значения точно.
        file << setprecision(17);

        for (int i = 0; i < RESOURCE_COUNT; i++)
        {
            if (i > 0)
                file << ",";

            file << data.resourceAmounts[i];
        }

        file << '\n';
    }

    file.flush();
    return file.good();
}


// =====================================
// LOAD
// =====================================

SaveData SaveManager::LoadSave(int slot)
{
    SaveData data;
    data.slot = slot;

    if (!IsValidSlot(slot))
        return data;

    ifstream file(GetSavePath(slot));

    if (!file.is_open())
        return data;

    data.exists = true;

    bool hasType = false;
    bool hasDifficulty = false;
    bool hasName = false;

    string line;

    while (getline(file, line))
    {
        if (line.rfind("day=", 0) == 0)
        {
            int value = 0;

            if (
                ParseInt(line.substr(4), value) &&
                value >= 1
            )
            {
                data.day = value;
            }
        }
        else if (line.rfind("created=", 0) == 0)
        {
            data.createdAt = line.substr(8);
        }
        else if (line.rfind("world=", 0) == 0)
        {
            data.worldData = line.substr(6);
        }
        else if (line.rfind("kingdomType=", 0) == 0)
        {
            int value = -1;

            if (
                ParseInt(line.substr(12), value) &&
                value >= 0 &&
                value <= 3
            )
            {
                data.kingdomType =
                    static_cast<KingdomType>(value);

                hasType = true;
            }
        }
        else if (line.rfind("difficulty=", 0) == 0)
        {
            int value = 0;

            if (
                ParseInt(line.substr(11), value) &&
                value >= 1 &&
                value <= 3
            )
            {
                data.difficulty = value;
                hasDifficulty = true;
            }
        }
        else if (line.rfind("kingdomName=", 0) == 0)
        {
            data.kingdomName = line.substr(12);
            hasName = !data.kingdomName.empty();
        }
        else if (line.rfind("resources=", 0) == 0)
        {
            array<double, RESOURCE_COUNT> parsed = {};

            if (ParseResources(line.substr(10), parsed))
            {
                data.resourceAmounts = parsed;
                data.hasResourceData = true;
            }
        }
    }

    data.hasKingdomSettings =
        hasType && hasDifficulty && hasName;

    return data;
}
