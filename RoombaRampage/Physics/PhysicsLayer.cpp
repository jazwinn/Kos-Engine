#include "../Config/pch.h"
#include "PhysicsLayer.h"
#include "../De&Serialization/json_handler.h"

namespace physicslayer {
    std::unique_ptr<PhysicsLayer> PhysicsLayer::instance = nullptr;
   
 
    PhysicsLayer::PhysicsLayer() {
      // collisionMatrix = std::vector<std::vector<bool>>(size, std::vector<bool>(size, false));
        for (int i = 0; i < size; ++i) {
            layerCollisions[i].reset(); // Initialize all bits to 0 (no collisions)
        }
    }

    void PhysicsLayer::printCollisionMatrix() const {
        for (int i = 0; i < size; ++i) {
            std::ostringstream rowStream;
            for (int j = 0; j < size; ++j) {
                rowStream << layerCollisions[i][j] << " ";
            }
            LOGGING_INFO("Collision Matrix Row {}: {}", i, rowStream.str().c_str());
        }
    }

    void PhysicsLayer::setCollision(int row, int col, bool value) {
        //if (row >= 0 && row < size && col >= 0 && col < size) {
        //    collisionMatrix[row][col] = value;
        //    collisionMatrix[col][row] = value;
        //}
        layerCollisions[row].set(col, value);
        layerCollisions[col].set(row, value); // Ensure symmetric relationship
        Serialization::Serialize::SavePhysicsLayerMatrix(); // Save changes to file
    }
    bool PhysicsLayer::getCollide(int layer1, int layer2) {
        //if (layer1 < 0 || layer1 >= size || layer2 < 0 || layer2 >= size) {
        //    std::cerr << "Error: getCollide out of range access. Layers: " << layer1 << ", " << layer2 << std::endl;
        //    return false; // Return a default value or handle the error as needed.
        //}
        //return collisionMatrix[layer1][layer2];
        return layerCollisions[layer1].test(layer2); // Check if layers should collide
    }

    //void PhysicsLayer::m_deserializeData() {
    //    std::ifstream file("../Physics/PhysicsLayerMatrix.txt");
    //    if (!file.is_open()) {
    //        LOGGING_ERROR("Could not open PhysicsLayerMatrix.txt for reading.");
    //        return;
    //    }

    //    std::string line;
    //    int row = 0;
    //    while (std::getline(file, line) && row < size) {
    //        std::istringstream iss(line);
    //        int col = 0;
    //        int value;
    //        while (iss >> value && col < size) {
    //            setCollision(row, col, value == 1);
    //            ++col;
    //        }
    //        ++row;
    //    }

    //    file.close();
    //    LOGGING_INFO("Collision matrix loaded from PhysicsLayerMatrix.txt");
    //}
    //void PhysicsLayer::m_serializeData() {
    //    std::ofstream file("../Physics/PhysicsLayerMatrix.txt");
    //    if (!file.is_open()) {
    //        LOGGING_ERROR("Could not open PhysicsLayerMatrix.txt for writing.");
    //        return;
    //    }

    //    for (int i = 0; i < size; ++i) {
    //        for (int j = 0; j < size; ++j) {
    //            file << layerCollisions[i][j] << " ";
    //        }
    //        file << "\n";
    //    }

    //    file.close();
    //    LOGGING_INFO("Collision matrix saved to PhysicsLayerMatrix.txt");
    //}
    std::vector<std::vector<bool>> PhysicsLayer::getMatrix() const {
        return collisionMatrix;
    }
     std::bitset<size>* PhysicsLayer::getMat() {
        return layerCollisions;
    }
     const std::bitset<size>* PhysicsLayer::getMat() const {
         return layerCollisions;
     }
}