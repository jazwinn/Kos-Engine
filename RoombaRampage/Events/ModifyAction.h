#pragma once
#include "Action.h"
#include "../ECS/Hierachy.h"

namespace actions {
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

		void m_undoAction() override {
			m_changedComp->m_position = m_oldPos;
			m_changedComp->m_rotation = m_oldRot;
			m_changedComp->m_scale = m_oldScale;
			m_changedComp->m_transformation = m_oldTrans;

		}

		void m_redoAction() override {
			m_changedComp->m_position = m_newPos;
			m_changedComp->m_rotation = m_newRot;
			m_changedComp->m_scale = m_newScale;
			m_changedComp->m_transformation = m_newTrans;
		}


	};

	class AddComponentAction : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::ComponentType m_type;
	public:

		AddComponentAction(ecs::EntityID inID, ecs::ComponentType inType) : m_entityID(inID), m_type(inType) {};

		void m_undoAction() override {
			ecs::ECS::m_GetInstance()->m_RemoveComponent(m_type, m_entityID);
		}

		void m_redoAction() override {
			ecs::ECS::m_GetInstance()->m_AddComponent(m_type, m_entityID);
		}
	};

	class RemoveComponentAction : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::ComponentType m_type;
	public:
		RemoveComponentAction(ecs::EntityID inID, ecs::ComponentType inType) : m_entityID(inID), m_type(inType) {};

		void m_undoAction() override {
			ecs::ECS::m_GetInstance()->m_AddComponent(m_type, m_entityID);
		}

		void m_redoAction() override {
			ecs::ECS::m_GetInstance()->m_RemoveComponent(m_type, m_entityID);
		}
	};

	class AddEntityAction : public Action {
	private:
		ecs::EntityID m_entityID;
		bool m_hasBeenUndo;
	public:
		AddEntityAction(ecs::EntityID inID) : m_entityID(inID), m_hasBeenUndo(false) {};

		void m_undoAction() override {
			if (!m_hasBeenUndo) {
				ecs::ECS::m_GetInstance()->m_DeleteEntity(m_entityID);
				m_hasBeenUndo = true;
			}
		}

		void m_redoAction() override {
			if (m_hasBeenUndo) {
				ecs::ECS::m_GetInstance()->m_RestoreEntity(m_entityID);
				m_hasBeenUndo = false;
			}
		}
	};

	class RemoveEntityAction : public Action {
	private:
		ecs::EntityID m_entityID;
		bool m_hasBeenUndo;
	public:
		RemoveEntityAction(ecs::EntityID inID) : m_entityID(inID), m_hasBeenUndo(false) {};

		void m_undoAction() override {
			if (!m_hasBeenUndo) {
				m_hasBeenUndo = true;
				ecs::ECS::m_GetInstance()->m_RestoreEntity(m_entityID);
			}
		}

		void m_redoAction() override {
			if (m_hasBeenUndo) {
				m_hasBeenUndo = false;
				ecs::ECS::m_GetInstance()->m_DeleteEntity(m_entityID);
			}
		}
	};

}