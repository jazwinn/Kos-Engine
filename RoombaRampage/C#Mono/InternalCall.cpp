/********************************************************************/
/*!
\file      InternalCall.h
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Nov 11, 2024
\brief     This header file implements the functions that allow C# scripts to interact with ECS components.
		   - m_InternalGetTransformComponent: Retrieves transform data of an entity.
		   - m_InternalSetTransformComponent: Sets transform data of an entity.
		   - m_InternalGetVelocity: Retrieves the velocity of an entity.
		   - m_InternalSetVelocity: Sets the velocity of an entity.
		   - m_InternalCallIsKeyPressed: Checks if a key is currently pressed.
		   - m_InternalCallGetDeltaTime: Retrieves the fixed delta time.
		   - m_RegisterInternalCalls: Registers internal calls for Mono scripting.

Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/


#include "InternalCall.h"
#include "../Inputs/Input.h"
#include "../Application/Helper.h"

#define MONO_ADD_INTERNAL_CALL(METHOD_NAME) \
    mono_add_internal_call("Namespace.ScriptBase::" #METHOD_NAME, METHOD_NAME);

namespace script {

	bool InternalCall::m_InternalGetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate)
	{
		auto* transform = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entity));

		if (transform == nullptr) return false;

		*trans = { transform->m_position.m_x, transform->m_position.m_y };
		*scale = { transform->m_scale.m_x, transform->m_scale.m_y };
		*rotate = { transform->m_rotation };

		return true;
	}



	bool InternalCall::m_InternalSetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate)
	{
		auto* transform = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entity));

		if (transform == nullptr) return false;

		transform->m_position = *trans;
		transform->m_scale = *scale;
		transform->m_rotation = *rotate;

		return true;
	}

	bool InternalCall::m_InternalGetTranslate(ecs::EntityID entity, vector2::Vec2* trans)
	{
		auto* transform = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entity));
		*trans = { transform->m_position.m_x, transform->m_position.m_y };

		return true;
	}

	bool InternalCall::m_InternalSetTranslate(ecs::EntityID entity, vector2::Vec2* trans)
	{
		auto* transform = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entity));
		transform->m_position = *trans;

		return true;
	}

	bool InternalCall::m_InternalGetVelocity(ecs::EntityID entity, vector2::Vec2* vec)
	{
		auto* rbcomp = static_cast<ecs::RigidBodyComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(entity));
		if (rbcomp == nullptr) return false;

		*vec = rbcomp->m_Velocity;

		return true;
	}

	bool InternalCall::m_InternalSetVelocity(ecs::EntityID entity, vector2::Vec2* vec)
	{
		auto* rbcomp = static_cast<ecs::RigidBodyComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(entity));
		if (rbcomp == nullptr) return false;

		rbcomp->m_Velocity = *vec;

		return true;
	}


	bool InternalCall::m_InternalCallIsKeyPressed(keyCode key)
	{
		return Input::InputSystem::m_isKeyPressed(key);
	}

	bool InternalCall::m_InternalCallGetDeltaTime(float* deltatime)
	{
		*deltatime = Helper::Helpers::GetInstance()->m_fixedDeltaTime;

		return false;
	}

	int InternalCall::m_InternalCallGetPlayer()
	{

		ecs::ECS* ecs = ecs::ECS::m_GetInstance();

		for (const auto& scene : ecs->m_ECS_SceneMap) {
			if (scene.second.m_isPrefab == false && scene.second.m_isActive) {
				for (const auto& id : scene.second.m_sceneIDs) {
					if (ecs->m_ECS_EntityMap.find(id)->second.test(ecs::TYPEPLAYERCOMPONENT)) {
						return id;

						break;
					}
				}
			}
		}


		return -1;
	}

	bool InternalCall::m_InternalCallIsCollided(ecs::EntityID entity)
	{

		ecs::ECS* ecs = ecs::ECS::m_GetInstance();

		return static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(entity))->m_isCollided;

	}


	void InternalCall::m_RegisterInternalCalls()
	{
		MONO_ADD_INTERNAL_CALL(m_InternalGetTransformComponent);
		MONO_ADD_INTERNAL_CALL(m_InternalSetTransformComponent);
		MONO_ADD_INTERNAL_CALL(m_InternalCallIsKeyPressed);
		MONO_ADD_INTERNAL_CALL(m_InternalGetVelocity);
		MONO_ADD_INTERNAL_CALL(m_InternalSetVelocity);
		MONO_ADD_INTERNAL_CALL(m_InternalCallGetDeltaTime);
		MONO_ADD_INTERNAL_CALL(m_InternalCallGetPlayer);
		MONO_ADD_INTERNAL_CALL(m_InternalGetTranslate);
		MONO_ADD_INTERNAL_CALL(m_InternalSetTranslate);
		MONO_ADD_INTERNAL_CALL(m_InternalCallIsCollided);
	}

}

