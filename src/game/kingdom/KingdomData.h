
#pragma once

#include "KingdomType.h"

#include <string>
#include <vector>

using namespace std;

struct KingdomModifier
{
    string id;
    float value = 0.0f;
};

struct KingdomData
{
    KingdomType type = KingdomType::Red;

    string nameKey;
    string descriptionKey;

    string artworkPath;

    vector<KingdomModifier> bonuses;
    vector<KingdomModifier> penalties;
};
