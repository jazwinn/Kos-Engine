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
#include "EventsBaseEvent.h"
#include "../Math/vector2.h"
#include "../Math/Mat3x3.h"
#include "../Actions/ModifyAction.h"

namespace events {
	class TransformComponentChanged : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityChanged;
		ecs::TransformComponent* m_changedComponent;
		vector2::Vec2 m_oldPos;
		float m_oldRot;
		vector2::Vec2 m_oldScale;
		mat3x3::Mat3x3 m_oldTrans;
		//ecs::TransformComponent oldVal, newVal;
	public:
		TransformComponentChanged(ecs::ComponentType inType, ecs::EntityID inID, ecs::TransformComponent* inComp, ecs::TransformComponent inOld) : BaseEvent<Actions>(Actions::TRANSFORMCOMP),
			m_changedType(inType), m_entityChanged(inID), m_changedComponent(inComp),m_oldPos(inOld.m_position), m_oldRot(inOld.m_rotation), m_oldScale(inOld.m_scale), m_oldTrans(inOld.m_transformation){}
		ecs::ComponentType m_GetComponentType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityChanged; }
		ecs::TransformComponent* m_GetComp() { return m_changedComponent; }
		vector2::Vec2 m_GetOldPos() { return m_oldPos; }
		vector2::Vec2 m_GetOldScale() { return m_oldScale; }
		float m_GetOldRot() { return m_oldRot; }
		mat3x3::Mat3x3 m_GetOldTrans() { return m_oldTrans; }

	};

	class AddComponent : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityChanged;
		ecs::ComponentType m_addedType;
	public:
		AddComponent(ecs::EntityID inID, ecs::ComponentType inType) : BaseEvent<Actions>(Actions::ADDCOMP), m_entityChanged(inID),m_addedType(inType) {}
		ecs::ComponentType m_GetComponentType() { return m_addedType; }
		ecs::EntityID m_GetID() { return m_entityChanged; }
	};
	
	class RemoveComponent : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityChanged;
		ecs::ComponentType m_removedType;
	public:
		RemoveComponent(ecs::EntityID inID, ecs::ComponentType inType) : BaseEvent<Actions>(Actions::REMOVECOMP), m_entityChanged(inID), m_removedType(inType) {}
		ecs::ComponentType m_GetComponentType() { return m_removedType; }
		ecs::EntityID m_GetID() { return m_entityChanged; }
	};

	class AddEntity : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID;
	public:
		AddEntity(ecs::EntityID inID) : BaseEvent<Actions>(Actions::ADDENT), m_entityID(inID) {}
		ecs::EntityID m_GetID() { return m_entityID; }

	};

	class RemoveEntity : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID;
	public:
		RemoveEntity(ecs::EntityID inID) : BaseEvent<Actions>(Actions::DELENT), m_entityID(inID) {}
		ecs::EntityID m_GetID() { return m_entityID; }
	};

	class MoveEntityChildToChild : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID;
		ecs::EntityID m_prevParent;
		ecs::EntityID m_newParent;
	public:
		MoveEntityChildToChild(ecs::EntityID inID, ecs::EntityID inOld, ecs::EntityID inNew) : BaseEvent<Actions>(Actions::MOVECTC), m_entityID(inID),m_prevParent(inOld), m_newParent(inNew) {}
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::EntityID m_GetOldParentID() { return m_prevParent; }
		ecs::EntityID m_GetNewParentID() { return m_newParent; }
	};

	class MoveEntityChildToParent : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID;
		ecs::EntityID m_prevParent;
	public:
		MoveEntityChildToParent(ecs::EntityID inID, ecs::EntityID inOld) : BaseEvent<Actions>(Actions::MOVECTP), m_entityID(inID), m_prevParent(inOld){}
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::EntityID m_GetOldParentID() { return m_prevParent; }
	};

	class MoveEntityParentToChild : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID;
		ecs::EntityID m_newParent;
	public:
		MoveEntityParentToChild(ecs::EntityID inID,  ecs::EntityID inNew) : BaseEvent<Actions>(Actions::MOVEPTC), m_entityID(inID),  m_newParent(inNew) {}
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::EntityID m_GetNewParentID() { return m_newParent; }
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
