#include <iostream>

#include "ECS.h"

//ECS Varaible

ECS* ECS::InstancePtr = new ECS{};

void ECS::Init() {

		
}

void ECS::Load() {

	ECS* ecs = ECS::GetInstance();

	//Allocate memory to each component pool
	ecs->ECS_CombinedComponentPool[TypeTransformComponent] = new ComponentPool<TransformComponent>{};
	ecs->ECS_CombinedComponentPool[TypeMovemmentComponent] = new ComponentPool<MovementComponent>{};
	ecs->ECS_CombinedComponentPool[TypeSpriteComponent] = new ComponentPool<SpriteComponent>{};

	//Allocate memory to each system
	ecs->ECS_SystemMap[TypeMovementSystem] = new MovementSystem;
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

	delete ecs->ECS_CombinedComponentPool[TypeTransformComponent];
	delete ecs->ECS_CombinedComponentPool[TypeMovemmentComponent];

	delete ecs;
}

void ECS::AddComponent(ComponentType Type, EntityID ID) {
	
	ECS* ecs = ECS::GetInstance();

	ecs->ECS_CombinedComponentPool[Type]->AssignComponent(ID);

	ecs->ECS_EntityMap.find(ID)->second.set(Type);

	//checks if new component fufils any of the system requirements
	RegisterSystem(ID);

}

void ECS::RegisterSystem(EntityID ID) {

	ECS* ecs = ECS::GetInstance();
	for (auto& system : ecs->ECS_SystemMap) {
		if ((ecs->ECS_EntityMap.find(ID)->second & system.second->SystemSignature) == system.second->SystemSignature) {

			system.second->RegisterSystem(ID);

		}
	}
}

EntityID ECS::CreateEntity() {

	ECS* ecs = ECS::GetInstance();

	EntityID ID = ecs->EntityCount;

	ecs->ECS_EntityMap[ID] = 0;

	AddComponent(TypeTransformComponent, ID);

	ecs->EntityCount++;

	return ID;
}

bool ECS::DeleteEntity(EntityID ID) {
	ECS* ecs = ECS::GetInstance();

	// refector

	ecs->ECS_CombinedComponentPool[TypeTransformComponent]->DeleteEntityComponent(ID);
	ecs->ECS_CombinedComponentPool[TypeMovemmentComponent]->DeleteEntityComponent(ID);

	ecs->EntityCount--;

	return true;
}

