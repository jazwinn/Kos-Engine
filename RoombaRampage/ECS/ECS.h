/******************************************************************/
/*!
\file      ECS.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the ECS class that is the factor that
		   creates, destroy and duplicates entity. It is also the point
		   to retrieve component pointers of all component type.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef ECS_H
#define ECS_H

#include <unordered_map>
#include <memory>

#include "../ECS/Component/ComponentPool.h"
#include "../ECS/Component/Component.h"

#include "../ECS/System/System.h"
#include "Layers.h"

#include "ECSList.h"
#include "../ECS/System/SystemList.h"

#include <iostream>
#include <bitset>
#include <optional>
#include <functional>

#include <any>

namespace ecs {

	class ECS {

	private:



		ECS() = default;



	public:
		//singleton
		static ECS* m_GetInstance() {
			if (!m_InstancePtr) {
				m_InstancePtr.reset(new ECS{});
			}
			return m_InstancePtr.get();
		}

		/******************************************************************/
		/*!
		\def       m_Load()
		\brief     Allocate memory to the the heap and storing it on a map
		*/
		/******************************************************************/
		static void m_Load();

		/******************************************************************/
		/*!
		\def       m_Init()
		\brief     Call on all systems initialization
		*/
		/******************************************************************/
		static void m_Init();

		/******************************************************************/
		/*!
		\def       m_Update()
		\brief     Call on all systems update function
		*/
		/******************************************************************/
		static void m_Update(float DeltaTime);

		/******************************************************************/
		/*!
		\def       m_Unload()
		\brief     Release all memeory allocated
		*/
		/******************************************************************/
		static void m_Unload();

		/******************************************************************/
		/*!
		\def       m_CreateEntity()
		\brief     Creates an entity with a name & transform component
		*/
		/******************************************************************/
		static EntityID m_CreateEntity();
		/******************************************************************/
		/*!
		\def       m_DuplicateEntity()
		\brief     Creates entity and duplicate the data to the new entity
		*/
		/******************************************************************/
		static EntityID m_DuplicateEntity(EntityID);
		/******************************************************************/
		/*!
		\def       m_DeleteEntity()
		\brief     Removes entities component pointer from all system
		*/
		/******************************************************************/
		static bool m_DeleteEntity(EntityID);


		template <typename T>
		static void m_AddComponentToECS(ComponentType);

		/******************************************************************/
		/*!
		\def       m_AddComponent()
		\brief     Assign a component to the entity and update the entity
				   map bitflag
		\return	   pointer to the newly created component
		*/
		/******************************************************************/
		static void* m_AddComponent(ComponentType, EntityID);


		static bool m_RemoveComponent(ComponentType Type, EntityID ID);


		/******************************************************************/
		/*!
		\def       m_RegisterSystems()
		\brief     Checks entity map bitflag against all the system's signature
				   if any matches, the entitie's component pointer is stored
				   in the system
		*/
		/******************************************************************/
		static void m_RegisterSystems(EntityID);
		/******************************************************************/
		/*!
		\def       m_DeregisterSystem()
		\brief     Removes entity's component from the system
		*/
		/******************************************************************/
		static void m_DeregisterSystem(EntityID);

		//create getters
		std::unordered_map<ComponentType, std::shared_ptr<IComponentPool>> m_ECS_CombinedComponentPool{};

		std::unordered_map<TypeSystem, std::shared_ptr<ISystem>> m_ECS_SystemMap{};

		std::unordered_map<EntityID, std::bitset<TOTALTYPECOMPONENT>> m_ECS_EntityMap{};

		//store type conversion
		std::unordered_map<ComponentType, std::function<std::any(void*)>> m_conversion_Map;// TODO in future, std::any doesnt work

		//layering system
		layer::LayerStack m_layersStack;

		EntityID m_EntityCount{};

		float m_DeltaTime{};

	private:
		static std::unique_ptr<ECS> m_InstancePtr;

		

	};

	


}




#endif  ECS_H