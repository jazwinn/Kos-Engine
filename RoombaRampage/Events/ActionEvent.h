/*!
\file      ActionEvent.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the declaration and definitions of the specialised action event classes for undo/redo

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#include "BaseEvent.h"
#include "../Math/vector2.h"
#include "../Math/Mat3x3.h"
#include "ModifyAction.h"

namespace events {
	class TransformComponentChanged : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityChanged;
		ecs::Component* m_changedComponent;
		ecs::Component oldVal, newVal;
	public:
		TransformComponentChanged(ecs::ComponentType inType, ecs::EntityID inID, ecs::Component* inComp, ecs::Component inOld) : BaseEvent<Actions>(Actions::TRANSFORMCOMP), 
			m_changedType(inType), m_entityChanged(inID), m_changedComponent(inComp), oldVal(inOld), newVal(*inComp){}
		ecs::ComponentType m_getComponentType() { return m_changedType; }
		ecs::EntityID m_getID() { return m_entityChanged; }
		ecs::Component* m_getComp() { return m_changedComponent; }
		ecs::Component m_getOld() { return oldVal; }
		ecs::Component m_getNew() { return newVal; }
	};

	class UndoLatest : public BaseEvent<Actions> {
	public:
		UndoLatest() : BaseEvent<Actions>(Actions::UNDO) {}
	};
}
