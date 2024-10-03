/******************************************************************/
/*!
\file      Prefab.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file the class prefabs that read an json file and 
		   stores each object into the prefabs. When the function
		   is called. The prefab, would create and clone its data
		   onto the entity


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

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
	/******************************************************************/
	/*!
	\fn        m_DeSerializePrefab(std::string path)
	\brief     Reads json file and copies data into the prefab
	\param[in] path  The file path of the json file
	*/
	/******************************************************************/
		static void m_DeSerializePrefab(std::string path);
	/******************************************************************/
	/*!
	\fn        m__CreateEntityFromPrefab(std::string Prefab)
	\brief     Creates a new entity and copies the prefab data into the entity
	\param[in] Prefab  The string name of the prefab to copy
	*/
	/******************************************************************/
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

