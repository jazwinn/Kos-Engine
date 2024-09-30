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

namespace ecs {

	class ECS {

	private:


		//using EntityMap;

		using CombinedComponentPool = std::unordered_map<ComponentType, std::shared_ptr<IComponentPool>>;
		using SystemMap = std::unordered_map<TypeSystem, std::shared_ptr<ISystem>>;
		using EntityMap = std::unordered_map<EntityID, std::bitset<TOTALTYPECOMPONENT>>;
		//using ComponentPoolMap = std::unordered_map<ComponentType, std::unique_ptr<ComponentPool>>;

		ECS() = default;



	public:
		//singleton
		static ECS* m_GetInstance() {
			if (!m_InstancePtr) {
				m_InstancePtr.reset(new ECS{});
			}
			return m_InstancePtr.get();
		}

		//load the programme
		static void m_Load();

		//Initializes the programme
		static void m_Init();

		//Update loops that updates the ECS
		static void m_Update(float DeltaTime);

		//Unloads and free all memory
		static void m_Unload();


		static EntityID m_CreateEntity();

		static EntityID m_DuplicateEntity(EntityID);

		static bool m_DeleteEntity(EntityID);

		static void* m_AddComponent(ComponentType, EntityID);

		static void m_RegisterSystems(EntityID);

		static void m_DeregisterSystem(EntityID);


		CombinedComponentPool m_ECS_CombinedComponentPool{};

		SystemMap m_ECS_SystemMap{};

		EntityMap m_ECS_EntityMap{};

		EntityID m_EntityCount{};

		float m_DeltaTime{};

	private:
		static std::unique_ptr<ECS> m_InstancePtr;

	};

}




#endif  ECS_H