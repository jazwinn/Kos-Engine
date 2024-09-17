#include <iostream>

#include "ECS.h"

//ECS Varaible

ECS* ECS::InstancePtr = new ECS{};

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
	ecs->ECS_CombinedComponentPool[TypeTransformComponent] = new ComponentPool<TransformComponent>{};
	ecs->ECS_CombinedComponentPool[TypeMovemmentComponent] = new ComponentPool<MovementComponent>{};
	ecs->ECS_CombinedComponentPool[TypeSpriteComponent] = new ComponentPool<SpriteComponent>{};

	//Allocate memory to each system
	ecs->ECS_SystemMap[TypeMovementSystem] = new MovementSystem;
	ecs->ECS_SystemMap[TypeRenderSystem] = new RenderSystem;
}


void ECS::Update() {

	ECS* ecs = ECS::GetInstance();

	//loops through all the system
	for (auto& System : ecs->ECS_SystemMap) {
		System.second->Update();
		
	}

}

void ECS::Unload() {

	ECS* ecs = ECS::GetInstance();

	//delete componentpool
	for (size_t n{}; n < TotalTypeComponent; n++) {
		delete ecs->ECS_CombinedComponentPool[(ComponentType)n];
	}

	//delete systems
	for (auto& System : ecs->ECS_SystemMap) {
		delete System.second;

	}

	delete ecs;
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

	Trans->scale = { 0.5f, 0.5f };

	AddComponent(TypeSpriteComponent, ID);

	AddComponent(TypeMovemmentComponent, ID);

	/*--------------------------------------------------------------*/

	ecs->EntityCount++;

	return ID;
}

EntityID ECS::DuplicateEntity(EntityID DuplicatesID) {

	ECS* ecs = ECS::GetInstance();
	EntityID NewEntity = ecs->CreateEntity();

	compSignature DuplicateSignature = ecs->ECS_EntityMap.find(DuplicatesID)->second;

	for (size_t n{}; n < TotalTypeComponent; n++) {

		if (DuplicateSignature.test((ComponentType)n)){

			ecs->ECS_CombinedComponentPool[(ComponentType)n]->DuplicateComponent(DuplicatesID, NewEntity);

		}

	}
	
	ecs->ECS_EntityMap.find(NewEntity)->second = DuplicateSignature;


	return NewEntity;

}

bool ECS::DeleteEntity(EntityID ID) {

	// refector
	DeregisterSystem(ID);

	return true;
}

