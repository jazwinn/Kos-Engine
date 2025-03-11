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

	class ModifyAnim : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::AnimationComponent* m_changedComp;
		int m_oldFrameNum;
		int m_oldFPS;
		float m_oldFrameTimer;
		bool m_oldIsAnim;
		int m_oldStrip;
	public:
		ModifyAnim(ecs::ComponentType inType, ecs::EntityID inID, ecs::AnimationComponent* inComp, ecs::AnimationComponent inOld) : BaseEvent<Actions>(Actions::MODIFYANIM),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp), m_oldFrameNum(inOld.m_frameNumber), m_oldFPS(inOld.m_framesPerSecond),
			m_oldFrameTimer(inOld.m_frameTimer), m_oldIsAnim(inOld.m_isAnimating),m_oldStrip(inOld.m_stripCount) {}
		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::AnimationComponent* m_GetComp() { return m_changedComp; }
		int m_GetOldFrameNum() { return m_oldFrameNum; }
		int m_GetOldFPS() { return m_oldFPS; }
		float m_GetOldFT() { return m_oldFrameTimer; }
		bool m_GetOldIsAnim() { return m_oldIsAnim; }
		int m_GetOldStrip() { return m_oldStrip; }
	};

	class ModifyCamera : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::CameraComponent* m_changedComp;
		float m_oldLeft, m_oldRight, m_oldTop, m_oldBottom, m_oldAspectRatio;

	public:
		ModifyCamera(ecs::ComponentType inType, ecs::EntityID inID, ecs::CameraComponent* inComp, ecs::CameraComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYCAMERA),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldLeft(inOld.m_left), m_oldRight(inOld.m_right),
			m_oldTop(inOld.m_top), m_oldBottom(inOld.m_bottom),
			m_oldAspectRatio(inOld.m_aspectRatio) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::CameraComponent* m_GetComp() { return m_changedComp; }
		float m_GetOldLeft() { return m_oldLeft; }
		float m_GetOldRight() { return m_oldRight; }
		float m_GetOldTop() { return m_oldTop; }
		float m_GetOldBottom() { return m_oldBottom; }
		float m_GetOldAspectRatio() { return m_oldAspectRatio; }
	};

	class ModifyCollider : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::ColliderComponent* m_changedComp;
		vector2::Vec2 m_oldSize;
		bool m_oldDrawDebug, m_oldCollisionResponse, m_oldCollisionCheck;
		float m_oldRadius;
		physicspipe::EntityType m_oldShape;

	public:
		ModifyCollider(ecs::ComponentType inType, ecs::EntityID inID, ecs::ColliderComponent* inComp, ecs::ColliderComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYCOLL),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldSize(inOld.m_Size), m_oldDrawDebug(inOld.m_drawDebug),
			m_oldRadius(inOld.m_radius), m_oldCollisionResponse(inOld.m_collisionResponse),
			m_oldCollisionCheck(inOld.m_collisionCheck), m_oldShape(inOld.m_type) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::ColliderComponent* m_GetComp() { return m_changedComp; }
		vector2::Vec2 m_GetOldSize() { return m_oldSize; }
		bool m_GetOldDrawDebug() { return m_oldDrawDebug; }
		float m_GetOldRadius() { return m_oldRadius; }
		bool m_GetOldCollisionResponse() { return m_oldCollisionResponse; }
		bool m_GetOldCollisionCheck() { return m_oldCollisionCheck; }
		physicspipe::EntityType m_GetOldShape() { return m_oldShape; }
	};

	class ModifyEnemy : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::EnemyComponent* m_changedComp;
		int m_oldTag, m_oldType, m_oldBehavior;

	public:
		ModifyEnemy(ecs::ComponentType inType, ecs::EntityID inID, ecs::EnemyComponent* inComp, ecs::EnemyComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYENEMY),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldTag(inOld.m_enemyTag), m_oldType(inOld.m_enemyTypeInt),
			m_oldBehavior(inOld.m_enemyRoamBehaviourInt) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::EnemyComponent* m_GetComp() { return m_changedComp; }
		int m_GetOldTag() { return m_oldTag; }
		int m_GetOldType() { return m_oldType; }
		int m_GetOldBehavior() { return m_oldBehavior; }
	};

	class ModifyGrid : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::GridComponent* m_changedComp;
		vector2::Vec2 m_oldAnchor;
		int m_oldRowLength, m_oldColumnLength, m_oldGridKey;
		bool m_oldCollidable;

	public:
		ModifyGrid(ecs::ComponentType inType, ecs::EntityID inID, ecs::GridComponent* inComp, ecs::GridComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYGRID),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldAnchor(inOld.m_Anchor), m_oldRowLength(inOld.m_GridRowLength),
			m_oldColumnLength(inOld.m_GridColumnLength), m_oldCollidable(inOld.m_SetCollidable),
			m_oldGridKey(inOld.m_GridKey) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::GridComponent* m_GetComp() { return m_changedComp; }
		vector2::Vec2 m_GetOldAnchor() { return m_oldAnchor; }
		int m_GetOldRowLength() { return m_oldRowLength; }
		int m_GetOldColumnLength() { return m_oldColumnLength; }
		bool m_GetOldCollidable() { return m_oldCollidable; }
		int m_GetOldGridKey() { return m_oldGridKey; }
	};

	class ModifyLight : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::LightingComponent* m_changedComp;
		graphicpipe::LightType m_oldType;
		float m_oldIntensity;
		vector2::Vec2 m_oldInnerOuterRadius;
		vector3::Vec3 m_oldColor;

	public:
		ModifyLight(ecs::ComponentType inType, ecs::EntityID inID, ecs::LightingComponent* inComp, ecs::LightingComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYLIGHT),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldType(inOld.m_lightType), m_oldIntensity(inOld.m_intensity),
			m_oldInnerOuterRadius(inOld.m_innerOuterRadius), m_oldColor(inOld.m_colour) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::LightingComponent* m_GetComp() { return m_changedComp; }
		graphicpipe::LightType m_GetOldType() { return m_oldType; }
		float m_GetOldIntensity() { return m_oldIntensity; }
		vector2::Vec2 m_GetOldInnerOuterRadius() { return m_oldInnerOuterRadius; }
		vector3::Vec3 m_GetOldColor() { return m_oldColor; }
	};

	class ModifyPathfinding : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::PathfindingComponent* m_changedComp;
		vector2::Vec2 m_oldStartPos, m_oldEndPos;
		int m_oldGridKey;

	public:
		ModifyPathfinding(ecs::ComponentType inType, ecs::EntityID inID, ecs::PathfindingComponent* inComp, ecs::PathfindingComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYPATHFINDING),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldStartPos(inOld.m_StartPos), m_oldEndPos(inOld.m_TargetPos),
			m_oldGridKey(inOld.m_GridKey) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::PathfindingComponent* m_GetComp() { return m_changedComp; }
		ecs::EntityID m_GetID() { return m_entityID; }
		vector2::Vec2 m_GetOldStartPos() { return m_oldStartPos; }
		vector2::Vec2 m_GetOldEndPos() { return m_oldEndPos; }
		int m_GetOldGridKey() { return m_oldGridKey; }
	};

	class ModifyText : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::TextComponent* m_changedComp;
		std::string m_oldFile;
		std::string m_oldText;
		int m_oldLayer;
		float m_oldSize;
		vector3::Vec3 m_oldColor;

	public:
		ModifyText(ecs::ComponentType inType, ecs::EntityID inID, ecs::TextComponent* inComp, ecs::TextComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYTEXT),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldFile(inOld.m_fileName), m_oldText(inOld.m_text),
			m_oldLayer(inOld.m_fontLayer), m_oldSize(inOld.m_fontSize),
			m_oldColor(inOld.m_color) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		std::string m_GetOldFile() { return m_oldFile; }
		std::string m_GetOldText() { return m_oldText; }
		int m_GetOldLayer() { return m_oldLayer; }
		float m_GetOldSize() { return m_oldSize; }
		vector3::Vec3 m_GetOldColor() { return m_oldColor; }

		// New function
		ecs::TextComponent* m_GetComp() { return m_changedComp; }
	};

	class ModifyRigid : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::RigidBodyComponent* m_changedComp;
		vector2::Vec2 m_oldVelocity;
		vector2::Vec2 m_oldAcceleration;
		float m_oldRotation;
		float m_oldAngularVelocity;
		float m_oldAngularAcceleration;
		float m_oldMass;
		float m_oldInverseMass;
		float m_oldLinearDamping;
		float m_oldAngularDamping;
		vector2::Vec2 m_oldForce;
		float m_oldTorque;
		bool m_oldKinematic;
		bool m_oldStatic;

	public:
		ModifyRigid(ecs::ComponentType inType, ecs::EntityID inID, ecs::RigidBodyComponent* inComp, ecs::RigidBodyComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYRIGID),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldVelocity(inOld.m_Velocity), m_oldAcceleration(inOld.m_Acceleration),
			m_oldRotation(inOld.m_Rotation), m_oldAngularVelocity(inOld.m_AngularVelocity),
			m_oldAngularAcceleration(inOld.m_AngularAcceleration), m_oldMass(inOld.m_Mass),
			m_oldInverseMass(inOld.m_InverseMass), m_oldLinearDamping(inOld.m_LinearDamping),
			m_oldAngularDamping(inOld.m_AngularDamping), m_oldForce(inOld.m_Force),
			m_oldTorque(inOld.m_Torque), m_oldKinematic(inOld.m_IsKinematic), m_oldStatic(inOld.m_IsStatic) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		vector2::Vec2 m_GetOldVelocity() { return m_oldVelocity; }
		vector2::Vec2 m_GetOldAcceleration() { return m_oldAcceleration; }
		float m_GetOldRotation() { return m_oldRotation; }
		float m_GetOldAngularVelocity() { return m_oldAngularVelocity; }
		float m_GetOldAngularAcceleration() { return m_oldAngularAcceleration; }
		float m_GetOldMass() { return m_oldMass; }
		float m_GetOldInverseMass() { return m_oldInverseMass; }
		float m_GetOldLinearDamping() { return m_oldLinearDamping; }
		float m_GetOldAngularDamping() { return m_oldAngularDamping; }
		vector2::Vec2 m_GetOldForce() { return m_oldForce; }
		float m_GetOldTorque() { return m_oldTorque; }
		bool m_GetOldKinematic() { return m_oldKinematic; }
		bool m_GetOldStatic() { return m_oldStatic; }

		// New function
		ecs::RigidBodyComponent* m_GetComp() { return m_changedComp; }
	};

	class ModifySprite : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::SpriteComponent* m_changedComp;
		std::string m_oldFile;
		int m_oldLayer;
		vector3::Vec3 m_oldColor;
		float m_oldAlpha;
		bool m_oldIlluminated;

	public:
		ModifySprite(ecs::ComponentType inType, ecs::EntityID inID, ecs::SpriteComponent* inComp, ecs::SpriteComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYSPRITE),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldFile(inOld.m_imageFile), m_oldLayer(inOld.m_layer),
			m_oldColor(inOld.m_color), m_oldAlpha(inOld.m_alpha),
			m_oldIlluminated(inOld.m_isIlluminated) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		std::string m_GetOldFile() { return m_oldFile; }
		int m_GetOldLayer() { return m_oldLayer; }
		vector3::Vec3 m_GetOldColor() { return m_oldColor; }
		float m_GetOldAlpha() { return m_oldAlpha; }
		bool m_GetOldIlluminated() { return m_oldIlluminated; }

		// New function
		ecs::SpriteComponent* m_GetComp() { return m_changedComp; }
	};



}
