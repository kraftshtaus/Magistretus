
#pragma once

#include <array>
#include <algorithm>

using namespace std;

constexpr int RESOURCE_COUNT = 6;

enum class ResourceType
{
    Mana = 0,
    Wood,
    Food,
    Water,
    Stone,
    Money
};

class ResourceSystem
{
public:
    // Количество ресурсов.
    array<double, RESOURCE_COUNT> amounts = {};

    // Производство в секунду.
    array<double, RESOURCE_COUNT> rates = {};

    void Reset()
    {
        amounts.fill(0.0);
        rates.fill(0.0);
    }

    void Update(float deltaTime)
    {
        if (deltaTime <= 0.0f)
            return;

        for (int i = 0; i < RESOURCE_COUNT; i++)
        {
            amounts[i] = max(
                0.0,
                amounts[i] + rates[i] * deltaTime
            );
        }
    }

    void SetRate(ResourceType type, double value)
    {
        rates[static_cast<int>(type)] =
            max(0.0, value);
    }

    void AddRate(ResourceType type, double value)
    {
        int index = static_cast<int>(type);

        rates[index] = max(
            0.0,
            rates[index] + value
        );
    }

    void SetAmounts(
        const array<double, RESOURCE_COUNT>& saved
    )
    {
        amounts = saved;
    }

    double Get(ResourceType type) const
    {
        return amounts[static_cast<int>(type)];
    }

    double GetRate(ResourceType type) const
    {
        return rates[static_cast<int>(type)];
    }
};
