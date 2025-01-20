#pragma once
#ifndef PATHCOM_H
#define PATHCOM_H

#include "Component.h"
#include "../Pathfinding/AStarPathfinding.h"
#include <vector>
#include <string>

namespace ecs {

    class PathfindingComponent : public Component {
    public:
        std::vector<int> m_StartPos{ 0, 0 };        // Start position as {x, y}
        std::vector<int> m_TargetPos{ 0, 0 };       // Target position as {x, y}
        std::string m_GridKey;                      // Identifier for the grid
        std::vector <Node> m_Path;                  // Calculated path

        REFLECTABLE(PathfindingComponent, m_StartPos, m_TargetPos, m_GridKey, m_Path)
    };
}

#endif // PATHCOM_H