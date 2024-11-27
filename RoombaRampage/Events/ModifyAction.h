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
		ecs::EntityID m_newEntityID;
		std::string* m_scene;
		std::string m_entName;
		ecs::compSignature m_comps;
		bool m_hasBeenRedo;
		bool m_hasBeenUndo;
		vector2::Vec2 m_oldPos;
		float m_oldRot;
		vector2::Vec2 m_oldScale;
		mat3x3::Mat3x3 m_oldTrans;
	public:
		AddEntityAction(ecs::EntityID inID, ecs::compSignature inComps, std::string* inScene, const std::string& inName) : m_entityID(inID),m_newEntityID(),m_scene(inScene), m_entName(inName), m_comps(inComps), m_hasBeenRedo(false), m_hasBeenUndo(false) {};

		void m_undoAction() override {
			if (!m_hasBeenUndo) {
				if (!m_hasBeenRedo) {
					ecs::TransformComponent* tComp = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(m_newEntityID));
					m_oldPos = tComp->m_position;
					m_oldRot = tComp->m_rotation;
					m_oldScale = tComp->m_scale;
					m_oldTrans = tComp->m_transformation;
					ecs::ECS::m_GetInstance()->m_DeleteEntity(m_entityID);
				}
				else {
					ecs::TransformComponent* tComp = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(m_newEntityID));
					m_oldPos = tComp->m_position;
					m_oldRot = tComp->m_rotation;
					m_oldScale = tComp->m_scale;
					m_oldTrans = tComp->m_transformation;
					ecs::ECS::m_GetInstance()->m_DeleteEntity(m_newEntityID);
				}
				m_hasBeenUndo = true;
			}
		}

		void m_redoAction() override {
			if (m_hasBeenUndo) {
				m_newEntityID = ecs::ECS::m_GetInstance()->m_CreateEntity(*m_scene);
				static_cast<ecs::NameComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(m_newEntityID))->m_entityName = std::string(m_entName);
				if (m_comps.test(ecs::TYPECOLLIDERCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPESPRITECOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPESPRITECOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPEPLAYERCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPEPLAYERCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPERIGIDBODYCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPERIGIDBODYCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPETEXTCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPETEXTCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPEANIMATIONCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPECAMERACOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPECAMERACOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPEBUTTONCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPEBUTTONCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPESCRIPTCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPESCRIPTCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPETILEMAPCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPETILEMAPCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPEAUDIOCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPEAUDIOCOMPONENT, m_newEntityID);
				}
				ecs::TransformComponent* tComp = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(m_newEntityID));
				tComp->m_position = m_oldPos;
				tComp->m_rotation = m_oldRot;
				tComp->m_scale = m_oldScale;
				tComp->m_transformation = m_oldTrans;
				m_hasBeenUndo = false;
				m_hasBeenRedo = true;
			}
		}
	};

	class RemoveEntityAction : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::EntityID m_newEntityID;
		std::string* m_scene;
		std::string m_entName;
		ecs::compSignature m_comps;
		bool m_hasBeenUndo;
		vector2::Vec2 m_oldPos;
		float m_oldRot;
		vector2::Vec2 m_oldScale;
		mat3x3::Mat3x3 m_oldTrans;
	public:
		RemoveEntityAction(ecs::EntityID inID, ecs::compSignature inComps, std::string* inScene, const std::string& inName) : m_entityID(inID), m_newEntityID(),m_scene(inScene), m_entName(inName), m_comps(inComps), m_hasBeenUndo(false) {};

		void m_undoAction() override {
			if (!m_hasBeenUndo) {
				m_hasBeenUndo = true;
				m_newEntityID = ecs::ECS::m_GetInstance()->m_CreateEntity(*m_scene);
				static_cast<ecs::NameComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(m_newEntityID))->m_entityName = std::string(m_entName);
				if (m_comps.test(ecs::TYPECOLLIDERCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPESPRITECOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPESPRITECOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPEPLAYERCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPEPLAYERCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPERIGIDBODYCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPERIGIDBODYCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPETEXTCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPETEXTCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPEANIMATIONCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPECAMERACOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPECAMERACOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPEBUTTONCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPEBUTTONCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPESCRIPTCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPESCRIPTCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPETILEMAPCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPETILEMAPCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPEAUDIOCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPEAUDIOCOMPONENT, m_newEntityID);
				}
				ecs::TransformComponent* tComp = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(m_newEntityID));
				tComp->m_position = m_oldPos;
				tComp->m_rotation = m_oldRot;
				tComp->m_scale = m_oldScale;
				tComp->m_transformation = m_oldTrans;
			}
		}

		void m_redoAction() override {
			if (m_hasBeenUndo) {
				m_hasBeenUndo = false;
				ecs::TransformComponent* tComp = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(m_newEntityID));
				m_oldPos = tComp->m_position;
				m_oldRot = tComp->m_rotation;
				m_oldScale = tComp->m_scale;
				m_oldTrans = tComp->m_transformation;
				ecs::ECS::m_GetInstance()->m_DeleteEntity(m_newEntityID);
			}
		}
	};

	//only used for entity with no children but with parent
	class RemoveEntityWithParentAction : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::EntityID m_newEntityID;
		ecs::EntityID m_parentID;
		std::string* m_scene;
		std::string m_entName;
		bool m_hasBeenUndo;
		ecs::compSignature m_comps;
		vector2::Vec2 m_oldPos;
		float m_oldRot;
		vector2::Vec2 m_oldScale;
		mat3x3::Mat3x3 m_oldTrans;
	public:
		RemoveEntityWithParentAction(ecs::EntityID inID, ecs::EntityID inParent ,std::string* inScene, const std::string& inName, ecs::compSignature inSig) : m_entityID(inID), m_newEntityID(), m_parentID(inParent), m_scene(inScene), m_entName(inName),m_hasBeenUndo(false), m_comps(inSig) {};

		void m_undoAction() override {
			if (!m_hasBeenUndo) {
				m_newEntityID = ecs::ECS::m_GetInstance()->m_CreateEntity(*m_scene);
				static_cast<ecs::NameComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(m_newEntityID))->m_entityName = std::string(m_entName);
				ecs::Hierachy::m_SetParent(m_parentID, m_newEntityID);
				m_hasBeenUndo = true;
				if (m_comps.test(ecs::TYPECOLLIDERCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPESPRITECOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPESPRITECOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPEPLAYERCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPEPLAYERCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPERIGIDBODYCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPERIGIDBODYCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPETEXTCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPETEXTCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPEANIMATIONCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPECAMERACOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPECAMERACOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPEBUTTONCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPEBUTTONCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPESCRIPTCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPESCRIPTCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPETILEMAPCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPETILEMAPCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPEAUDIOCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPEAUDIOCOMPONENT, m_newEntityID);
				}
				ecs::TransformComponent* tComp = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(m_newEntityID));
				tComp->m_position = m_oldPos;
				tComp->m_rotation = m_oldRot;
				tComp->m_scale = m_oldScale;
				tComp->m_transformation = m_oldTrans;
			}

		}

		void m_redoAction() override {
			if (m_hasBeenUndo) {
				m_hasBeenUndo = false;
				ecs::TransformComponent* tComp = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(m_newEntityID));
				m_oldPos = tComp->m_position;
				m_oldRot = tComp->m_rotation;
				m_oldScale = tComp->m_scale;
				m_oldTrans = tComp->m_transformation;
				ecs::ECS::m_GetInstance()->m_DeleteEntity(m_newEntityID);
			}
		}
	};

	//only used for entity with no children but with parent
	class AddEntityWithParentAction : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::EntityID m_newEntityID;
		ecs::EntityID m_parentID;
		std::string* m_scene;
		std::string m_entName;
		bool m_hasBeenUndo;
		bool m_hasBeenRedo;
		ecs::compSignature m_comps;
		vector2::Vec2 m_oldPos;
		float m_oldRot;
		vector2::Vec2 m_oldScale;
		mat3x3::Mat3x3 m_oldTrans;
	public:
		AddEntityWithParentAction(ecs::EntityID inID, ecs::EntityID inParent,std::string* inScene, const std::string& inName, ecs::compSignature inSig) : m_entityID(inID), m_newEntityID(), m_parentID(inParent), m_scene(inScene), m_entName(inName), m_hasBeenUndo(false), m_hasBeenRedo(false),m_comps(inSig) {};

		void m_undoAction() override {
			if (!m_hasBeenUndo) {
				if (!m_hasBeenRedo) {
					ecs::TransformComponent* tComp = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(m_newEntityID));
					m_oldPos = tComp->m_position;
					m_oldRot = tComp->m_rotation;
					m_oldScale = tComp->m_scale;
					m_oldTrans = tComp->m_transformation;
					ecs::ECS::m_GetInstance()->m_DeleteEntity(m_entityID);
				}
				else {
					ecs::TransformComponent* tComp = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(m_newEntityID));
					m_oldPos = tComp->m_position;
					m_oldRot = tComp->m_rotation;
					m_oldScale = tComp->m_scale;
					m_oldTrans = tComp->m_transformation;
					ecs::ECS::m_GetInstance()->m_DeleteEntity(m_newEntityID);
				}
				m_hasBeenUndo = true;
			}
		}

		void m_redoAction() override {
			if (m_hasBeenUndo) {
				m_newEntityID = ecs::ECS::m_GetInstance()->m_CreateEntity(*m_scene);
				static_cast<ecs::NameComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(m_newEntityID))->m_entityName = std::string(m_entName);
				ecs::Hierachy::m_SetParent(m_parentID, m_newEntityID);
				if (m_comps.test(ecs::TYPECOLLIDERCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPESPRITECOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPESPRITECOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPEPLAYERCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPEPLAYERCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPERIGIDBODYCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPERIGIDBODYCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPETEXTCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPETEXTCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPEANIMATIONCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPECAMERACOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPECAMERACOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPEBUTTONCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPEBUTTONCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPESCRIPTCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPESCRIPTCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPETILEMAPCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPETILEMAPCOMPONENT, m_newEntityID);
				}
				if (m_comps.test(ecs::TYPEAUDIOCOMPONENT)) {
					ecs::ECS::m_GetInstance()->m_AddComponent(ecs::TYPEAUDIOCOMPONENT, m_newEntityID);
				}
				ecs::TransformComponent* tComp = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(m_newEntityID));
				tComp->m_position = m_oldPos;
				tComp->m_rotation = m_oldRot;
				tComp->m_scale = m_oldScale;
				tComp->m_transformation = m_oldTrans;
				m_hasBeenUndo = false;
				m_hasBeenRedo = true;
			}
		}
	};

}