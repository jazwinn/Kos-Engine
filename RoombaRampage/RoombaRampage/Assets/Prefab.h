#ifndef PREFAB_H
#define PREFAB_H

#include <vector>
#include <unordered_map>
#include <string>
#include "../ECS/Component/ComponentPool.h"
#include "../ECS/ECSList.h"
#include "../ECS/Component/ComponentList.h"


namespace prefab {

	class Prefab {

	public:
		static void m_DeSerializePrefab(std::string path);

		static int m_CreateEntityFromPrefab(std::string Prefab);

		static std::unordered_map<std::string, Prefab> m_prefabs;
	private:

		std::bitset<Ecs::TotalTypeComponent> m_prefabSignature;


		//Add vector for every new components
		Ecs::MovementComponent m_movementComponents;
		Ecs::PlayerComponent m_playerComponents;
		Ecs::SpriteComponent m_spriteComponents;
		Ecs::TransformComponent m_transformComponents;
		Ecs::ColliderComponent m_colliderComponents;
		Ecs::RigidBodyComponent m_rigidBodyComponents;

		


	};

}




#endif PREFAB_H

