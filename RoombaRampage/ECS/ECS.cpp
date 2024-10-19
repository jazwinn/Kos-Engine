/******************************************************************/
/*!
\file      ECS.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the defination of the Entity Component
		   System. It acts as a factory to add and remove entities.
		   It also assignment entities to component datas.



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include <iostream>

#include "ECS.h"
#include <algorithm>
#include "../Debugging/Performance.h"
#include "../Debugging/Logging.h"
#include "Hierachy.h"
//ECS Varaible

namespace ecs{

	std::unique_ptr<ECS> ECS::m_InstancePtr = nullptr;


	void ECS::m_Init() {
		ECS* ecs = ECS::m_GetInstance();
		//loops through all the system
		for (auto& System : ecs->m_ECS_SystemMap) {
			System.second->m_Init();
		}


	}

	void ECS::m_Load() {

		ECS* ecs = ECS::m_GetInstance();

		//Allocate memory to each component pool
		ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT] = std::make_shared<ComponentPool<TransformComponent>>();
		ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT] = std::make_shared<ComponentPool<NameComponent>>();
		ecs->m_ECS_CombinedComponentPool[TYPESPRITECOMPONENT] = std::make_shared < ComponentPool<SpriteComponent>>();
		ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT] = std::make_shared < ComponentPool<ColliderComponent>>();
		ecs->m_ECS_CombinedComponentPool[TYPERIGIDBODYCOMPONENT] = std::make_shared < ComponentPool<RigidBodyComponent>>();
		ecs->m_ECS_CombinedComponentPool[TYPEPLAYERCOMPONENT] = std::make_shared<ComponentPool<PlayerComponent>>();
		ecs->m_ECS_CombinedComponentPool[TYPEMOVEMENTCOMPONENT] = std::make_shared < ComponentPool<MovementComponent>>();
		ecs->m_ECS_CombinedComponentPool[TYPETEXTCOMPONENT] = std::make_shared<ComponentPool<TextComponent>>();
		ecs->m_ECS_CombinedComponentPool[TYPEANIMATIONCOMPONENT] = std::make_shared<ComponentPool<AnimationComponent>>();
		ecs->m_ECS_CombinedComponentPool[TYPECAMERACOMPONENT] = std::make_shared<ComponentPool<CameraComponent>>();
		ecs->m_ECS_CombinedComponentPool[TYPECAMERACOMPONENT] = std::make_shared<ComponentPool<CameraComponent>>();
		ecs->m_ECS_CombinedComponentPool[TYPESCRIPTCOMPONENT] = std::make_shared<ComponentPool<ScriptComponent>>();

		//Allocate memory to each system
		ecs->m_ECS_SystemMap[TYPECONTROLSYSTEM] = std::make_shared<ControlSystem>();
		ecs->m_ECS_SystemMap[TYPETRANSFORMSYSTEM] = std::make_shared<TransformSystem>();
		ecs->m_ECS_SystemMap[TYPECOLLISIONSYSTEM] = std::make_shared<CollisionSystem>();
		ecs->m_ECS_SystemMap[TYPECOLLISIONRESPONSESYSTEM] = std::make_shared<CollisionResponseSystem>();
		ecs->m_ECS_SystemMap[TYPEMOVEMENTSYSTEM] = std::make_shared<MovementSystem>(); // movement should be the last logic
		ecs->m_ECS_SystemMap[TYPERENDERSYSTEM] = std::make_shared<RenderSystem>();
		ecs->m_ECS_SystemMap[TYPERENDERTEXTSYSTEM] = std::make_shared<RenderTextSystem>();
		ecs->m_ECS_SystemMap[TYPEDEBUGDRAWINGSYSTEM] = std::make_shared<DebugDrawingSystem>();
		ecs->m_ECS_SystemMap[TYPECAMERASYSTEM] = std::make_shared<CameraSystem>();
		ecs->m_ECS_SystemMap[TYPEANIMATIONSYSTEM] = std::make_shared<AnimationSystem>();
		ecs->m_ECS_SystemMap[TYPELOGICSYSTEM] = std::make_shared<LogicSystem>();

		//Initialize all system Peformance
		performancetracker::Performance Perform{};
		for (int n{}; n < TOTALTYPESYSTEM; n++) {
			Perform.m_AddSystem((TypeSystem)n);
		}
	}


	void ECS::m_Update(float DT) {

		ECS* ecs = ECS::m_GetInstance();
		//update deltatime
		ecs->m_DeltaTime = DT;

		performancetracker::Performance::m_ResetTotalSystemTime();
		

		//loops through all the system
		for (auto& System : ecs->m_ECS_SystemMap) {
			auto start = std::chrono::steady_clock::now();
			

			System.second->m_Update();


			auto end = std::chrono::steady_clock::now();

			std::chrono::duration<float> duration = end - start;
			

			performancetracker::Performance::m_UpdateTotalSystemTime(duration.count());
			performancetracker::Performance::m_UpdateSystemTime(System.first, duration.count());
		

		}
		
	}

	void ECS::m_Unload() {



		//delete ecs;
	}

	void* ECS::m_AddComponent(ComponentType Type, EntityID ID) {

		ECS* ecs = ECS::m_GetInstance();

		//checks if component already exist
		if (ecs->m_ECS_EntityMap[ID].test(Type)) {
			LOGGING_WARN("Entity Already Has Component");
			return NULL;
		}

		void* ComponentPtr = ecs->m_ECS_CombinedComponentPool[Type]->m_AssignComponent(ID);

		ecs->m_ECS_EntityMap.find(ID)->second.set(Type);

		//checks if new component fufils any of the system requirements
		m_RegisterSystems(ID);

		return ComponentPtr;
	}

	bool ECS::m_RemoveComponent(ComponentType Type, EntityID ID) {

		ECS* ecs = ECS::m_GetInstance();

		//checks if component already exist
		if (!ecs->m_ECS_EntityMap[ID].test(Type)) {
			LOGGING_WARN("Entity Component has already been removed");
			return false;
		}

		ecs->m_ECS_CombinedComponentPool[Type]->m_DeleteEntityComponent(ID);

		//deregister everthing
		m_DeregisterSystem(ID);

		ecs->m_ECS_EntityMap.find(ID)->second.reset(Type);

		//register everything
		m_RegisterSystems(ID);

		return true;
	}

	void ECS::m_RegisterSystems(EntityID ID) {

		ECS* ecs = ECS::m_GetInstance();
		for (auto& system : ecs->m_ECS_SystemMap) {
			if ((ecs->m_ECS_EntityMap.find(ID)->second & system.second->m_SystemSignature) == system.second->m_SystemSignature) {

				system.second->m_RegisterSystem(ID);

			}
		}
	}

	void ECS::m_DeregisterSystem(EntityID ID) {

		ECS* ecs = ECS::m_GetInstance();
		for (auto& system : ecs->m_ECS_SystemMap) {
			if ((ecs->m_ECS_EntityMap.find(ID)->second & system.second->m_SystemSignature) == system.second->m_SystemSignature) {

				system.second->m_DeregisterSystem(ID);

			}
		}

	}

	EntityID ECS::m_CreateEntity() {

		ECS* ecs = ECS::m_GetInstance();

		//TODO change to sth other than assert
		if (ecs->m_EntityCount >= MaxEntity) {
			LOGGING_ASSERT_WITH_MSG("Max Entity Count has been reached");
		}
		//assert(ecs->m_EntityCount < MaxEntity, "Max Entity Count has been reached");

		EntityID ID = ecs->m_EntityCount;

		// set bitflag to 0
		ecs->m_ECS_EntityMap[ID] = 0;

		//add transform component and name component as default
		m_AddComponent(TYPENAMECOMPONENT, ID);
		m_AddComponent(TYPETRANSFORMCOMPONENT, ID);

		ecs->m_EntityCount++;

		//assign entity to default layer
		ecs->m_layersStack.m_layerMap[layer::DEFAULT].second.push_back(ID);

		return ID;
	}

	EntityID ECS::m_DuplicateEntity(EntityID DuplicatesID) {

		ECS* ecs = ECS::m_GetInstance();
		EntityID NewEntity = ecs->m_CreateEntity();

		compSignature DuplicateSignature = ecs->m_ECS_EntityMap.find(DuplicatesID)->second;

		for (size_t n{}; n < TOTALTYPECOMPONENT; n++) {

			if (DuplicateSignature.test((ComponentType)n)) {

				ecs->m_ECS_CombinedComponentPool[(ComponentType)n]->m_DuplicateComponent(DuplicatesID, NewEntity);

			}

		}

		ecs->m_ECS_EntityMap.find(NewEntity)->second = DuplicateSignature;
		m_RegisterSystems(NewEntity);

		//checks if duplicates entity has parent and assign it
		if (Hierachy::m_GetParent(DuplicatesID).has_value()) {
			TransformComponent* transform = (TransformComponent*)(ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(Hierachy::m_GetParent(DuplicatesID).value()));
			transform->m_childID.push_back(NewEntity);
		}

		//checks if entity has child call recursion
		if (Hierachy::m_GetChild(DuplicatesID).has_value()) {
			//clear child id of vector for new entity
			TransformComponent* transform = (TransformComponent*)(ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(NewEntity));
			transform->m_childID.clear();

			std::vector<EntityID> childID = Hierachy::m_GetChild(DuplicatesID).value();
			for (const auto& child : childID) {
				EntityID dupChild = m_DuplicateEntity(child);
				Hierachy::m_SetParent(NewEntity, dupChild);
			}
		}

		return NewEntity;

	}

	bool ECS::m_DeleteEntity(EntityID ID) {

		ECS* ecs = ECS::m_GetInstance();

		Hierachy::m_RemoveParent(ID);
		
		//get child
		if (Hierachy::m_GetChild(ID).has_value()) {
			std::vector<EntityID> childs = Hierachy::m_GetChild(ID).value();
			for (auto& x : childs) {
				m_DeleteEntity(x);
			}
		}


		// refector
		m_DeregisterSystem(ID);

		ecs->m_ECS_EntityMap.erase(ID);

		return true;
	}

	
	

}



