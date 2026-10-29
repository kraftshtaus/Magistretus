
#pragma once

#include <raylib.h>

#include "SidebarSection.h"

using namespace std;

class Sidebar
{
public:
    Sidebar() = default;

    void Update();
    void Draw() const;

    void Close();

    SidebarSection GetActiveSection() const;

private:
    SidebarSection activeSection =
        SidebarSection::None;

    Rectangle GetButtonBounds(int index) const;

    void DrawPanel() const;
};
