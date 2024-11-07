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


	void InternalCall::m_RegisterInternalCalls()
	{
		mono_add_internal_call("Namespace.ScriptBase::""m_InternalGetTransformComponent", m_InternalGetTransformComponent);
		mono_add_internal_call("Namespace.ScriptBase::""m_InternalSetTransformComponent", m_InternalSetTransformComponent);
		mono_add_internal_call("Namespace.ScriptBase::""m_InternalCallIsKeyPressed", m_InternalCallIsKeyPressed);
		mono_add_internal_call("Namespace.ScriptBase::""m_InternalGetVelocity", m_InternalGetVelocity);
		mono_add_internal_call("Namespace.ScriptBase::""m_InternalSetVelocity", m_InternalSetVelocity);
		mono_add_internal_call("Namespace.ScriptBase::""m_InternalCallGetDeltaTime", m_InternalCallGetDeltaTime);
	}

}

