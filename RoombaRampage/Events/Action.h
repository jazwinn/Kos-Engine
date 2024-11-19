#pragma once
#include <stack>
#include "../ECS/Component/Component.h"
#include "../ECS/ECSList.h"
#include "../ECS/ECS.h"

namespace actions {
	class Action {
	public:
		virtual void m_undoAction() = 0;
		virtual void m_redoAction() = 0;
		virtual ~Action() = default;
	};
}
