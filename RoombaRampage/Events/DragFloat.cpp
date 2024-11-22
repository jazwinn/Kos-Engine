
#include "../Config/pch.h"
#include "DragFloat.h"

namespace DragFloat {
	std::unique_ptr<DragFloat::dragFloatCheck> DragFloat::dragFloatCheck::m_instance = nullptr;

	DragFloat::dragFloatCheck* DragFloat::dragFloatCheck::m_GetInstance() {
		if (!m_instance) {
			m_instance = std::make_unique<dragFloatCheck>();
		}
		return m_instance.get();
	}
}