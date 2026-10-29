#pragma once

#include <raylib.h>
#include <vector>
#include <string>

#include "Tile.h"
#include "TerrainTextures.h"

using namespace std;

// Поле 16x16.
// Хранит тип каждой клетки,
// генерирует special tiles и рисует карту.
class WorldGrid
{
public:
    WorldGrid(
        TerrainTextures* terrainTextures
    );

    void Generate();
    void Draw() const;

    string Serialize() const;

    bool Deserialize(
        const string& data
    );

private:
    static const int GRID_SIZE = 16;

    // Наш базовый размер тайла.
    static const int TILE_SIZE = 64;

    vector<vector<TileType>> tiles;

    TerrainTextures* terrainTextures;


    void PlaceSpecialTiles(
        TileType type,
        int amount
    );


    bool CanPlaceSpecialTile(
        int x,
        int y
    ) const;


    bool IsSpecialTile(
        TileType type
    ) const;


    Texture2D* GetTileTexture(
        TileType type
    ) const;
};