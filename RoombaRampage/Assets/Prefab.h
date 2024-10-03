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

		
	private:

		std::bitset<ecs::TOTALTYPECOMPONENT> m_prefabSignature;


		//Add vector for every new components
		ecs::MovementComponent m_movementComponents;
		ecs::NameComponent m_nameComponents;
		ecs::PlayerComponent m_playerComponents;
		ecs::SpriteComponent m_spriteComponents;
		ecs::TransformComponent m_transformComponents;
		ecs::ColliderComponent m_colliderComponents;
		ecs::RigidBodyComponent m_rigidBodyComponents;
		ecs::TextComponent m_textComponent;
		ecs::AnimationComponent m_animationComponent;
		


	};

}




#endif PREFAB_H

