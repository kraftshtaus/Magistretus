
#pragma once

#include <raylib.h>

#include "../game/resources/ResourceSystem.h"

using namespace std;


class ResourceHUD
{
public:
    ResourceHUD();
    ~ResourceHUD();

    ResourceHUD(const ResourceHUD&) = delete;
    ResourceHUD& operator=(const ResourceHUD&) = delete;

    void Draw(const ResourceSystem& resources) const;

private:
    Texture2D icons[RESOURCE_COUNT] = {};
    bool iconLoaded[RESOURCE_COUNT] = {};

    void LoadIcons();
};
