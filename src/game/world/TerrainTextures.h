#pragma once

#include <raylib.h>

using namespace std;

// TerrainTextures хранит все текстуры тайлов.
// Загружаем их один раз при старте игры.
class TerrainTextures
{
public:
    bool Load();

    void Unload();

    Texture2D grass;
    Texture2D mana;
    Texture2D water;
    Texture2D forest;
    Texture2D quarry;
    Texture2D crystal;

    bool grassLoaded = false;
    bool manaLoaded = false;
    bool waterLoaded = false;
    bool forestLoaded = false;
    bool quarryLoaded = false;
    bool crystalLoaded = false;
};