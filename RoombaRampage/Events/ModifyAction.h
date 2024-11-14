#pragma once
#include "Action.h"

namespace actions {
	class ModifyAction : public Action{
		ecs::EntityID m_entityID;
		ecs::Component* m_changedComp;
		ecs::Component m_oldVal, m_newVal;

		ModifyAction(ecs::EntityID inID, ecs::Component* inComp, const ecs::Component& inOld, const ecs::Component& inNew) :
			m_entityID(inID), m_changedComp(inComp), m_oldVal(inOld), m_newVal(inNew) {}

		void undo() override {
			*m_changedComp = m_oldVal;
		}

		void redo() override {
			*m_changedComp = m_newVal;
		}
	};

	class AddComponentAction : public Action {
		ecs::EntityID m_entityID;
		ecs::ComponentType m_type;

		AddComponentAction(ecs::EntityID inID, ecs::ComponentType inType) : m_entityID(inID), m_type(inType) {};

		void undo() override {
			ecs::ECS::m_GetInstance()->m_RemoveComponent(m_type, m_entityID);
		}

		void redo() override {
			ecs::ECS::m_GetInstance()->m_AddComponent(m_type, m_entityID);
		}
	};

	class RemoveComponentAction : public Action {
		ecs::EntityID m_entityID;
		ecs::ComponentType m_type;

		RemoveComponentAction(ecs::EntityID inID, ecs::ComponentType inType) : m_entityID(inID), m_type(inType) {};

		void undo() override {
			ecs::ECS::m_GetInstance()->m_AddComponent(m_type, m_entityID);
		}

		void redo() override {
			ecs::ECS::m_GetInstance()->m_RemoveComponent(m_type, m_entityID);
		}
	};
}