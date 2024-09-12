#include "../ECS.h"

#include "MovementSystem.h"

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