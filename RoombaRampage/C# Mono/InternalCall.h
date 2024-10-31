#pragma once

#include "mono_handler.h"

namespace script {

	class InternalCall {
	
		static void m_InternalGetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate);


	public:
		static void m_RegisterInternalCalls();

	};

}