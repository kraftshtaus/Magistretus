
#pragma once

#include <raylib.h>

#include "../../game/world/WorldGrid.h"
#include "../../game/world/TerrainTextures.h"
#include "../../game/resources/ResourceSystem.h"

#include "../../ui/ResourceHUD.h"
#include "../../save/SaveData.h"

#include "../../ui/sidebar/Sidebar.h"

using namespace std;

class GameScreen
{
public:
    GameScreen(TerrainTextures* terrainTextures);
    ~GameScreen();

    void OpenSave(int slot);
    void InvalidateLoadedSave();

    void Update();
    void Draw() const;

private:
    // Игровой мир.
    WorldGrid worldGrid;

    // Ресурсы.
    ResourceSystem resources;

    // Интерфейс ресурсов.
    ResourceHUD resourceHUD;

    // Текущее сохранение.
    SaveData activeSave;

    Sidebar sidebar;

    int loadedSlot = -1;

    // Автосохранение.
    float autoSaveTimer = 0.0f;

    // Камера.
    Camera2D camera = {};

    const float minZoom = 0.5f;
    const float maxZoom = 2.0f;
    const float cameraSpeed = 500.0f;

    void ResetCamera();

    bool SaveProgress();
};
