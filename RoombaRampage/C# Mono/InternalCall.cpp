#include "InternalCall.h"
#include "../Inputs/Input.h"

namespace script {

	void InternalCall::m_InternalGetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate)
	{
		auto* transform = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entity));

		*trans = { transform->m_position.m_x, transform->m_position.m_y };
		*scale = { transform->m_scale.m_x, transform->m_scale.m_y };
		*rotate = { transform->m_rotation };

	}

	void InternalCall::m_InternalGetRigidBodyComponent(ecs::EntityID entity, ecs::RigidBodyComponent* rbcomp)
	{
		rbcomp = static_cast<ecs::RigidBodyComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(entity));

	}

	bool InternalCall::m_InternalCallIsKeyPressed(keyCode key)
	{
		if (Input::InputSystem::m_isKeyPressed(key)) {
			std::cout << "pressed" << std::endl;
			return true;
		}
		else {
			std::cout << "not pressed" << std::endl;
		}
		return false;
	}


	void InternalCall::m_RegisterInternalCalls()
	{

		mono_add_internal_call("Namespace.ScriptBase::""m_InternalGetTransformComponent", m_InternalGetTransformComponent);
		mono_add_internal_call("Namespace.ScriptBase::""m_InternalCallIsKeyPressed", m_InternalCallIsKeyPressed);
		mono_add_internal_call("Namespace.ScriptBase::""m_InternalGetRigidBodyComponent", m_InternalGetRigidBodyComponent);

	}

}

