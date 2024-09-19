#ifndef COMPOOL_H
#define COMPOOL_H


#include <vector>

#include "../ECS/ECSList.h"
#include "Component.h"
#include "TransformComponent.h"
#include "MovementComponent.h"
#include "SpriteComponent.h"

namespace Ecs {
	class IComponentPool {

	public:
		virtual void* AssignComponent(EntityID) = 0;

		virtual void* GetEntityComponent(EntityID) = 0;

		virtual bool HasComponent(EntityID) = 0;

		virtual bool DeleteEntityComponent(EntityID) = 0;

		virtual void* DuplicateComponent(EntityID, EntityID) = 0;

	};


	template <typename T>
	class ComponentPool : public IComponentPool {

	public:
		ComponentPool();

		void* AssignComponent(EntityID) override;

		void* GetEntityComponent(EntityID) override;

		bool HasComponent(EntityID) override;

		bool DeleteEntityComponent(EntityID) override;

		void* DuplicateComponent(EntityID DuplicatesID, EntityID NewID) override;

		std::vector<T> Pool;
	};


	template<typename T>
	ComponentPool<T>::ComponentPool() {

		Pool.resize(MaxEntity);


	}

	template <typename T>
	void* ComponentPool<T>::AssignComponent(EntityID ID) {


		for (auto& Component : Pool) {
			if (Component.IsLive == false) {
				Component.IsLive = true;
				Component.Entity = ID;
				return &Component;
			}
		}

		// return NULL if all component is stored
		// SAY COMPONENT POOL IS FULL
		return NULL;

	}


	template <typename T>
	void* ComponentPool<T>::DuplicateComponent(EntityID DuplicateID, EntityID NewID) {

		//UNTESED MIGHT FAIL


		auto* DuplicateComponent = (T*)GetEntityComponent(DuplicateID);

		auto* NewComponent = (T*)AssignComponent(NewID);

		*NewComponent = *DuplicateComponent;

		return NewComponent;
	}



	template <typename T>
	void* ComponentPool<T>::GetEntityComponent(EntityID ID) {

		for (auto& Component : Pool) {
			if (Component.Entity == ID && Component.IsLive) {
				return &Component;
			}
		}

		//No Component Allocated to Entity
		//SAY ENTITY NOT CREATED
		return NULL;
	}


	template <typename T>
	bool ComponentPool<T>::DeleteEntityComponent(EntityID ID) {

		//TODO delete component from system vector


		//task failed
		return false;
	}

	template <typename T>
	bool ComponentPool<T>::HasComponent(EntityID ID) { //contained any stored data

		for (auto& Component : Pool) {
			if (Component.Entity == ID) {
				return Component.IsLive;
			}
		}

		return false;
	}


}




#endif COMPOOL_H