#ifndef COMPOOL_H
#define COMPOOL_H


#include <vector>

#include "ECSList.h"
#include "Component.h"
#include "TransformComponent.h"
#include "MovementComponent.h"

class IComponentPool {

public:
	virtual void* CreateComponent() = 0;

	virtual void* GetEntityComponent(EntityID) = 0;

	virtual bool HasComponent(EntityID) = 0;

	virtual bool DeleteEntityComponent(EntityID) = 0;

};


template <typename T>
class ComponentPool : public IComponentPool {

public:
	ComponentPool();
	 
	void* CreateComponent() override;

	void* GetEntityComponent(EntityID) override;

	bool HasComponent(EntityID) override;
	
	bool DeleteEntityComponent(EntityID) override;



	std::vector<T> Pool;
};


template<typename T>
ComponentPool<T>::ComponentPool() {

	Pool.resize(MaxEntity);


}

template <typename T>
void* ComponentPool<T>::CreateComponent() {

	int count{};
	for (auto& Component : Pool) {
		if (Component.IsLive == false) {
			Component.IsLive = true;
			Component.Entity = count;
			return &Component;
		}

		count++;
	}

	// return NULL if all component is stored
	// SAY COMPONENT POOL IS FULL
	return NULL;

}



template <typename T>
void* ComponentPool<T>::GetEntityComponent(EntityID ID){

	for (auto& Component : Pool) {
		if (Component.Entity == ID) {
			Component.IsStored = true;
			return &Component;
		}
	}

	//No Component Allocated to Entity
	//SAY ENTITY NOT CREATED
	return NULL;
}


template <typename T>
bool ComponentPool<T>::DeleteEntityComponent(EntityID ID){
	for (auto& Component : Pool) {
		if (Component.Entity == ID) {
			Component.IsLive = false;
			Component.IsStored = false;

			//Task successful
			return true;
		}
	}

	//task failed
	return false;
}

template <typename T>
bool ComponentPool<T>::HasComponent(EntityID ID) { //contained any stored data

	for (auto& Component : Pool) {
		if (Component.Entity == ID) {
			return Component.IsStored;
		}
	}

	return false;
}


#endif COMPOOL_H