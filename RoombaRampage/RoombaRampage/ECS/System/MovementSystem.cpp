#include "../ECS.h"

#include "MovementSystem.h"

void MovementSystem::RegisterSystem(EntityID ID) {
	ECS* ecs = ECS::GetInstance();

	//Step 1 : check
	bool store = true;
	for (auto& TransformComponentPtr : vecTransformComponentPtr) {
		if (TransformComponentPtr->Entity == ID) {
			store = false;
			break;
		}
		else {
			store = true;
		}
	}
	//Step 2 : Store address into vector
	if (store == true) {
		vecTransformComponentPtr.push_back((TransformComponent*)ecs->ECS_CombinedComponentPool[TypeTransformComponent]->GetEntityComponent(ID));
	}

	//check again
	for (auto& MovementComponentPtr : vecMovementComponentPtr) {
		if (MovementComponentPtr->Entity == ID) {
			store = false;
			break;
		}
		else {
			store = true;
		}
	}
	if (store) {
		vecMovementComponentPtr.push_back((MovementComponent*)ecs->ECS_CombinedComponentPool[TypeMovemmentComponent]->GetEntityComponent(ID));
	}
	

}

void MovementSystem::DeregisterSystem(EntityID ID) {
	//search element location for the entity
	size_t IndexID{};
	for (auto& MovComponentPtr : vecMovementComponentPtr) {
		if (MovComponentPtr->Entity == ID) {
			return;
		}
		IndexID++;
	}

	//index to the last element
	size_t IndexLast = vecMovementComponentPtr.size() - 1;
	std::swap(vecMovementComponentPtr[IndexID], vecMovementComponentPtr[IndexLast]);
	std::swap(vecTransformComponentPtr[IndexID], vecTransformComponentPtr[IndexLast]);

	//popback the vector;
	vecMovementComponentPtr.pop_back();
	vecTransformComponentPtr.pop_back();
}

void MovementSystem::Init() {

	// requires both movement component and transform component
	SystemSignature.set(TypeMovemmentComponent);
	SystemSignature.set(TypeTransformComponent);

}

void MovementSystem::Update(){

	ECS* ecs = ECS::GetInstance();

	if (vecMovementComponentPtr.size() != vecTransformComponentPtr.size()) {
		std::cout << "Error: Vecotrs container size does not Match" << std::endl;
		return;
	}

	//loops through all vecoters pointing to component
	for (int n{}; n < vecMovementComponentPtr.size(); n++) {
		//std::cout << "Entity: " << n << "Movement System is getting Updated";

		MovementComponent* MovComp = vecMovementComponentPtr[n];
		TransformComponent* TransComp = vecTransformComponentPtr[n];

		Vector2::Vec2 Velocity = MovComp->Direction * MovComp->Speed;

		Vector2::Vec2 Displacement = Velocity * ecs->DeltaTime;

		TransComp->position += Displacement;
	}

}