#include "../ECS.h"

#include "ControlSystem.h"
#include "../Inputs/Input.h"

namespace Ecs {

	void ControlSystem::RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(vecMovementComponentPtr.begin(), vecMovementComponentPtr.end(), [ID](const auto& obj) { return obj->Entity == ID; })
			== vecMovementComponentPtr.end()) {
			vecPlayerComponentPtr.push_back((PlayerComponent*)ecs->ECS_CombinedComponentPool[TypePlayerComponent]->GetEntityComponent(ID));
			vecMovementComponentPtr.push_back((MovementComponent*)ecs->ECS_CombinedComponentPool[TypeMovemmentComponent]->GetEntityComponent(ID));
		}


	}

	void ControlSystem::DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : vecPlayerComponentPtr) {
			if (ComponentPtr->Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = vecPlayerComponentPtr.size() - 1;

		std::swap(vecPlayerComponentPtr[IndexID], vecPlayerComponentPtr[IndexLast]);
		std::swap(vecMovementComponentPtr[IndexID], vecMovementComponentPtr[IndexLast]);

		//popback the vector;
		vecPlayerComponentPtr.pop_back();
		vecMovementComponentPtr.pop_back();
	}

	void ControlSystem::Init() {

		// requires both movement component and transform component
		SystemSignature.set(TypePlayerComponent);
		SystemSignature.set(TypeMovemmentComponent);
		//SystemSignature.set();

	}

	void ControlSystem::Update() {

		ECS* ecs = ECS::GetInstance();

		if (vecMovementComponentPtr.size() != vecPlayerComponentPtr.size()) {
			std::cout << "Error: Vecotrs container size does not Match" << std::endl;
			return;
		}

		for (int n{}; n < vecMovementComponentPtr.size(); n++) {
			//std::cout << "Entity: " << n << "Movement System is getting Updated";

			PlayerComponent* BoxComp = vecPlayerComponentPtr[n];
			MovementComponent* MovComp = vecMovementComponentPtr[n];


			// set direction to always be 0,0
			//MovComp->Direction.x = 0;
			//MovComp->Direction.y = 0;

			Vector2::Vec2 NewDirection{};
			if (Input::InputSystem::KeyStateW) {
				NewDirection.y += 1;
			}

			if (Input::InputSystem::KeyStateS) {
				NewDirection.y += -1;
			}

			if (Input::InputSystem::KeyStateD) {
				NewDirection.x += 1;
			}

			if (Input::InputSystem::KeyStateA) {
				NewDirection.x += -1;
			}

			if ((NewDirection.x != 0) && (NewDirection.y != 0)) {
				Vector2::Vec2::funcVec2Normalize(NewDirection, NewDirection);
				
			}

			MovComp->Direction = NewDirection;

		}

	}


}
