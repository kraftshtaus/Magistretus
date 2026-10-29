#include "TerrainTextures.h"

using namespace std;

bool TerrainTextures::Load()
{
    grass = LoadTexture(
        "assets/textures/terrain/grass.png"
    );

    mana = LoadTexture(
        "assets/textures/terrain/mana.png"
    );

    water = LoadTexture(
        "assets/textures/terrain/water.png"
    );

    forest = LoadTexture(
        "assets/textures/terrain/forest.png"
    );

    quarry = LoadTexture(
        "assets/textures/terrain/quarry.png"
    );

    crystal = LoadTexture(
        "assets/textures/terrain/crystal.png"
    );


    grassLoaded = IsTextureValid(grass);
    manaLoaded = IsTextureValid(mana);
    waterLoaded = IsTextureValid(water);
    forestLoaded = IsTextureValid(forest);
    quarryLoaded = IsTextureValid(quarry);
    crystalLoaded = IsTextureValid(crystal);


    return grassLoaded;
}


void TerrainTextures::Unload()
{
    if (grassLoaded)
    {
        UnloadTexture(grass);
    }

    if (manaLoaded)
    {
        UnloadTexture(mana);
    }

    if (waterLoaded)
    {
        UnloadTexture(water);
    }

    if (forestLoaded)
    {
        UnloadTexture(forest);
    }

    if (quarryLoaded)
    {
        UnloadTexture(quarry);
    }

    if (crystalLoaded)
    {
        UnloadTexture(crystal);
    }
}