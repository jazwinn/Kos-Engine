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

#include "../Config/pch.h"

#include "../ECS/Component/ComponentPool.h"
#include "../ECS/Component/Component.h"

#include "../ECS/System/System.h"

#include "ECSList.h"
#include "../ECS/System/SystemList.h"


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
		\def       m_Update(const std::string&)
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
		static EntityID m_CreateEntity(std::string scene);
		/******************************************************************/
		/*!
		\def       m_DuplicateEntity()
		\brief     Creates entity and duplicate the data to the new entity
		*/
		/******************************************************************/
		static EntityID m_DuplicateEntity(EntityID, std::string scene = {});
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


		std::unordered_map<ComponentType, std::shared_ptr<IComponentPool>> m_ECS_CombinedComponentPool{};

		std::unordered_map<TypeSystem, std::shared_ptr<ISystem>> m_ECS_SystemMap{};

		std::unordered_map<EntityID, std::bitset<TOTALTYPECOMPONENT>> m_ECS_EntityMap{};

<<<<<<< HEAD
		struct SceneID{
			bool m_isActive{true};
			bool m_isPrefab{ false };
			ecs::EntityID m_prefabID = 0;
			std::vector<EntityID> m_sceneIDs;

		};

		std::unordered_map<std::string, SceneID> m_ECS_SceneMap{};// store scene file name e.g. scene.json 

		//store type conversion
		//using ActionFunction = std::function<void(void*, void (*)(void*))>;
		//std::unordered_map<ComponentType, ActionFunction> m_callFunctionToComponent_Map;// TODO in future, std::any doesnt work

		//layering system
		layer::LayerStack m_layersStack;

=======
>>>>>>> 2614f36e3dde51625ed71ac1889d9f61bb456128
		EntityID m_EntityCount{};

		float m_DeltaTime{};

		GAMESTATE m_nextState{STOP};
		GAMESTATE m_getState() { return m_state; };

	private:
		static std::unique_ptr<ECS> m_InstancePtr;

<<<<<<< HEAD
		//modify from set next state
		GAMESTATE m_state{};

=======
>>>>>>> 2614f36e3dde51625ed71ac1889d9f61bb456128
	};

	


}




#endif  ECS_H