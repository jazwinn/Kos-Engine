/******************************************************************/
/*!
\file      ComponentPool.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains declaration and defination for the
		   component pool. The component pool stores a pool of 
		   data for each component.


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef COMPOOL_H
#define COMPOOL_H


#include <vector>

#include "../ECS/ECSList.h"
#include "Component.h"
#include "TransformComponent.h"
#include "NameComponent.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"
#include "PlayerComponent.h"
#include "TextComponent.h"
#include "AnimationComponent.h"
#include "CameraComponent.h"
#include "ScriptComponent.h"

#include <algorithm>


namespace ecs {
/******************************************************************/
/*!
\enum     IComponentPool
\brief    Parent class for the component pool, contains pure virtual
		  functions that every child will have.
*/
/******************************************************************/
	class IComponentPool {

	public:
		/******************************************************************/
		/*!
		\fn        m_AssignComponent(EntityID)
		\brief     searches the pool for a inactive component and assign the
					entity to it.
		\return    pointer to the newly assigned class
		*/
		/******************************************************************/
		virtual void* m_AssignComponent(EntityID) = 0;
		/******************************************************************/
		/*!
		\fn        m_GetEntityComponent(EntityID)
		\brief     searches the pool for the entity's component and returns
					the address
		\return    pointer to the component
		*/
		/******************************************************************/
		virtual void* m_GetEntityComponent(EntityID) = 0;
		/******************************************************************/
		/*!
		\fn        m_HasComponent(EntityID)
		\brief     Searches the component pool if the entity's component is
				   present.
		\return    returns true if entity's component exist, false otherwise
		*/
		/******************************************************************/
		virtual bool m_HasComponent(EntityID) = 0;

		virtual bool m_DeleteEntityComponent(EntityID) = 0;
		/******************************************************************/
		/*!
		\fn        m_DuplicateComponent(EntityID DuplicatesID, EntityID NewID)
		\brief     Assign a new component and duplicate the data with another
		\return    pointer to the newly duplciated component
		*/
		/******************************************************************/
		virtual void* m_DuplicateComponent(EntityID, EntityID) = 0;

	};

	/******************************************************************/
	/*!
	\enum     ComponentPool
	\brief    Stores a pool of components 
	*/
	/******************************************************************/
	template <typename T>
	class ComponentPool : public IComponentPool {

	public:
	/******************************************************************/
	/*!
	\fn        ComponentPool()
	\brief     Constructor that resizes the pool
	*/
	/******************************************************************/
		ComponentPool();

		void* m_AssignComponent(EntityID) override;

		void* m_GetEntityComponent(EntityID) override;

		bool m_HasComponent(EntityID) override;

		bool m_DeleteEntityComponent(EntityID) override;

		void* m_DuplicateComponent(EntityID DuplicatesID, EntityID NewID) override;

		std::vector<T> m_Pool;
	};


	template<typename T>
	ComponentPool<T>::ComponentPool() {

		m_Pool.resize(MaxEntity);


	}

	template <typename T>
	void* ComponentPool<T>::m_AssignComponent(EntityID ID) {


		for (auto& Component : m_Pool) {
			if (Component.m_IsLive == false) {
				Component.m_IsLive = true;
				Component.m_Entity = ID;
				return &Component;
			}
		}



		// return NULL if all component is stored
		// SAY COMPONENT POOL IS FULL
		return NULL;

	}


	template <typename T>
	void* ComponentPool<T>::m_DuplicateComponent(EntityID DuplicateID, EntityID NewID) {

		//UNTESED MIGHT FAIL


		T* DuplicateComponent = (T*)m_GetEntityComponent(DuplicateID);

		T* NewComponent;
		if (m_HasComponent(NewID)) {
			NewComponent = (T*)m_GetEntityComponent(NewID);
		}
		else {
			NewComponent = (T*)m_AssignComponent(NewID);
		}

		

		*NewComponent = *DuplicateComponent;//copies everything INCLUDING ID
		NewComponent->m_Entity = NewID;//SET BACK ID

		return NewComponent;
	}



	template <typename T>
	void* ComponentPool<T>::m_GetEntityComponent(EntityID ID) {

		for (auto& Component : m_Pool) {
			if (Component.m_Entity == ID && Component.m_IsLive) {
				return &Component;
			}
		}

		//auto it = std::find_if(m_Pool.begin(), m_Pool.end(), [ID](const auto& obj) { return (obj.m_Entity == ID && obj.m_IsLive); });
		//if (it != m_Pool.end()) {
		//	return &(*it);
		//}


		//No Component Allocated to Entity
		//SAY ENTITY NOT CREATED
		LOGGING_ASSERT("Accessing non-existence entityID")
		return NULL;
	}


	template <typename T>
	bool ComponentPool<T>::m_DeleteEntityComponent(EntityID ID) {

		//TODO delete component from system vector
		for (auto& Component : m_Pool) {
			if (Component.m_Entity == ID && Component.m_IsLive) {
				//Component.m_IsLive = false;
			}
		}


		//task failed
		return false;
	}

	template <typename T>
	bool ComponentPool<T>::m_HasComponent(EntityID ID) { //contained any stored data

		for (auto& Component : m_Pool) {
			if (Component.m_Entity == ID) {
				return Component.m_IsLive;
			}
		}

		//if (std::find_if(m_Pool.begin(), m_Pool.end(), [ID](const auto& obj) { return obj.m_Entity == ID; }) != m_Pool.end()) {
		//	return true;
		//}

		return false;
	}


}




#endif COMPOOL_H