#include <iostream>

#include "ECS.h"
#include <algorithm>
#include "../Debugging/Performance.h"
//ECS Varaible

namespace Ecs{

	std::unique_ptr<ECS> ECS::InstancePtr = nullptr;

	void ECS::Init() {
		ECS* ecs = ECS::GetInstance();
		//loops through all the system
		for (auto& System : ecs->ECS_SystemMap) {
			System.second->Init();
		}

	}

	void ECS::Load() {

		ECS* ecs = ECS::GetInstance();

		//Allocate memory to each component pool
		ecs->ECS_CombinedComponentPool[TypeTransformComponent] = std::make_shared<ComponentPool<TransformComponent>>();
		ecs->ECS_CombinedComponentPool[TypeMovemmentComponent] = std::make_shared < ComponentPool<MovementComponent>>();
		ecs->ECS_CombinedComponentPool[TypeSpriteComponent] = std::make_shared < ComponentPool<SpriteComponent>>();
		ecs->ECS_CombinedComponentPool[TypeColliderComponent] = std::make_shared < ComponentPool<ColliderComponent>>();
		ecs->ECS_CombinedComponentPool[TypeRigidBodyComponent] = std::make_shared < ComponentPool<RigidBodyComponent>>();
		ecs->ECS_CombinedComponentPool[TypePlayerComponent] = std::make_shared<ComponentPool<PlayerComponent>>();

		//Allocate memory to each system
		ecs->ECS_SystemMap[TypeControlSystem] = std::make_shared<ControlSystem>();
		ecs->ECS_SystemMap[TypeMovementSystem] = std::make_shared<MovementSystem>();
		ecs->ECS_SystemMap[TypeCollisionSystem] = std::make_shared<CollisionSystem>();
		ecs->ECS_SystemMap[TypeRenderSystem] = std::make_shared<RenderSystem>();
		ecs->ECS_SystemMap[TypeCollisionResponseSystem] = std::make_shared<CollisionResponseSystem>();

		//Initialize all system Peformance
		PerformanceTracker::Performance Perform{};
		for (int n{}; n < TotalTypeSystem; n++) {
			Perform.AddSystem((TypeSystem)n);
		}
	}


	void ECS::Update(float DT) {

		ECS* ecs = ECS::GetInstance();
		//update deltatime
		ecs->DeltaTime = DT;

		//loops through all the system
		for (auto& System : ecs->ECS_SystemMap) {
			auto start = std::chrono::steady_clock::now();
			static float interval = 0;

			System.second->Update();


			auto end = std::chrono::steady_clock::now();

			std::chrono::duration<float> duration = end - start;

			interval += duration.count();
			PerformanceTracker::Performance::UpdateTotalSystemTime(duration.count());
			if (interval > 1) {
				
				PerformanceTracker::Performance::UpdateSystemTime(System.first, duration.count());
				interval = 0;
			}

		}
		
	}

	void ECS::Unload() {



		//delete ecs;
	}

	void* ECS::AddComponent(ComponentType Type, EntityID ID) {

		ECS* ecs = ECS::GetInstance();

		void* ComponentPtr = ecs->ECS_CombinedComponentPool[Type]->AssignComponent(ID);

		ecs->ECS_EntityMap.find(ID)->second.set(Type);

		//checks if new component fufils any of the system requirements
		RegisterSystems(ID);

		return ComponentPtr;
	}

	void ECS::RegisterSystems(EntityID ID) {

		ECS* ecs = ECS::GetInstance();
		for (auto& system : ecs->ECS_SystemMap) {
			if ((ecs->ECS_EntityMap.find(ID)->second & system.second->SystemSignature) == system.second->SystemSignature) {

				system.second->RegisterSystem(ID);

			}
		}
	}

	void ECS::DeregisterSystem(EntityID ID) {

		ECS* ecs = ECS::GetInstance();
		for (auto& system : ecs->ECS_SystemMap) {
			if ((ecs->ECS_EntityMap.find(ID)->second & system.second->SystemSignature) == system.second->SystemSignature) {

				system.second->DeregisterSystem(ID);

			}
		}
	}

	EntityID ECS::CreateEntity() {

		ECS* ecs = ECS::GetInstance();

		EntityID ID = ecs->EntityCount;

		ecs->ECS_EntityMap[ID] = 0;

		AddComponent(TypeTransformComponent, ID);


		/*--------------------------------------------------------------
		 for testing
		 --------------------------------------------------------------*/
		TransformComponent* Trans = (TransformComponent*)ecs->ECS_CombinedComponentPool[TypeTransformComponent]->GetEntityComponent(ID);

		Trans->scale;

		AddComponent(TypeSpriteComponent, ID);

		SpriteComponent* sprite = (SpriteComponent*)ecs->ECS_CombinedComponentPool[TypeSpriteComponent]->GetEntityComponent(ID);

		sprite->imageID = 4;

		MovementComponent* MovCom = (MovementComponent*)AddComponent(TypeMovemmentComponent, ID);
		MovCom->Speed = 1;

		AddComponent(TypePlayerComponent, ID);
		AddComponent(TypeRigidBodyComponent, ID);
		AddComponent(TypeColliderComponent, ID);

		/*--------------------------------------------------------------*/

		ecs->EntityCount++;

		return ID;
	}

	EntityID ECS::DuplicateEntity(EntityID DuplicatesID) {

		ECS* ecs = ECS::GetInstance();
		EntityID NewEntity = ecs->CreateEntity();

		compSignature DuplicateSignature = ecs->ECS_EntityMap.find(DuplicatesID)->second;

		for (size_t n{}; n < TotalTypeComponent; n++) {

			if (DuplicateSignature.test((ComponentType)n)) {

				ecs->ECS_CombinedComponentPool[(ComponentType)n]->DuplicateComponent(DuplicatesID, NewEntity);

			}

		}

		ecs->ECS_EntityMap.find(NewEntity)->second = DuplicateSignature;
		RegisterSystems(NewEntity);

		return NewEntity;

	}

	bool ECS::DeleteEntity(EntityID ID) {

		ECS* ecs = ECS::GetInstance();
		// refector
		DeregisterSystem(ID);

		ecs->ECS_EntityMap.erase(ID);

		return true;
	}

}



