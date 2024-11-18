#include "../Config/pch.h"
#include "ActionManager.h"

namespace actions {
	std::unique_ptr<ActionManager> ActionManager::m_instance = nullptr;

	actions::ActionManager* actions::ActionManager::m_GetManagerInstance() {
		if (!m_instance) {
			m_instance = std::make_unique<ActionManager>();
		}
		return m_instance.get();
	}
}