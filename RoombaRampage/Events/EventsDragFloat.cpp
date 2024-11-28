
#include "../Config/pch.h"
#include "EventsDragFloat.h"

namespace dragfloat {
	std::unique_ptr<dragfloat::DragFloatCheck> dragfloat::DragFloatCheck::m_instance = nullptr;

	/******************************************************************/
	/*!
		\brief     This is a derived class from BaseEvent to get the instance of the dragFloatCheck class
	*/
	/******************************************************************/
	dragfloat::DragFloatCheck* dragfloat::DragFloatCheck::m_GetInstance() {
		if (!m_instance) {
			m_instance = std::make_unique<DragFloatCheck>();
		}
		return m_instance.get();
	}
}