#ifndef COMPOOL_H
#define COMPOOL_H


#include <vector>

#include "../ECS/ECSList.h"
#include "Component.h"
#include "TransformComponent.h"
#include "NameComponent.h"
#include "MovementComponent.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "RigidBody.h"
#include "PlayerComponent.h"


namespace ecs {
	class IComponentPool {

	public:
		virtual void* m_AssignComponent(EntityID) = 0;

		virtual void* m_GetEntityComponent(EntityID) = 0;

		virtual bool m_HasComponent(EntityID) = 0;

		virtual bool m_DeleteEntityComponent(EntityID) = 0;

		virtual void* m_DuplicateComponent(EntityID, EntityID) = 0;

	};


	template <typename T>
	class ComponentPool : public IComponentPool {

	public:
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

		//No Component Allocated to Entity
		//SAY ENTITY NOT CREATED
		return NULL;
	}


	template <typename T>
	bool ComponentPool<T>::m_DeleteEntityComponent(EntityID ID) {

		//TODO delete component from system vector


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

		return false;
	}


}




#endif COMPOOL_H