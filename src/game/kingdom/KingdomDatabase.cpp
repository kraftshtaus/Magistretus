
#include "KingdomDatabase.h"

#include <stdexcept>

using namespace std;

const vector<KingdomData>& KingdomDatabase::GetAll()
{
    static const vector<KingdomData> kingdoms =
    {
        {
            KingdomType::Red,
            "kingdom_red_name",
            "kingdom_red_description",
            "assets/kingdoms/red.png",

            { {"unit_damage", 0.15f} },
            { {"food_production", -0.10f} }
        },

        {
            KingdomType::Blue,
            "kingdom_blue_name",
            "kingdom_blue_description",
            "assets/kingdoms/blue.png",

            { {"research_speed", 0.15f} },
            { {"unit_health", -0.10f} }
        },

        {
            KingdomType::Green,
            "kingdom_green_name",
            "kingdom_green_description",
            "assets/kingdoms/green.png",

            { {"food_production", 0.20f} },
            { {"stone_production", -0.10f} }
        },

        {
            KingdomType::Gold,
            "kingdom_gold_name",
            "kingdom_gold_description",
            "assets/kingdoms/gold.png",

            { {"gold_production", 0.20f} },
            { {"building_cost", 0.15f} }
        }
    };

    return kingdoms;
}

const KingdomData& KingdomDatabase::Get(KingdomType type)
{
    for (const KingdomData& kingdom : GetAll())
    {
        if (kingdom.type == type)
        {
            return kingdom;
        }
    }

    throw invalid_argument("Unknown kingdom type");
}
