#pragma once

#include "mono_handler.h"
#include "../Inputs/Keycodes.h"

namespace script {

	class InternalCall {
	
		static bool m_InternalGetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate);

		static bool m_InternalSetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate);

		static bool m_InternalGetVelocity(ecs::EntityID entity, vector2::Vec2* vec);

		static bool m_InternalSetVelocity(ecs::EntityID entity, vector2::Vec2* vec);

		static bool m_InternalCallIsKeyPressed(keyCode key);

		static bool m_InternalCallGetDeltaTime(float* deltatime);

	public:
		static void m_RegisterInternalCalls();

	};

}