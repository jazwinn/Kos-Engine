#include "../Pathfinding/AStarPathfinding.h"
#include <cmath>
#include <algorithm>

std::vector<Node> AStarPathfinding::FindPath(ecs::TilemapComponent* tilemap, int startX, int startY, int targetX, int targetY) {
    auto cmp = [](Node* a, Node* b) { return a->fCost() > b->fCost(); };
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> openList(cmp);
    std::set<std::pair<int, int>> openSet; // Tracks nodes in the open list
    std::set<std::pair<int, int>> closedSet;
    std::unordered_map<std::pair<int, int>, Node*, pair_hash> allNodes;

    Node* startNode = new Node(startX, startY);
    startNode->gCost = 0;
    startNode->hCost = CalculateHeuristic(startX, startY, targetX, targetY);
    openList.push(startNode);
    openSet.insert({ startX, startY });
    allNodes[{startX, startY}] = startNode;

    while (!openList.empty()) {
        Node* current = openList.top();
        openList.pop();
        openSet.erase({ current->x, current->y });

        if (current->x == targetX && current->y == targetY) {
            return ReconstructPath(current);
        }

        closedSet.insert({ current->x, current->y });

        for (const auto& [dx, dy] : directions) {
            int nx = current->x + dx;
            int ny = current->y + dy;

            if (!IsWalkable(tilemap, nx, ny) || closedSet.count({ nx, ny })) {
                continue;
            }

            float tentativeGCost = current->gCost + tilemap->m_tilePictureIndex[nx][ny];
            Node* neighbor;

            auto it = allNodes.find({ nx, ny });
            if (it == allNodes.end()) {
                neighbor = new Node(nx, ny);
                neighbor->hCost = CalculateHeuristic(nx, ny, targetX, targetY);
                allNodes[{nx, ny}] = neighbor;
            }
            else {
                neighbor = it->second;
            }

            if (tentativeGCost < neighbor->gCost) {
                neighbor->gCost = tentativeGCost;
                neighbor->parent = current;

                if (openSet.find({ nx, ny }) == openSet.end()) {
                    openList.push(neighbor);
                    openSet.insert({ nx, ny });
                }
            }
        }
    }

    return {}; //It returns empty if it does not find any path
}


    bool AStarPathfinding::IsWalkable(ecs::TilemapComponent* tilemap, int x, int y) {
        return x >= 0 && x < tilemap->m_rowLength && y >= 0 && y < tilemap->m_columnLength && tilemap->m_tilePictureIndex[x][y] != -1;
    }

    float AStarPathfinding::CalculateHeuristic(int x1, int y1, int x2, int y2) {
        return std::abs(x1 - x2) + std::abs(y1 - y2);
    }

    std::vector<Node> AStarPathfinding::ReconstructPath(Node* endNode) {
        std::vector<Node> path;
        Node* current = endNode;

        while (current != nullptr) {
            path.push_back(*current);
            current = current->parent;
        }

        std::reverse(path.begin(), path.end());
        return path;
    }