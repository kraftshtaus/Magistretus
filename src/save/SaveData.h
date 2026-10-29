
#pragma once

#include <string>
#include <array>

#include "../game/kingdom/KingdomType.h"
#if __has_include("../screens/game/resources/ResourceSystem.h")
#include "../screens/game/resources/ResourceSystem.h"
#else
#include "../game/resources/ResourceSystem.h"
#endif

using namespace std;


struct SaveData
{
    bool exists = false;

    int slot = 0;
    int day = 1;

    string createdAt = "";
    string worldData = "";

    // Королевство.
    bool hasKingdomSettings = false;

    KingdomType kingdomType = KingdomType::Red;
    int difficulty = 2;
    string kingdomName = "";

    // Экономика.
    // Старые сохранения этих данных не содержат.
    bool hasResourceData = false;

    array<double, RESOURCE_COUNT> resourceAmounts = {};
};
