#ifndef ECS_H
#define ECS_H

#include <unordered_map>
#include <memory>

#include "../ECS/Component/ComponentPool.h"
#include "../ECS/Component/Component.h"

#include "../ECS/System/System.h"

#include "ECSList.h"
#include "../ECS/System/SystemList.h"

#include <iostream>
#include <bitset>



class ECS {
	
private:
	

	//using EntityMap;

	using CombinedComponentPool = std::unordered_map<ComponentType, IComponentPool*>;
	using SystemMap = std::unordered_map<TypeSystem, ISystem*>;
	using EntityMap = std::unordered_map<EntityID, std::bitset<TotalTypeComponent>>;
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

	static void AddComponent(ComponentType ,EntityID );

	static void RegisterSystems(EntityID);

	static void DeregisterSystem(EntityID);


	CombinedComponentPool ECS_CombinedComponentPool{};

	SystemMap ECS_SystemMap{};

	EntityMap ECS_EntityMap{};

	EntityID EntityCount{};



private:
	static ECS* InstancePtr;

};


#endif  ECS_H