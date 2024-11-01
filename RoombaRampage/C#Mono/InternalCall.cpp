#include "InternalCall.h"
#include "../Inputs/Input.h"

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


	void InternalCall::m_RegisterInternalCalls()
	{

		mono_add_internal_call("Namespace.ScriptBase::""m_InternalGetTransformComponent", m_InternalGetTransformComponent);
		mono_add_internal_call("Namespace.ScriptBase::""m_InternalCallIsKeyPressed", m_InternalCallIsKeyPressed);
		mono_add_internal_call("Namespace.ScriptBase::""m_InternalGetVelocity", m_InternalGetVelocity);
		mono_add_internal_call("Namespace.ScriptBase::""m_InternalSetVelocity", m_InternalSetVelocity);
	}

}

