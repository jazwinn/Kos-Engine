#ifndef PREFAB_H
#define PREFAB_H

#include <vector>
#include <unordered_map>
#include <string>
#include "../ECS/Component/ComponentPool.h"

namespace prefab {

	class Prefab {

	public:
		void m_DeSerializePrefab(std::string path);




	private:

		std::unordered_map<std::string, std::vector<Ecs::Component*>> m_prefab_Names;

		//Add vector for every new components
		std::vector<Ecs::MovementComponent> m_movementComponents;
		std::vector<Ecs::PlayerComponent> m_playerComponents;
		std::vector<Ecs::SpriteComponent> m_spriteComponents;
		std::vector<Ecs::TransformComponent> m_transformComponents;
		std::vector<Ecs::ColliderComponent> m_colliderComponents;
		std::vector<Ecs::RigidBodyComponent> m_rigidBodyComponents;


	};

}




#endif PREFAB_H

