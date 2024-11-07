#include "../Config/pch.h"
#include "PhysicsLayer.h"

namespace physicslayer {
    std::unique_ptr<PhysicsLayer> PhysicsLayer::instance = nullptr;
   
 
    PhysicsLayer::PhysicsLayer() {
      // collisionMatrix = std::vector<std::vector<bool>>(size, std::vector<bool>(size, false));
        for (int i = 0; i < size; ++i) {
            layerCollisions[i].reset(); // Initialize all bits to 0 (no collisions)
        }
        setCollision(0, 0, true);
    }

    void PhysicsLayer::printCollisionMatrix() const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                std::cout << layerCollisions[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void PhysicsLayer::setCollision(int row, int col, bool value) {
        //if (row >= 0 && row < size && col >= 0 && col < size) {
        //    collisionMatrix[row][col] = value;
        //    collisionMatrix[col][row] = value;
        //}
        layerCollisions[row].set(col, value);
        layerCollisions[col].set(row, value); // Ensure symmetric relationship
    }
    bool PhysicsLayer::getCollide(int layer1, int layer2) {
        //if (layer1 < 0 || layer1 >= size || layer2 < 0 || layer2 >= size) {
        //    std::cerr << "Error: getCollide out of range access. Layers: " << layer1 << ", " << layer2 << std::endl;
        //    return false; // Return a default value or handle the error as needed.
        //}
        //return collisionMatrix[layer1][layer2];
        return layerCollisions[layer1].test(layer2); // Check if layers should collide
    }
    

    std::vector<std::vector<bool>> PhysicsLayer::getMatrix() const {
        return collisionMatrix;
    }
     std::bitset<size>* PhysicsLayer::getMat() {
        return layerCollisions;
    }
}