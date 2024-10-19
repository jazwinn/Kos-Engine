#ifndef PHYSICSLAYERR_H
#define PHYSICSLAYERR_H

#include "../ECS/Layers.h"
#include <vector>
#include <memory>
#include <iostream>
#include <utility>

namespace physicslayer {
    const int size = layer::LAYERS::MAXLAYER;
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
	void printCollisionMatrix() const;
    bool getCollide(int layer1, int layer2);
    std::vector<std::vector<bool>> getMatrix() const;

	private:
		 std::vector<std::vector<bool>> collisionMatrix; // 2D collision matrix
         static std::unique_ptr<PhysicsLayer> instance;
	};
   
}

#endif