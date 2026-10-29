
#pragma once

#include "KingdomData.h"

#include <vector>

using namespace std;

class KingdomDatabase
{
public:
    static const vector<KingdomData>& GetAll();

    static const KingdomData& Get(KingdomType type);
};
