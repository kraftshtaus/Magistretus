
#include "GameScreen.h"

#include "../../save/SaveManager.h"

using namespace std;


// ========================================
// CONSTRUCTOR
// ========================================

GameScreen::GameScreen(
    TerrainTextures* terrainTextures
)
    : worldGrid(terrainTextures)
{
    camera.offset = {
        640.0f,
        360.0f
    };

    camera.target = {
        512.0f,
        512.0f
    };

    camera.rotation = 0.0f;
    camera.zoom = 0.7f;
}


// ========================================
// DESTRUCTOR
// ========================================

GameScreen::~GameScreen()
{
    SaveProgress();
}


// ========================================
// RESET CAMERA
// ========================================

void GameScreen::ResetCamera()
{
    camera.target = {
        512.0f,
        512.0f
    };

    camera.zoom = 0.7f;
}


// ========================================
// INVALIDATE LOADED SAVE
// ========================================

void GameScreen::InvalidateLoadedSave()
{
    // Используется после создания
    // нового королевства.

    loadedSlot = -1;

    activeSave = SaveData{};

    resources.Reset();

    autoSaveTimer = 0.0f;
}


// ========================================
// OPEN SAVE
// ========================================

void GameScreen::OpenSave(int slot)
{
    if (slot < 1 || slot > 4)
    {
        return;
    }

    // Не загружаем один и тот же мир
    // повторно каждый кадр.
    if (loadedSlot == slot)
    {
        return;
    }

    // Сохраняем предыдущий мир
    // при переключении слота.
    if (loadedSlot >= 1)
    {
        if (!SaveProgress())
        {
            return;
        }
    }

    SaveData save =
        SaveManager::LoadSave(slot);

    // Для совместимости со старыми
    // сохранениями.
    if (!save.exists)
    {
        save =
            SaveManager::CreateNewSave(slot);
    }

    if (!save.exists)
    {
        return;
    }

    // ====================================
    // WORLD LOADING
    // ====================================

    if (save.worldData.empty())
    {
        // Создаём новый мир.
        worldGrid.Generate();

        save.worldData =
            worldGrid.Serialize();

        if (!SaveManager::Save(save))
        {
            return;
        }
    }
    else
    {
        // Загружаем существующий мир.
        bool success =
            worldGrid.Deserialize(
                save.worldData
            );

        if (!success)
        {
            // Пока сохраняем старое
            // поведение восстановления карты.
            worldGrid.Generate();

            save.worldData =
                worldGrid.Serialize();

            if (!SaveManager::Save(save))
            {
                return;
            }
        }
    }

    // ====================================
    // RESOURCE LOADING
    // ====================================

    resources.Reset();

    if (save.hasResourceData)
    {
        resources.SetAmounts(
            save.resourceAmounts
        );
    }

    // Скорости производства пока нулевые.
    // Позже рассчитаем их по объектам карты.

    activeSave = save;

    loadedSlot = slot;

    autoSaveTimer = 0.0f;

    ResetCamera();
}


// ========================================
// SAVE PROGRESS
// ========================================

bool GameScreen::SaveProgress()
{
    if (
        loadedSlot < 1 ||
        !activeSave.exists
    )
    {
        return false;
    }

    // Сохраняем текущее количество
    // ресурсов в активное сохранение.
    activeSave.resourceAmounts =
        resources.amounts;

    activeSave.hasResourceData = true;

    return SaveManager::Save(activeSave);
}


// ========================================
// UPDATE
// ========================================

void GameScreen::Update()
{
    if (loadedSlot < 1)
    {
        return;
    }

    float deltaTime = GetFrameTime();

    // ====================================
    // RESOURCE PRODUCTION
    // ====================================

    resources.Update(deltaTime);

    // ====================================
    // AUTOSAVE
    // ====================================

    autoSaveTimer += deltaTime;

    if (autoSaveTimer >= 5.0f)
    {
        if (SaveProgress())
        {
            autoSaveTimer = 0.0f;
        }
    }

    // Сохраняем при выходе в меню.
    if (IsKeyPressed(KEY_ESCAPE))
    {
        SaveProgress();
    }

    sidebar.Update();

    // ====================================
    // CAMERA ZOOM
    // ====================================

    float wheel = GetMouseWheelMove();

    if (wheel != 0.0f)
    {
        camera.zoom += wheel * 0.1f;

        if (camera.zoom < minZoom)
        {
            camera.zoom = minZoom;
        }

        if (camera.zoom > maxZoom)
        {
            camera.zoom = maxZoom;
        }
    }

    // ====================================
    // CAMERA MOVEMENT
    // ====================================

    float moveSpeed =
        cameraSpeed *
        deltaTime /
        camera.zoom;

    if (IsKeyDown(KEY_W))
    {
        camera.target.y -= moveSpeed;
    }

    if (IsKeyDown(KEY_S))
    {
        camera.target.y += moveSpeed;
    }

    if (IsKeyDown(KEY_A))
    {
        camera.target.x -= moveSpeed;
    }

    if (IsKeyDown(KEY_D))
    {
        camera.target.x += moveSpeed;
    }

    // Сброс камеры.
    if (IsKeyPressed(KEY_F))
    {
        ResetCamera();
    }
}


// ========================================
// DRAW
// ========================================

void GameScreen::Draw() const
{
    // ====================================
    // WORLD
    // ====================================

    BeginMode2D(camera);

    worldGrid.Draw();

    EndMode2D();

    // ====================================
    // RESOURCE HUD
    // ====================================

    // Панель не зависит от камеры.
    resourceHUD.Draw(resources);

    sidebar.Draw();
    // ====================================
    // CONTROLS
    // ====================================

    int bottom =
        GetScreenHeight() - 68;

    DrawText(
        "WASD - move camera",
        20,
        bottom,
        18,
        LIGHTGRAY
    );

    DrawText(
        "Mouse Wheel - zoom",
        20,
        bottom + 22,
        18,
        LIGHTGRAY
    );

    DrawText(
        "F - reset camera",
        20,
        bottom + 44,
        18,
        LIGHTGRAY
    );
}
