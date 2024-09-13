#include "../ECS.h"

#include "MovementSystem.h"

void MovementSystem::RegisterSystem(EntityID ID) {
	ECS* ecs = ECS::GetInstance();

	//TODO check if component is already inside the vector

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

	vecMovementComponentPtr.push_back((MovementComponent*)ecs->ECS_CombinedComponentPool[TypeMovemmentComponent]->GetEntityComponent(ID));

}

void MovementSystem::Init() {

	// requires both movement component and transform component
	std::bitset<TotalTypeComponent> BitCompare;
	SystemSignature.set(TypeMovemmentComponent);
	SystemSignature.set(TypeTransformComponent);

}

void MovementSystem::Update(){

	ECS* ecs = ECS::GetInstance();
	//Loops Through all the entity

	for (EntityID ID{}; ID < ecs->EntityCount; ID++) {

		//IS THERE A MORE EFFICIENT WAY?? ASK KITSON
		if (ecs->ECS_CombinedComponentPool[TypeMovemmentComponent]->HasComponent(ID) && ecs->ECS_CombinedComponentPool[TypeTransformComponent]->HasComponent(ID)) {

			std::cout << "Entity: " << ID << "Movement System is getting Updated";

		}


	}

}