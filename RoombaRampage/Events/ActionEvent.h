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
		std::string* m_scene;
		std::string m_entityName;
		ecs::compSignature m_sig;
	public:
		AddEntity(ecs::EntityID inID, std::string* inScene, std::string inName, ecs::compSignature inSig) : BaseEvent<Actions>(Actions::ADDENT), m_entityID(inID), m_scene(inScene), m_entityName(inName), m_sig(inSig) {}
		std::string* m_GetScene() { return m_scene; }
		ecs::EntityID m_GetID() { return m_entityID; }
		std::string m_GetName() { return m_entityName; }
		ecs::compSignature m_GetSignature() { return m_sig; }
	};

	class RemoveEntity : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID;
		std::string* m_scene;
		std::string m_entityName;
		ecs::compSignature m_sig;
	public:
		RemoveEntity(ecs::EntityID inID, std::string* inScene, std::string inName, ecs::compSignature inSig) : BaseEvent<Actions>(Actions::DELENT), m_entityID(inID), m_scene(inScene), m_entityName(inName), m_sig(inSig) {}
		std::string* m_GetScene() { return m_scene; }
		ecs::EntityID m_GetID() { return m_entityID; }
		std::string m_GetName() { return m_entityName; }
		ecs::compSignature m_GetSignature() { return m_sig; }
	};

	class AddChild : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID;
		std::optional<ecs::EntityID> m_parentID;
		std::string* m_scene;
		std::string m_entityName;
		ecs::compSignature m_sig;
	public:
		AddChild(ecs::EntityID inID, std::optional<ecs::EntityID> inParent,std::string* inScene, std::string inName, ecs::compSignature inSig) : BaseEvent<Actions>(Actions::ADDCHILD), m_entityID(inID), m_parentID(inParent), m_scene(inScene), m_entityName(inName), m_sig(inSig) {}
		std::string* m_GetScene() { return m_scene; }
		ecs::EntityID m_GetID() { return m_entityID; }
		std::string m_GetName() { return m_entityName; }
		ecs::EntityID m_GetParentID() { return m_parentID.value(); }
		ecs::compSignature m_GetSignature() { return m_sig; }
	};

	class RemoveChild : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID;
		std::optional<ecs::EntityID> m_parentID;
		std::string* m_scene;
		std::string m_entityName;
		ecs::compSignature m_sig;
	public:
		RemoveChild(ecs::EntityID inID, std::optional<ecs::EntityID> inParent, std::string* inScene, std::string inName, ecs::compSignature inSig) : BaseEvent<Actions>(Actions::REMOVECHILD), m_entityID(inID), m_parentID(inParent), m_scene(inScene), m_entityName(inName), m_sig(inSig) {}
		std::string* m_GetScene() { return m_scene; }
		ecs::EntityID m_GetID() { return m_entityID; }
		std::string m_GetName() { return m_entityName; }
		ecs::EntityID m_GetParentID() { return m_parentID.value(); }
		ecs::compSignature m_GetSignature() { return m_sig; }
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
