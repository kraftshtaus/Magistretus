#include "WorldGrid.h"

#include <sstream>

using namespace std;


WorldGrid::WorldGrid(
    TerrainTextures* terrainTextures
)
    : terrainTextures(terrainTextures)
{
    tiles.assign(
        GRID_SIZE,
        vector<TileType>(
            GRID_SIZE,
            TileType::Grass
        )
    );
}


void WorldGrid::Generate()
{
    tiles.assign(
        GRID_SIZE,
        vector<TileType>(
            GRID_SIZE,
            TileType::Grass
        )
    );


    int crystalCount =
        GetRandomValue(0, 1);

    int manaCount =
        GetRandomValue(1, 2);

    int quarryCount =
        GetRandomValue(1, 2);

    int waterCount =
        GetRandomValue(1, 2);

    int forestCount =
        GetRandomValue(1, 2);


    PlaceSpecialTiles(
        TileType::CrystalField,
        crystalCount
    );

    PlaceSpecialTiles(
        TileType::ManaField,
        manaCount
    );

    PlaceSpecialTiles(
        TileType::QuarrySpot,
        quarryCount
    );

    PlaceSpecialTiles(
        TileType::Water,
        waterCount
    );

    PlaceSpecialTiles(
        TileType::Forest,
        forestCount
    );
}


bool WorldGrid::IsSpecialTile(
    TileType type
) const
{
    return type != TileType::Grass;
}


bool WorldGrid::CanPlaceSpecialTile(
    int x,
    int y
) const
{
    if (
        tiles[y][x] !=
        TileType::Grass
    )
    {
        return false;
    }


    for (
        int offsetY = -1;
        offsetY <= 1;
        offsetY++
    )
    {
        for (
            int offsetX = -1;
            offsetX <= 1;
            offsetX++
        )
        {
            int checkX =
                x + offsetX;

            int checkY =
                y + offsetY;


            if (
                checkX < 0 ||
                checkX >= GRID_SIZE ||
                checkY < 0 ||
                checkY >= GRID_SIZE
            )
            {
                continue;
            }


            if (
                IsSpecialTile(
                    tiles[checkY][checkX]
                )
            )
            {
                return false;
            }
        }
    }


    return true;
}


void WorldGrid::PlaceSpecialTiles(
    TileType type,
    int amount
)
{
    int placed = 0;
    int attempts = 0;

    const int MAX_ATTEMPTS = 1000;


    while (
        placed < amount &&
        attempts < MAX_ATTEMPTS
    )
    {
        attempts++;


        int x =
            GetRandomValue(
                0,
                GRID_SIZE - 1
            );

        int y =
            GetRandomValue(
                0,
                GRID_SIZE - 1
            );


        if (
            !CanPlaceSpecialTile(
                x,
                y
            )
        )
        {
            continue;
        }


        tiles[y][x] =
            type;

        placed++;
    }
}


Texture2D* WorldGrid::GetTileTexture(
    TileType type
) const
{
    switch (type)
    {
        case TileType::Grass:
        {
            if (terrainTextures->grassLoaded)
            {
                return &terrainTextures->grass;
            }

            break;
        }

        case TileType::ManaField:
        {
            if (terrainTextures->manaLoaded)
            {
                return &terrainTextures->mana;
            }

            break;
        }

        case TileType::Water:
        {
            if (terrainTextures->waterLoaded)
            {
                return &terrainTextures->water;
            }

            break;
        }

        case TileType::Forest:
        {
            if (terrainTextures->forestLoaded)
            {
                return &terrainTextures->forest;
            }

            break;
        }

        case TileType::QuarrySpot:
        {
            if (terrainTextures->quarryLoaded)
            {
                return &terrainTextures->quarry;
            }

            break;
        }

        case TileType::CrystalField:
        {
            if (terrainTextures->crystalLoaded)
            {
                return &terrainTextures->crystal;
            }

            break;
        }
    }


    return nullptr;
}


void WorldGrid::Draw() const
{
    const int startX = 80;
    const int startY = 80;


    for (
        int y = 0;
        y < GRID_SIZE;
        y++
    )
    {
        for (
            int x = 0;
            x < GRID_SIZE;
            x++
        )
        {
            int drawX =
                startX +
                x * TILE_SIZE;

            int drawY =
                startY +
                y * TILE_SIZE;


            Texture2D* texture =
                GetTileTexture(
                    tiles[y][x]
                );


            if (texture != nullptr)
            {
                Rectangle source =
                {
                    0.0f,
                    0.0f,
                    (float)texture->width,
                    (float)texture->height
                };


                Rectangle destination =
                {
                    (float)drawX,
                    (float)drawY,
                    (float)TILE_SIZE,
                    (float)TILE_SIZE
                };


                DrawTexturePro(
                    *texture,
                    source,
                    destination,
                    Vector2{ 0.0f, 0.0f },
                    0.0f,
                    WHITE
                );
            }
            else
            {
                // Заглушка, если PNG не найден.
                DrawRectangle(
                    drawX,
                    drawY,
                    TILE_SIZE,
                    TILE_SIZE,
                    MAGENTA
                );
            }
        }
    }
}


string WorldGrid::Serialize() const
{
    stringstream stream;


    for (
        int y = 0;
        y < GRID_SIZE;
        y++
    )
    {
        for (
            int x = 0;
            x < GRID_SIZE;
            x++
        )
        {
            int value =
                static_cast<int>(
                    tiles[y][x]
                );


            stream << value;


            if (
                !(
                    y == GRID_SIZE - 1 &&
                    x == GRID_SIZE - 1
                )
            )
            {
                stream << ",";
            }
        }
    }


    return stream.str();
}


bool WorldGrid::Deserialize(
    const string& data
)
{
    if (data.empty())
    {
        return false;
    }


    stringstream stream(data);
    string valueText;

    int index = 0;


    while (
        getline(
            stream,
            valueText,
            ','
        )
    )
    {
        if (
            index >=
            GRID_SIZE * GRID_SIZE
        )
        {
            break;
        }


        int value =
            stoi(valueText);


        int y =
            index / GRID_SIZE;

        int x =
            index % GRID_SIZE;


        tiles[y][x] =
            static_cast<TileType>(
                value
            );


        index++;
    }


    return
        index ==
        GRID_SIZE * GRID_SIZE;
}