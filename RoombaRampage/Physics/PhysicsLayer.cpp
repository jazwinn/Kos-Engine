#include "PhysicsLayer.h"

namespace physicslayer {
    std::unique_ptr<PhysicsLayer> PhysicsLayer::instance = nullptr;
   
 
    PhysicsLayer::PhysicsLayer() {
        collisionMatrix = std::vector<std::vector<bool>>(size, std::vector<bool>(size, false));
        for (int i = 0; i < size; ++i) {
            setCollision(0, i, true);
        }
    }

    void PhysicsLayer::printCollisionMatrix() const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                std::cout << collisionMatrix[i][j] << " ";
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
    bool PhysicsLayer::getCollide(int layer1, int layer2) {\
        if (layer1 < 0 || layer1 >= size || layer2 < 0 || layer2 >= size) {
            std::cerr << "Error: getCollide out of range access. Layers: " << layer1 << ", " << layer2 << std::endl;
            return false; // Return a default value or handle the error as needed.
        }
        return collisionMatrix[layer1][layer2];
    }
    

    std::vector<std::vector<bool>> PhysicsLayer::getMatrix() const {
        return collisionMatrix;
    }
}