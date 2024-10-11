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


		//Allocate memory to each system
		ecs->m_ECS_SystemMap[TYPECONTROLSYSTEM] = std::make_shared<ControlSystem>();
		ecs->m_ECS_SystemMap[TYPETRANSFORMSYSTEM] = std::make_shared<TransformSystem>();
		ecs->m_ECS_SystemMap[TYPECOLLISIONSYSTEM] = std::make_shared<CollisionSystem>();
		ecs->m_ECS_SystemMap[TYPECOLLISIONRESPONSESYSTEM] = std::make_shared<CollisionResponseSystem>();
		ecs->m_ECS_SystemMap[TYPEMOVEMENTSYSTEM] = std::make_shared<MovementSystem>(); // movement should be the last logic
		ecs->m_ECS_SystemMap[TYPERENDERSYSTEM] = std::make_shared<RenderSystem>();
		ecs->m_ECS_SystemMap[TYPERENDERTEXTSYSTEM] = std::make_shared<RenderTextSystem>();
		ecs->m_ECS_SystemMap[TYPEDEBUGDRAWINGSYSTEM] = std::make_shared<DebugDrawingSystem>();
		ecs->m_ECS_SystemMap[TYPEANIMATIONSYSTEM] = std::make_shared<AnimationSystem>();


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

		return NewEntity;

	}

	bool ECS::m_DeleteEntity(EntityID ID) {

		ECS* ecs = ECS::m_GetInstance();

		m_RemoveParent(ID);
		
		//get child
		if (m_GetChild(ID).has_value()) {
			std::vector<EntityID> childs = m_GetChild(ID).value();
			for (auto& x : childs) {
				m_DeleteEntity(x);
			}
		}


		// refector
		m_DeregisterSystem(ID);

		ecs->m_ECS_EntityMap.erase(ID);

		return true;
	}

	void ECS::m_SetParent(EntityID parent, EntityID child) {

		ECS* ecs = ECS::m_GetInstance();

		m_RemoveParent(child);

		TransformComponent* parentTransform =  (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(parent);
		//checks if child is already in parent
		if (m_GetParent(child).has_value()) {
			return;
		}

		//checks if parent is getting dragged into its child
		EntityID id = parent;
		while (m_GetParent(id).has_value()) {
			EntityID checkParentid = m_GetParent(id).value();
			if (checkParentid == child) {
				LOGGING_WARN("Cannot assign parent to its own child");
				return;
			}
			id = checkParentid;

		}


		parentTransform->m_childID.push_back(child);

		TransformComponent* childTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(child);
		childTransform->m_haveParent = true;
		childTransform->m_parentID = parent;
	}

	void ECS::m_RemoveParent(EntityID child) {
		// removes id from both the child and the parents vector
		ECS* ecs = ECS::m_GetInstance();

		if (!m_GetParent(child).has_value()) {
			// does not have parrent
			return;
		}

		EntityID parent = m_GetParent(child).value();
		TransformComponent* parentTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(parent);
		size_t pos{};
		for (EntityID& id : parentTransform->m_childID) {
			if (child == id) {
				parentTransform->m_childID.erase(parentTransform->m_childID.begin() + pos);
				break;
			}
			pos++;
		}


		TransformComponent* childTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(child);
		childTransform->m_haveParent = false;
		childTransform->m_parentID = 0;
	}

	std::optional<EntityID> ECS::m_GetParent(EntityID child)
	{
		ECS* ecs = ECS::m_GetInstance();
		TransformComponent* childTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(child);
		if (!childTransform->m_haveParent) {
			return std::optional<EntityID>();
		}

		return childTransform->m_parentID;

	}

	std::optional<std::vector<EntityID>>ECS::m_GetChild(EntityID parent)
	{
		ECS* ecs = ECS::m_GetInstance();
		TransformComponent* parentTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(parent);
		if (parentTransform->m_childID.size() <= 0) {
			return std::optional<std::vector<EntityID>>();
		}

		return parentTransform->m_childID;

	}
	

}



