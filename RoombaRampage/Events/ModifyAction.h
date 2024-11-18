#pragma once
#include "Action.h"

namespace actions {
	class ModifyTransformAction : public Action{
	private:
		ecs::EntityID m_entityID;
		ecs::TransformComponent* m_changedComp;
		ecs::TransformComponent m_oldVal, m_newVal;
	public:
		ModifyTransformAction(ecs::EntityID inID, ecs::TransformComponent* inComp, const ecs::TransformComponent inOld, const ecs::TransformComponent inNew) :
			m_entityID(inID), m_changedComp(inComp), m_oldVal(inOld), m_newVal(inNew) {}

		void m_undoAction() override {
			*m_changedComp = m_oldVal;
		}

		void m_redoAction() override {
			*m_changedComp = m_newVal;
		}


	};

	class AddComponentAction : public Action {
		ecs::EntityID m_entityID;
		ecs::ComponentType m_type;

		AddComponentAction(ecs::EntityID inID, ecs::ComponentType inType) : m_entityID(inID), m_type(inType) {};

		void m_undoAction() override {
			ecs::ECS::m_GetInstance()->m_RemoveComponent(m_type, m_entityID);
		}

		void m_redoAction() override {
			ecs::ECS::m_GetInstance()->m_AddComponent(m_type, m_entityID);
		}
	};

	class RemoveComponentAction : public Action {
		ecs::EntityID m_entityID;
		ecs::ComponentType m_type;

		RemoveComponentAction(ecs::EntityID inID, ecs::ComponentType inType) : m_entityID(inID), m_type(inType) {};

		void m_undoAction() override {
			ecs::ECS::m_GetInstance()->m_AddComponent(m_type, m_entityID);
		}

		void m_redoAction() override {
			ecs::ECS::m_GetInstance()->m_RemoveComponent(m_type, m_entityID);
		}
	};
}