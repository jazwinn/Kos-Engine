#pragma once

#include "mono_handler.h"
#include "../Inputs/Keycodes.h"

namespace script {

	class InternalCall {
	
		static void m_InternalGetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate);

		static void m_InternalGetRigidBodyComponent(ecs::EntityID entity, ecs::RigidBodyComponent* rbcomp);

		static bool m_InternalCallIsKeyPressed(keyCode key);

	public:
		static void m_RegisterInternalCalls();

	};

}