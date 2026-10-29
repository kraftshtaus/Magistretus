#pragma once

// Тип клетки игрового поля.
// Позже сюда можно будет добавлять новые виды территории.
enum class TileType
{
    Grass,
    QuarrySpot,
    ManaField,
    CrystalField,
    Water,
    Forest
};