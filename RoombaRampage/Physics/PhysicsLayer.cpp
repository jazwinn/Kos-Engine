#include "PhysicsLayer.h"

namespace physicslayer {
    std::unique_ptr<PhysicsLayer> PhysicsLayer::instance = nullptr;
    std::set<std::pair<int, int>> checkedPairs;
    
    PhysicsLayer::PhysicsLayer() {
        collisionMatrix = {};
    }

    void PhysicsLayer::printCollisionMatrix() const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size - i; ++j) {
                std::cout << collisionMatrix[i][j] << " ";
            }
            for (int j = size - i + 1; j < size; ++j) {
                std::cout << "  "; // Two spaces for each missing element
            }
            std::cout << std::endl;
        }
    }

    void PhysicsLayer::setCollision(int row, int col, bool value) {
        if (row >= 0 && row < size && col >= 0 && col < size) {
            collisionMatrix[row][col] = value;
            collisionMatrix[col][row] = value;
        }
    }
    bool PhysicsLayer::getCollide(int layer1, int layer2) {
        return collisionMatrix[layer1][layer2];
    }
    
    bool PhysicsLayer::shouldCollide(int layer1, int layer2) {
        if (layer1 > layer2) {
            std::swap(layer1, layer2);
        }
        std::pair<int, int> layerPair = { layer1, layer2 };
        // Check if this pair has already been checked
        if (checkedPairs.find(layerPair) != checkedPairs.end()) {
            return false; // Pair has already been checked
        }

        // Add the pair to the set of checked pairs
        checkedPairs.insert(layerPair);

        return collisionMatrix[layer1][layer2];
    }

    std::vector<std::vector<bool>> PhysicsLayer::getMatrix() const {
        return collisionMatrix;
    }
}