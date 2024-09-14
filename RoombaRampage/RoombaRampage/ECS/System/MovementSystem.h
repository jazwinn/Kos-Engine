#ifndef MOVESYS_H
#define MOVESYS_H

#include "System.h"
#include "../ECS/Component/TransformComponent.h"
#include "../ECS/Component/MovementComponent.h"

class MovementSystem : public ISystem {

public:
	void RegisterSystem(EntityID) override;

	void DeregisterSystem(EntityID) override;

	void Init() override;

	void Update() override;

private:
	//Storage to point to components
	std::vector<TransformComponent*> vecTransformComponentPtr;
	std::vector<MovementComponent*> vecMovementComponentPtr;

};

#endif MOVESYS_H