#pragma once
#include <stack>
#include "../ECS/Component/Component.h"
#include "../ECS/ECSList.h"
#include "../ECS/ECS.h"

namespace actions {
	class Action {
	public:
		virtual void undo() = 0;
		virtual void redo() = 0;
		virtual ~Action() = 0;
	};
}
