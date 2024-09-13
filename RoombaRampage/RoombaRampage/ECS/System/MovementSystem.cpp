#include "../ECS.h"

#include "MovementSystem.h"

void MovementSystem::RegisterSystem() {
	ECS* ecs = ECS::GetInstance();

	// requires both movement component and transform component
	std::bitset<TotalTypeComponent> BitCompare;
	BitCompare.set(TypeMovemmentComponent);
	BitCompare.set(TypeTransformComponent);

	for (auto& EntityBit : ecs->ECS_EntityMap) {
		if (EntityBit.second.test(TypeMovemmentComponent) && EntityBit.second.test(TypeTransformComponent)) {
			EntityID StoringID = EntityBit.first;

			//TODO check if component is already inside the vector

			//Step 1 : check
			bool store = true;
			for (auto& TransformComponentPtr : vecTransformComponentPtr) {
				if (TransformComponentPtr->Entity == StoringID) {
					store = false;
					break;
				}
				else {
					store = true;
				}
			}
			//Step 2 : Store address into vector
			if (store == true) {
				vecTransformComponentPtr.push_back((TransformComponent*)ecs->ECS_CombinedComponentPool[TypeTransformComponent]->GetEntityComponent(StoringID));
			}
			
			vecMovementComponentPtr.push_back((MovementComponent*)ecs->ECS_CombinedComponentPool[TypeMovemmentComponent]->GetEntityComponent(StoringID));

		}

	}

}

void MovementSystem::Init() {

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