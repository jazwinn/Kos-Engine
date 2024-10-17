#ifndef PHYSICSLAYERR_H
#define PHYSICSLAYERR_H

#include "../ECS/Layers.h"
#include <vector>

const int size = layer::LAYERS::MAXLAYER;
namespace physicslayer {
	class PhysicsLayer {
	public:
	PhysicsLayer() {
		// Initialize the collisionMatrix with 'false'
		collisionMatrix = std::vector<std::vector<bool>>(size, std::vector<bool>(size, false));
	}
	void setCollision(int layer1, int layer2, bool value);
	void printCollisionMatrix() const;

	private:
		std::vector<std::vector<bool>> collisionMatrix; // 2D collision matrix
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