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

	/******************************************************************/
	/*!
	\class     TransformComponentChanged
	\brief     A derived class from BaseEvent class that holds which entityID, a pointer to the transform component, all the previous member variables in the transform component
	*/
	/******************************************************************/
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
		// Constructor for TransformComponentChanged
		/******************************************************************/
		/*!
		\fn        TransformComponentChanged::TransformComponentChanged(ecs::ComponentType inType, ecs::EntityID inID, ecs::TransformComponent* inComp, ecs::TransformComponent inOld)
		\brief     Constructor for creating a transform component changed event
		\param[in] inType	Type of component that was changed
		\param[in] inID		Entitys' ID
		\param[in] inComp	Pointer to the Entitys' transform component
		\param[in] inOld	Copy of previous transform component
		*/
		/******************************************************************/
		TransformComponentChanged(ecs::ComponentType inType, ecs::EntityID inID, ecs::TransformComponent* inComp, ecs::TransformComponent inOld) : BaseEvent<Actions>(Actions::TRANSFORMCOMP),
			m_changedType(inType), m_entityChanged(inID), m_changedComponent(inComp),m_oldPos(inOld.m_position), m_oldRot(inOld.m_rotation), m_oldScale(inOld.m_scale), m_oldTrans(inOld.m_transformation){}

		/******************************************************************/
		/*!
		\fn        TransformComponentChanged::m_GetComponentType()
		\brief     Getter for the changed component type
		\return	   Component type of the entity
		*/
		/******************************************************************/
		ecs::ComponentType m_GetComponentType() { return m_changedType; }
		/******************************************************************/
		/*!
		\fn        TransformComponentChanged::m_GetID()
		\brief     Getter for the entitys' ID
		\return	   Entitys' ID
		*/
		/******************************************************************/
		ecs::EntityID m_GetID() { return m_entityChanged; }
		/******************************************************************/
		/*!
		\fn        TransformComponentChanged::m_GetComp()
		\brief     Getter for the pointer to the transform component
		\return	   Pointer to the entity's transform component
		*/
		/******************************************************************/
		ecs::TransformComponent* m_GetComp() { return m_changedComponent; }
		/******************************************************************/
		/*!
		\fn        TransformComponentChanged::m_GetOldPos()
		\brief     Getter for the previous position
		\return	   Previous position of entity
		*/
		/******************************************************************/
		vector2::Vec2 m_GetOldPos() { return m_oldPos; }
		/******************************************************************/
		/*!
		\fn        TransformComponentChanged::m_GetOldScale()
		\brief     Getter for the previous scale
		\return	   Previous scale of entity
		*/
		/******************************************************************/
		vector2::Vec2 m_GetOldScale() { return m_oldScale; }
		/******************************************************************/
		/*!
		\fn        TransformComponentChanged::m_GetOldRot()
		\brief     Getter for the previous rotation
		\return	   Previous rotation of entity
		*/
		/******************************************************************/
		float m_GetOldRot() { return m_oldRot; }
		/******************************************************************/
		/*!
		\fn        TransformComponentChanged::m_GetOldTrans()
		\brief     Getter for the previous transform matrix
		\return    Previous computed Transform matrix
		*/
		/******************************************************************/
		mat3x3::Mat3x3 m_GetOldTrans() { return m_oldTrans; }

	};

	/******************************************************************/
	/*!
	\class     AddComponent
	\brief     A derived class from BaseEvent class that represents adding a component to an entity.
	*/
	/******************************************************************/
	class AddComponent : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityChanged; /*!< The ID of the entity to which the component is added. */
		ecs::ComponentType m_addedType; /*!< The type of component being added. */
	public:
		/******************************************************************/
		/*!
		\fn        AddComponent::AddComponent(ecs::EntityID inID, ecs::ComponentType inType)
		\brief     Constructor for creating an add component event.
		\param[in] inID      The ID of the entity.
		\param[in] inType    The type of the component to be added.
		*/
		/******************************************************************/
		AddComponent(ecs::EntityID inID, ecs::ComponentType inType) : BaseEvent<Actions>(Actions::ADDCOMP), m_entityChanged(inID), m_addedType(inType) {}

		/******************************************************************/
		/*!
		\fn        AddComponent::m_GetComponentType()
		\brief     Getter for the type of the component added.
		\return    The type of the added component.
		*/
		/******************************************************************/
		ecs::ComponentType m_GetComponentType() { return m_addedType; }

		/******************************************************************/
		/*!
		\fn        AddComponent::m_GetID()
		\brief     Getter for the entity's ID.
		\return    The ID of the entity to which the component is added.
		*/
		/******************************************************************/
		ecs::EntityID m_GetID() { return m_entityChanged; }
	};

	/******************************************************************/
	/*!
	\class     RemoveComponent
	\brief     A derived class from BaseEvent class that represents removing a component from an entity.
	*/
	/******************************************************************/
	class RemoveComponent : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityChanged; /*!< The ID of the entity from which the component is removed. */
		ecs::ComponentType m_removedType; /*!< The type of component being removed. */
	public:
		/******************************************************************/
		/*!
		\fn        RemoveComponent::RemoveComponent(ecs::EntityID inID, ecs::ComponentType inType)
		\brief     Constructor for creating a remove component event.
		\param[in] inID      The ID of the entity.
		\param[in] inType    The type of the component to be removed.
		*/
		/******************************************************************/
		RemoveComponent(ecs::EntityID inID, ecs::ComponentType inType) : BaseEvent<Actions>(Actions::REMOVECOMP), m_entityChanged(inID), m_removedType(inType) {}

		/******************************************************************/
		/*!
		\fn        RemoveComponent::m_GetComponentType()
		\brief     Getter for the type of the component removed.
		\return    The type of the removed component.
		*/
		/******************************************************************/
		ecs::ComponentType m_GetComponentType() { return m_removedType; }

		/******************************************************************/
		/*!
		\fn        RemoveComponent::m_GetID()
		\brief     Getter for the entity's ID.
		\return    The ID of the entity from which the component is removed.
		*/
		/******************************************************************/
		ecs::EntityID m_GetID() { return m_entityChanged; }
	};

	/******************************************************************/
	/*!
	\class     AddEntity
	\brief     A derived class from BaseEvent class that represents adding a new entity.
	*/
	/******************************************************************/
	class AddEntity : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID; /*!< The ID of the newly added entity. */
	public:
		/******************************************************************/
		/*!
		\fn        AddEntity::AddEntity(ecs::EntityID inID)
		\brief     Constructor for creating an add entity event.
		\param[in] inID      The ID of the newly added entity.
		*/
		/******************************************************************/
		AddEntity(ecs::EntityID inID) : BaseEvent<Actions>(Actions::ADDENT), m_entityID(inID) {}

		/******************************************************************/
		/*!
		\fn        AddEntity::m_GetID()
		\brief     Getter for the ID of the added entity.
		\return    The ID of the newly added entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetID() { return m_entityID; }
	};

	/******************************************************************/
	/*!
	\class     RemoveEntity
	\brief     A derived class from BaseEvent class that represents removing an entity.
	*/
	/******************************************************************/
	class RemoveEntity : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID; /*!< The ID of the entity to be removed. */
	public:
		/******************************************************************/
		/*!
		\fn        RemoveEntity::RemoveEntity(ecs::EntityID inID)
		\brief     Constructor for creating a remove entity event.
		\param[in] inID      The ID of the entity to be removed.
		*/
		/******************************************************************/
		RemoveEntity(ecs::EntityID inID) : BaseEvent<Actions>(Actions::DELENT), m_entityID(inID) {}

		/******************************************************************/
		/*!
		\fn        RemoveEntity::m_GetID()
		\brief     Getter for the ID of the removed entity.
		\return    The ID of the removed entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetID() { return m_entityID; }
	};

	/******************************************************************/
	/*!
	\class     MoveEntityChildToChild
	\brief     A derived class from BaseEvent class that represents moving an entity from one parent entity to another parent entity.
	*/
	/******************************************************************/
	class MoveEntityChildToChild : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID; /*!< The ID of the entity being moved. */
		ecs::EntityID m_prevParent; /*!< The ID of the previous parent entity. */
		ecs::EntityID m_newParent; /*!< The ID of the new parent entity. */
	public:
		/******************************************************************/
		/*!
		\fn        MoveEntityChildToChild::MoveEntityChildToChild(ecs::EntityID inID, ecs::EntityID inOld, ecs::EntityID inNew)
		\brief     Constructor for creating a move entity child-to-child event.
		\param[in] inID       The ID of the entity being moved.
		\param[in] inOld      The ID of the previous parent entity.
		\param[in] inNew      The ID of the new parent entity.
		*/
		/******************************************************************/
		MoveEntityChildToChild(ecs::EntityID inID, ecs::EntityID inOld, ecs::EntityID inNew) : BaseEvent<Actions>(Actions::MOVECTC), m_entityID(inID), m_prevParent(inOld), m_newParent(inNew) {}

		/******************************************************************/
		/*!
		\fn        MoveEntityChildToChild::m_GetID()
		\brief     Getter for the ID of the moved entity.
		\return    The ID of the moved entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetID() { return m_entityID; }

		/******************************************************************/
		/*!
		\fn        MoveEntityChildToChild::m_GetOldParentID()
		\brief     Getter for the ID of the previous parent entity.
		\return    The ID of the previous parent entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetOldParentID() { return m_prevParent; }

		/******************************************************************/
		/*!
		\fn        MoveEntityChildToChild::m_GetNewParentID()
		\brief     Getter for the ID of the new parent entity.
		\return    The ID of the new parent entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetNewParentID() { return m_newParent; }
	};

	/******************************************************************/
	/*!
	\class     MoveEntityChildToParent
	\brief     A derived class from BaseEvent class that represents moving an entity from a child-parent relationship to a standalone parent entity.
	*/
	/******************************************************************/
	class MoveEntityChildToParent : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID; /*!< The ID of the entity being moved. */
		ecs::EntityID m_prevParent; /*!< The ID of the previous parent entity. */
	public:
		/******************************************************************/
		/*!
		\fn        MoveEntityChildToParent::MoveEntityChildToParent(ecs::EntityID inID, ecs::EntityID inOld)
		\brief     Constructor for creating a move entity child-to-parent event.
		\param[in] inID       The ID of the entity being moved.
		\param[in] inOld      The ID of the previous parent entity.
		*/
		/******************************************************************/
		MoveEntityChildToParent(ecs::EntityID inID, ecs::EntityID inOld) : BaseEvent<Actions>(Actions::MOVECTP), m_entityID(inID), m_prevParent(inOld) {}

		/******************************************************************/
		/*!
		\fn        MoveEntityChildToParent::m_GetID()
		\brief     Getter for the ID of the moved entity.
		\return    The ID of the moved entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetID() { return m_entityID; }

		/******************************************************************/
		/*!
		\fn        MoveEntityChildToParent::m_GetOldParentID()
		\brief     Getter for the ID of the previous parent entity.
		\return    The ID of the previous parent entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetOldParentID() { return m_prevParent; }
	};


	/******************************************************************/
	/*!
	\class     MoveEntityParentToChild
	\brief     A derived class from BaseEvent class that represents moving a standalone parent entity to become a child entity of another parent.
	*/
	/******************************************************************/
	class MoveEntityParentToChild : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID; /*!< The ID of the entity being moved. */
		ecs::EntityID m_newParent; /*!< The ID of the new parent entity. */
	public:
		/******************************************************************/
		/*!
		\fn        MoveEntityParentToChild::MoveEntityParentToChild(ecs::EntityID inID, ecs::EntityID inNew)
		\brief     Constructor for creating a move entity parent-to-child event.
		\param[in] inID       The ID of the entity being moved.
		\param[in] inNew      The ID of the new parent entity.
		*/
		/******************************************************************/
		MoveEntityParentToChild(ecs::EntityID inID, ecs::EntityID inNew) : BaseEvent<Actions>(Actions::MOVEPTC), m_entityID(inID), m_newParent(inNew) {}

		/******************************************************************/
		/*!
		\fn        MoveEntityParentToChild::m_GetID()
		\brief     Getter for the ID of the moved entity.
		\return    The ID of the moved entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetID() { return m_entityID; }

		/******************************************************************/
		/*!
		\fn        MoveEntityParentToChild::m_GetNewParentID()
		\brief     Getter for the ID of the new parent entity.
		\return    The ID of the new parent entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetNewParentID() { return m_newParent; }
	};

	/******************************************************************/
	/*!
	\class     UndoLatest
	\brief     A derived class from BaseEvent class that represents an undo action for the most recent change.
	*/
	/******************************************************************/
	class UndoLatest : public BaseEvent<Actions> {
	public:
		/******************************************************************/
		/*!
		\fn        UndoLatest::UndoLatest()
		\brief     Constructor for creating an undo action event.
		*/
		/******************************************************************/
		UndoLatest() : BaseEvent<Actions>(Actions::UNDO) {}
	};


	/******************************************************************/
	/*!
	\class     RedoPrevious
	\brief     A derived class from BaseEvent class that represents a redo action for the last undone change.
	*/
	/******************************************************************/
	class RedoPrevious : public BaseEvent<Actions> {
	public:
		/******************************************************************/
		/*!
		\fn        RedoPrevious::RedoPrevious()
		\brief     Constructor for creating a redo action event.
		*/
		/******************************************************************/
		RedoPrevious() : BaseEvent<Actions>(Actions::REDO) {}
	};


}
