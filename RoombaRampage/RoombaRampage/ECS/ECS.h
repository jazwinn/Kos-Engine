#ifndef ECS_H
#define ECS_H

#include <unordered_map>
#include <memory>

#include "ComponentPool.h"
#include "Component.h"

#include "System.h"

#include "ECSList.h"

#include <iostream>



class ECS {
	
private:
	

	//using EntityMap;
	using CombinedComponentPool = std::unordered_map<EntityID, IComponentPool*>;
	using SystemMap = std::unordered_map<TypeSystem, ISystem*>;
	//using ComponentPoolMap = std::unordered_map<ComponentType, std::unique_ptr<ComponentPool>>;

	ECS() = default;

	

public:
	//singleton
	static ECS* GetInstance() { return InstancePtr; }

	//load the programme
	static void Load();

	//Initializes the programme
	static void Init();

	//Update loops that updates the ECS
	static void Update();

	//Unloads and free all memory
	static void Unload();


	static EntityID CreateEntity();

	static bool DeleteEntity(EntityID);


	CombinedComponentPool ECS_CombinedComponentPool{};

	SystemMap ECS_SystemMap{};


	EntityID EntityCount{};

private:
	static ECS* InstancePtr;

};


#endif  ECS_H