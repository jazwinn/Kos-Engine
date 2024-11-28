/********************************************************************/
/*!
\file      ModifyAction.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Nov 12, 2024
\brief     This header file declares and defines all the derived classes that inherit from Action class
		   -ModifyTransformAction
		   -AddComponentAction
		   -RemoveComponentAction
		   -AddEntityAction
		   -RemoveEntityAction

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#include "Action.h"
#include "../ECS/Hierachy.h"

namespace actions {
	
	/******************************************************************/
	/*!
		\brief     This is a class to hold transform component data for later undo or redo
	*/
	/******************************************************************/
	class ModifyTransformAction : public Action{
	private:
		ecs::EntityID m_entityID;
		ecs::TransformComponent* m_changedComp;
		vector2::Vec2 m_oldPos, m_newPos;
		float m_oldRot, m_newRot;
		vector2::Vec2 m_oldScale, m_newScale;
		mat3x3::Mat3x3 m_oldTrans, m_newTrans;
	public:
		ModifyTransformAction(ecs::EntityID inID, ecs::TransformComponent* inComp, vector2::Vec2 inOldPos, float inOldRot, vector2::Vec2 inOldScale, mat3x3::Mat3x3 inOldTrans) :
			m_entityID(inID), m_changedComp(inComp), m_oldPos(inOldPos), m_newPos(inComp->m_position), m_oldRot(inOldRot), m_newRot(inComp->m_rotation), 
			m_oldScale(inOldScale), m_newScale(inComp->m_scale), m_oldTrans(inOldTrans), m_newTrans(inComp->m_transformation){}

		void m_UndoAction() override {
			m_changedComp->m_position = m_oldPos;
			m_changedComp->m_rotation = m_oldRot;
			m_changedComp->m_scale = m_oldScale;
			m_changedComp->m_transformation = m_oldTrans;

		}

		void m_RedoAction() override {
			m_changedComp->m_position = m_newPos;
			m_changedComp->m_rotation = m_newRot;
			m_changedComp->m_scale = m_newScale;
			m_changedComp->m_transformation = m_newTrans;
		}


	};

	/******************************************************************/
	/*!
		\brief     This is a class to hold which component has been added to which entity
	*/
	/******************************************************************/
	class AddComponentAction : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::ComponentType m_type;
	public:

		AddComponentAction(ecs::EntityID inID, ecs::ComponentType inType) : m_entityID(inID), m_type(inType) {};

		void m_UndoAction() override {
			ecs::ECS::m_GetInstance()->m_RemoveComponent(m_type, m_entityID);
		}

		void m_RedoAction() override {
			ecs::ECS::m_GetInstance()->m_AddComponent(m_type, m_entityID);
		}
	};

	/******************************************************************/
	/*!
		\brief     This is a class to hold which component has been removed from which entity
	*/
	/******************************************************************/
	class RemoveComponentAction : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::ComponentType m_type;
	public:
		RemoveComponentAction(ecs::EntityID inID, ecs::ComponentType inType) : m_entityID(inID), m_type(inType) {};

		void m_UndoAction() override {
			ecs::ECS::m_GetInstance()->m_AddComponent(m_type, m_entityID);
		}

		void m_RedoAction() override {
			ecs::ECS::m_GetInstance()->m_RemoveComponent(m_type, m_entityID);
		}
	};

	/******************************************************************/
	/*!
		\brief     This is a class to hold which entity has been added
	*/
	/******************************************************************/
	class AddEntityAction : public Action {
	private:
		ecs::EntityID m_entityID;
		bool m_hasBeenUndo;
	public:
		AddEntityAction(ecs::EntityID inID) : m_entityID(inID), m_hasBeenUndo(false) {};

		void m_UndoAction() override {
			if (!m_hasBeenUndo) {
				ecs::ECS::m_GetInstance()->m_DeleteEntity(m_entityID);
				m_hasBeenUndo = true;
			}
		}

		void m_RedoAction() override {
			if (m_hasBeenUndo) {
				ecs::ECS::m_GetInstance()->m_RestoreEntity(m_entityID);
				m_hasBeenUndo = false;
			}
		}
	};

	/******************************************************************/
	/*!
		\brief     This is a class to hold which entity has been deleted
	*/
	/******************************************************************/
	class RemoveEntityAction : public Action {
	private:
		ecs::EntityID m_entityID;
		bool m_hasBeenUndo;
	public:
		RemoveEntityAction(ecs::EntityID inID) : m_entityID(inID), m_hasBeenUndo(false) {};

		void m_UndoAction() override {
			if (!m_hasBeenUndo) {
				m_hasBeenUndo = true;
				ecs::ECS::m_GetInstance()->m_RestoreEntity(m_entityID);
			}
		}

		void m_RedoAction() override {
			if (m_hasBeenUndo) {
				m_hasBeenUndo = false;
				ecs::ECS::m_GetInstance()->m_DeleteEntity(m_entityID);
			}
		}
	};

	class MoveEntityChildToChildAction : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::EntityID m_prevParent;
		ecs::EntityID m_newParent;
		bool m_hasBeenUndo;
	public:
		MoveEntityChildToChildAction(ecs::EntityID inID, ecs::EntityID inOld, ecs::EntityID inNew) : m_entityID(inID), m_prevParent(inOld), m_newParent(inNew), m_hasBeenUndo(false) {};

		void m_UndoAction() override {
			if (!m_hasBeenUndo) {
				m_hasBeenUndo = true;
				ecs::Hierachy::m_SetParent(m_prevParent, m_entityID);
			}
		}

		void m_RedoAction() override {
			if (m_hasBeenUndo) {
				m_hasBeenUndo = false;
				ecs::Hierachy::m_SetParent(m_newParent, m_entityID);
			}
		}
	};

	class MoveEntityChildToParentAction : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::EntityID m_prevParent;
		bool m_hasBeenUndo;
	public:
		MoveEntityChildToParentAction(ecs::EntityID inID, ecs::EntityID inOld) : m_entityID(inID), m_prevParent(inOld), m_hasBeenUndo(false) {};

		void m_UndoAction() override {
			if (!m_hasBeenUndo) {
				m_hasBeenUndo = true;
				ecs::Hierachy::m_SetParent(m_prevParent, m_entityID);
			}
		}

		void m_RedoAction() override {
			if (m_hasBeenUndo) {
				m_hasBeenUndo = false;
				ecs::Hierachy::m_RemoveParent(m_entityID);
			}
		}
	};

	class MoveEntityParentToChildAction : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::EntityID m_newParent;
		bool m_hasBeenUndo;
	public:
		MoveEntityParentToChildAction(ecs::EntityID inID,  ecs::EntityID inNew) : m_entityID(inID), m_newParent(inNew), m_hasBeenUndo(false) {};

		void m_UndoAction() override {
			if (!m_hasBeenUndo) {
				m_hasBeenUndo = true;
				ecs::Hierachy::m_RemoveParent(m_entityID);
			}
		}

		void m_RedoAction() override {
			if (m_hasBeenUndo) {
				m_hasBeenUndo = false;
				ecs::Hierachy::m_SetParent(m_newParent, m_entityID);
			}
		}
	};

}