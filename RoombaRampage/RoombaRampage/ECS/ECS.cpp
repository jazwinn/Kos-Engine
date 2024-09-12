#include <iostream>

#include "ECS.h"
#include "../ECS/System/SystemList.h"

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



EntityID ECS::CreateEntity() {

	ECS* ecs = ECS::GetInstance();

	// create for all components
	TransformComponent* TransComponent = static_cast<TransformComponent*>(ecs->ECS_CombinedComponentPool[TypeTransformComponent]->CreateComponent());
	MovementComponent* MoveComponent = static_cast<MovementComponent*>(ecs->ECS_CombinedComponentPool[TypeMovemmentComponent]->CreateComponent());
	SpriteComponent* SpComponent = static_cast<SpriteComponent*>(ecs->ECS_CombinedComponentPool[TypeSpriteComponent]->CreateComponent());

	ecs->EntityCount++;

	return TransComponent->Entity;
}

bool ECS::DeleteEntity(EntityID ID) {
	ECS* ecs = ECS::GetInstance();

	ecs->ECS_CombinedComponentPool[TypeTransformComponent]->DeleteEntityComponent(ID);
	ecs->ECS_CombinedComponentPool[TypeMovemmentComponent]->DeleteEntityComponent(ID);

	ecs->EntityCount--;

	return true;
}

void ECS::Unload() {

	ECS* ecs = ECS::GetInstance();

	delete ecs->ECS_CombinedComponentPool[TypeTransformComponent];
	delete ecs->ECS_CombinedComponentPool[TypeMovemmentComponent];

	delete ecs;
}