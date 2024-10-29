#include "InternalCall.h"

namespace script {

	void InternalCall::m_InternalGetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate)
	{
		auto* transform = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entity));

		*trans = { transform->m_position.m_x, transform->m_position.m_y };
		*scale = { transform->m_scale.m_x, transform->m_scale.m_y };
		*rotate = { transform->m_rotation };

	}


	void InternalCall::m_RegisterInternalCalls()
	{

		mono_add_internal_call("Namespace.ScriptBase::""m_InternalGetTransformComponent", m_InternalGetTransformComponent);

	}

}

