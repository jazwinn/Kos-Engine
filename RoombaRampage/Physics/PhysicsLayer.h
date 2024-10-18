#ifndef PHYSICSLAYERR_H
#define PHYSICSLAYERR_H

#include "../ECS/Layers.h"
#include <vector>
#include <memory>
#include <iostream>
#include <set>
#include <utility>

const int size = layer::LAYERS::MAXLAYER;
namespace physicslayer {
	class PhysicsLayer {
	public:
    // Method to access the single instance of PhysicsLayer
    static PhysicsLayer* getInstance() {
        if (!instance) {
            instance = std::make_unique<PhysicsLayer>();
        }
        return instance.get();
    }
    PhysicsLayer();
	PhysicsLayer(const PhysicsLayer&) = delete;
	PhysicsLayer& operator=(const PhysicsLayer&) = delete;

	void setCollision(int layer1, int layer2, bool value);
    bool shouldCollide(int layer1, int layer2);
	void printCollisionMatrix() const;
    bool getCollide(int layer1, int layer2);
    std::vector<std::vector<bool>> getMatrix() const;

	private:
		 std::vector<std::vector<bool>> collisionMatrix; // 2D collision matrix
         static std::unique_ptr<PhysicsLayer> instance;
	};
   
}
/*
        // Print only the upper triangle part
        for (int j = i; j >= 0; --j) {
            std::cout << collisionMatrix[i][j] << " ";
        }
        for (int j = i + 1 ; j >= 0 ; --j){
            std::cout << "  "; // Two spaces for each missing element
        }
        std::cout << std::endl;
*/
#endif