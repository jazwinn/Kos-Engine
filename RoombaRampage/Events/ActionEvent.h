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
		ecs::TransformComponent* m_changedComponent;
		ecs::TransformComponent oldVal, newVal;
	public:
		TransformComponentChanged(ecs::ComponentType inType, ecs::EntityID inID, ecs::TransformComponent* inComp, ecs::TransformComponent inOld) : BaseEvent<Actions>(Actions::TRANSFORMCOMP),
			m_changedType(inType), m_entityChanged(inID), m_changedComponent(inComp), oldVal(inOld), newVal(*inComp){}
		ecs::ComponentType m_getComponentType() { return m_changedType; }
		ecs::EntityID m_getID() { return m_entityChanged; }
		ecs::TransformComponent* m_getComp() { return m_changedComponent; }
		ecs::TransformComponent m_getOld() { return oldVal; }
		ecs::TransformComponent m_getNew() { return newVal; }
	};

	class AddComponent : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityChanged;
		ecs::ComponentType m_addedType;
	public:
		AddComponent(ecs::EntityID inID, ecs::ComponentType inType) : BaseEvent<Actions>(Actions::ADDCOMP), m_entityChanged(inID),m_addedType(inType) {}
		ecs::ComponentType m_getComponentType() { return m_addedType; }
		ecs::EntityID m_getID() { return m_entityChanged; }
	};
	
	class RemoveComponent : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityChanged;
		ecs::ComponentType m_removedType;
	public:
		RemoveComponent(ecs::EntityID inID, ecs::ComponentType inType) : BaseEvent<Actions>(Actions::REMOVECOMP), m_entityChanged(inID), m_removedType(inType) {}
		ecs::ComponentType m_getComponentType() { return m_removedType; }
		ecs::EntityID m_getID() { return m_entityChanged; }
	};

	class UndoLatest : public BaseEvent<Actions> {
	public:
		UndoLatest() : BaseEvent<Actions>(Actions::UNDO) {}
	};

	class RedoPrevious : public BaseEvent<Actions> {
	public:
		RedoPrevious() : BaseEvent<Actions>(Actions::REDO) {}
	};
}
