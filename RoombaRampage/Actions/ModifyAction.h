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
#include "../Config/pch.h"
#include "Action.h"
#include "../ECS/Hierachy.h"
#include "../Asset Manager/AssetManager.h"


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


	class ModifyAnimComponent : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::AnimationComponent* m_changedComp;
		int m_oldFrameNum, m_newFrameNum;
		int m_oldFPS, m_newFPS;
		float m_oldFrameTimer, m_newFrameTimer;
		bool m_oldIsAnim, m_newIsAnim;
		int m_oldStrip, m_newStrip;
	public:
		ModifyAnimComponent(ecs::EntityID inID, ecs::AnimationComponent* inComp, int inOldFrame, int inOldFPS,float inOldFT, bool oldCheck, int inOldStip) :
			m_entityID(inID), m_changedComp(inComp), m_oldFrameNum(inOldFrame), m_newFrameNum(inComp->m_frameNumber), m_oldFPS(inOldFPS), m_newFPS(inComp->m_framesPerSecond),
			m_oldFrameTimer(inOldFT), m_newFrameTimer(inComp->m_frameTimer),m_oldIsAnim(oldCheck),m_newIsAnim(inComp->m_isAnimating), m_oldStrip(inOldStip), m_newStrip(inComp->m_stripCount) {}

		void m_UndoAction() override {
			m_changedComp->m_frameNumber = m_oldFrameNum;
			m_changedComp->m_framesPerSecond = m_oldFPS;
			m_changedComp->m_frameTimer = m_oldFrameTimer;
			m_changedComp->m_isAnimating = m_oldIsAnim;
			m_changedComp->m_stripCount = m_oldStrip;

		}

		void m_RedoAction() override {
			m_changedComp->m_frameNumber = m_newFrameNum;
			m_changedComp->m_framesPerSecond = m_newFPS;
			m_changedComp->m_frameTimer = m_newFrameTimer;
			m_changedComp->m_isAnimating = m_newIsAnim;
			m_changedComp->m_stripCount = m_newStrip;
		}


	};
	class RemoveAudio : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::AudioComponent* m_changedComp;
		std::string m_name;
		std::string m_filePath;
		float m_volume;
		bool m_loop;
		bool m_playOnStart, m_hasPlayed;
		float m_pan;
	public:
		RemoveAudio(ecs::EntityID inID, ecs::AudioComponent* inComp,std::string inName, std::string inFP ,float inVol, bool inLoop, bool inPlay, bool inPlayed, float inPan) :
			m_entityID(inID), m_changedComp(inComp), m_name(inName), m_filePath(inFP), m_volume(inVol), m_loop(inLoop), m_playOnStart(inPlay), m_hasPlayed(inPlayed), m_pan(inPan) {}

		void m_UndoAction() override {
			ecs::AudioFile newAF;
			newAF.m_Name = m_name;
			newAF.m_FilePath = m_filePath;
			newAF.m_Volume = m_volume;
			newAF.m_Loop = m_loop;
			newAF.m_PlayOnStart = m_playOnStart;
			newAF.m_HasPlayed = m_hasPlayed;
			newAF.m_Pan = m_pan;
			m_changedComp->m_AudioFiles.emplace_back(newAF);

		}

		void m_RedoAction() override {
			for (auto it2 = m_changedComp->m_AudioFiles.begin(); it2 != m_changedComp->m_AudioFiles.end();) {
				if (it2->m_Name == m_name && it2->m_FilePath == m_filePath && it2->m_Volume == m_volume) {
					auto& audioManager = assetmanager::AssetManager::m_funcGetInstance()->m_audioManager;
					audioManager.m_StopAudioForEntity(m_entityID, it2->m_Name);
					it2 = m_changedComp->m_AudioFiles.erase(it2);
				}
			}
			
		}


	};
	class AddAudio : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::AudioComponent* m_changedComp;
		std::string m_name;
		std::string m_filePath;
		float m_volume;
		bool m_loop;
		bool m_playOnStart, m_hasPlayed;
		float m_pan;
	public:
		AddAudio(ecs::EntityID inID, ecs::AudioComponent* inComp, std::string inName, std::string inFP, float inVol, bool inLoop, bool inPlay, bool inPlayed, float inPan) :
			m_entityID(inID), m_changedComp(inComp), m_name(inName), m_filePath(inFP), m_volume(inVol), m_loop(inLoop), m_playOnStart(inPlay), m_hasPlayed(inPlayed), m_pan(inPan) {}

		void m_UndoAction() override {
			for (auto it2 = m_changedComp->m_AudioFiles.begin(); it2 != m_changedComp->m_AudioFiles.end();) {
				if (it2->m_Name == m_name && it2->m_FilePath == m_filePath && it2->m_Volume == m_volume) {
					auto& audioManager = assetmanager::AssetManager::m_funcGetInstance()->m_audioManager;
					audioManager.m_StopAudioForEntity(m_entityID, it2->m_Name);
					it2 = m_changedComp->m_AudioFiles.erase(it2);
				}
			}
			//ecs::AudioFile newAF;
			//newAF.m_Name = m_name;
			//newAF.m_FilePath = m_filePath;
			//newAF.m_Volume = m_volume;
			//newAF.m_Loop = m_loop;
			//newAF.m_PlayOnStart = m_playOnStart;
			//newAF.m_HasPlayed = m_hasPlayed;
			//newAF.m_Pan = m_pan;
			//m_changedComp->m_AudioFiles.emplace_back(newAF);

		}

		void m_RedoAction() override {
			m_changedComp->m_AudioFiles.emplace_back();

		}
	};

	class ModifyCamera : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::CameraComponent* m_changedComp;
		float m_oldLeft, m_newLeft;
		float m_oldRight, m_newRight;
		float m_oldTop, m_newTop;
		float m_oldBot, m_newBot;
		float m_oldAR, m_newAR;
	public:
		ModifyCamera(ecs::EntityID inID, ecs::CameraComponent* inComp, float inLeft, float inRight, float inTop, float inBot, float inAR) :
			m_entityID(inID), m_changedComp(inComp), m_oldLeft(inLeft), m_newLeft(inComp->m_left), m_oldRight(inRight), m_newRight(inComp->m_right),
			m_oldTop(inTop), m_newTop(inComp->m_top), m_oldBot(inBot), m_newBot(inComp->m_bottom), m_oldAR(inAR), m_newAR(inComp->m_aspectRatio) {}

		void m_UndoAction() override {
			m_changedComp->m_left = m_oldLeft;
			m_changedComp->m_right = m_oldRight;
			m_changedComp->m_top = m_oldTop;
			m_changedComp->m_bottom = m_oldBot;
			m_changedComp->m_aspectRatio = m_oldAR;
			
		}

		void m_RedoAction() override {
			m_changedComp->m_left = m_newLeft;
			m_changedComp->m_right = m_newRight;
			m_changedComp->m_top = m_newTop;
			m_changedComp->m_bottom = m_newBot;
			m_changedComp->m_aspectRatio = m_newAR;
		}
	};

	class ModifyCollider : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::ColliderComponent* m_changedComp;
		vector2::Vec2 m_oldSize, m_newSize;
		vector2::Vec2 m_oldOffset, m_newOffset;
		bool m_oldDraw, m_newDraw;
		float m_oldRad, m_newRad;
		bool m_oldCR, m_newCR;
		bool m_oldCC, m_newCC;
		physicspipe::EntityType m_oldShape, m_newShape;
	public:
		ModifyCollider(ecs::EntityID inID, ecs::ColliderComponent* inComp, vector2::Vec2 inSize, vector2::Vec2 inOffset,bool inDraw, float inRad, bool inCR, bool inCC, physicspipe::EntityType inShape) :
			m_entityID(inID), m_changedComp(inComp), m_oldSize(inSize), m_newSize(inComp->m_Size), m_oldOffset(inOffset), m_newOffset(inComp->m_OffSet), m_oldDraw(inDraw), m_newDraw(inComp->m_drawDebug),
			m_oldRad(inRad), m_newRad(inComp->m_radius), m_oldCR(inCR), m_newCR(inComp->m_collisionResponse), m_oldCC(inCC), m_newCC(inComp->m_collisionCheck),
			m_oldShape(inShape), m_newShape(inComp->m_type) {}

		void m_UndoAction() override {
			m_changedComp->m_Size = m_oldSize;
			m_changedComp->m_OffSet = m_oldOffset;
			m_changedComp->m_drawDebug = m_oldDraw;
			m_changedComp->m_radius = m_oldRad;
			m_changedComp->m_collisionResponse = m_oldCR;
			m_changedComp->m_collisionCheck = m_oldCC;
			m_changedComp->m_type = m_oldShape;

		}

		void m_RedoAction() override {
			m_changedComp->m_Size = m_newSize;
			m_changedComp->m_OffSet = m_newOffset;
			m_changedComp->m_drawDebug = m_newDraw;
			m_changedComp->m_radius = m_newRad;
			m_changedComp->m_collisionResponse = m_newCR;
			m_changedComp->m_collisionCheck = m_newCC;
			m_changedComp->m_type = m_newShape;
		}
	};

	class ModifyEnemy : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::EnemyComponent* m_changedComp;
		int m_oldTag, m_newTag;
		int m_oldType, m_newType;
		int m_oldBehave, m_newBehave;
	public:
		ModifyEnemy(ecs::EntityID inID, ecs::EnemyComponent* inComp, int inTag, int inType, int inBehave) :
			m_entityID(inID), m_changedComp(inComp), m_oldTag(inTag), m_newTag(inComp->m_enemyTag), m_oldType(inType), m_newType(inComp->m_enemyTypeInt),
			m_oldBehave(inBehave), m_newBehave(inComp->m_enemyRoamBehaviourInt) {}

		void m_UndoAction() override {
			m_changedComp->m_enemyTag = m_oldTag;
			m_changedComp->m_enemyTypeInt = m_oldType;
			m_changedComp->m_enemyRoamBehaviourInt = m_oldBehave;

		}

		void m_RedoAction() override {
			m_changedComp->m_enemyTag = m_newTag;
			m_changedComp->m_enemyTypeInt = m_newType;
			m_changedComp->m_enemyRoamBehaviourInt = m_newBehave;
		}
	};

	class ModifyGrid : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::GridComponent* m_changedComp;
		vector2::Vec2 m_oldAnc, m_newAnc;
		int m_oldRowLen, m_newRowLen;
		int m_oldColLen, m_newColLen;
		bool m_oldColl, m_newColl;
		int m_oldGridKey, m_newGridKey;
	public:
		ModifyGrid(ecs::EntityID inID, ecs::GridComponent* inComp, vector2::Vec2 inAnc,int inRLen, int inCLen, bool inColl, int inGridKey) :
			m_entityID(inID), m_changedComp(inComp), m_oldAnc(inAnc), m_newAnc(inComp->m_Anchor), m_oldRowLen(inRLen), m_newRowLen(inComp->m_GridRowLength),
			m_oldColLen(inCLen), m_newColLen(inComp->m_GridColumnLength), m_oldColl(inColl), m_newColl(inComp->m_SetCollidable),
			m_oldGridKey(inGridKey), m_newGridKey(inComp->m_GridKey){}

		void m_UndoAction() override {
			m_changedComp->m_Anchor = m_oldAnc;
			m_changedComp->m_GridRowLength = m_oldRowLen;
			m_changedComp->m_GridColumnLength = m_oldColLen;
			m_changedComp->m_SetCollidable = m_oldColl;
			m_changedComp->m_GridKey = m_oldGridKey;

		}

		void m_RedoAction() override {
			m_changedComp->m_Anchor = m_newAnc;
			m_changedComp->m_GridRowLength = m_newRowLen;
			m_changedComp->m_GridColumnLength = m_newColLen;
			m_changedComp->m_SetCollidable = m_newColl;
			m_changedComp->m_GridKey = m_newGridKey;
		}
	};

	class ModifyLight : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::LightingComponent* m_changedComp;
		graphicpipe::LightType m_oldType, m_newType;
		float m_oldIntensity, m_newIntensity;
		vector2::Vec2 m_oldInOutRad, m_newInOutRad;
		vector3::Vec3 m_oldClr, m_newClr;
	public:
		ModifyLight(ecs::EntityID inID, ecs::LightingComponent* inComp, graphicpipe::LightType inType, float inIntensity,vector2::Vec2 inRad, vector3::Vec3 inClr) :
			m_entityID(inID), m_changedComp(inComp), m_oldType(inType), m_newType(inComp->m_lightType), m_oldIntensity(inIntensity), m_newIntensity(inComp->m_intensity),
			m_oldInOutRad(inRad), m_newInOutRad(inComp->m_innerOuterRadius), m_oldClr(inClr), m_newClr(inComp->m_colour) {}

		void m_UndoAction() override {
			m_changedComp->m_lightType = m_oldType;
			m_changedComp->m_intensity = m_oldIntensity;
			m_changedComp->m_innerOuterRadius = m_oldInOutRad;
			m_changedComp->m_colour = m_oldClr;
		}

		void m_RedoAction() override {
			m_changedComp->m_lightType = m_newType;
			m_changedComp->m_intensity = m_newIntensity;
			m_changedComp->m_innerOuterRadius = m_newInOutRad;
			m_changedComp->m_colour = m_newClr;
		}
	};


	class ModifyPathfinding : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::PathfindingComponent* m_changedComp;
		vector2::Vec2 m_oldStart, m_newStart;
		vector2::Vec2 m_oldEnd, m_newEnd;
		int m_oldKey, m_newKey;
	public:
		ModifyPathfinding(ecs::EntityID inID, ecs::PathfindingComponent* inComp, vector2::Vec2 inStart, vector2::Vec2 inEnd, int inKey) :
			m_entityID(inID), m_changedComp(inComp), m_oldStart(inStart), m_newStart(inComp->m_StartPos), m_oldEnd(inEnd), m_newEnd(inComp->m_TargetPos),
			m_oldKey(inKey), m_newKey(inComp->m_GridKey) {}

		void m_UndoAction() override {
			m_changedComp->m_StartPos = m_oldStart;
			m_changedComp->m_TargetPos = m_oldEnd;
			m_changedComp->m_GridKey = m_oldKey;
		}

		void m_RedoAction() override {
			m_changedComp->m_StartPos = m_newStart;
			m_changedComp->m_TargetPos = m_newEnd;
			m_changedComp->m_GridKey = m_newKey;
		}
	};

	class ModifyRigid : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::RigidBodyComponent* m_changedComp;
		vector2::Vec2 m_oldVelo, m_newVelo;
		vector2::Vec2 m_oldAccel, m_newAccel;
		float m_oldRot, m_newRot;
		float m_oldAngVelo, m_newAngVelo;
		float m_oldAngAccel, m_newAngAccel;
		float m_oldMass, m_newMass;
		float m_oldInvMass, m_newInvMass;
		float m_oldLinDamp, m_newLinDamp;
		float m_oldAngDamp, m_newAngDamp;
		vector2::Vec2 m_oldForce, m_newForce;
		float m_oldTorque, m_newTorque;
		bool m_oldKine, m_newKine;
		bool m_oldStatic, m_newStatic;
	public:
		ModifyRigid(ecs::EntityID inID, ecs::RigidBodyComponent* inComp, vector2::Vec2 inVelo, vector2::Vec2 inAccel, float inRot, float inAngVelo, float inAngAccel, float inMass, 
					float inInvMass,float inLinDamp, float inAngDamp, vector2::Vec2 inForce, float inTorque, bool inKine, bool inStatic) :
					m_entityID(inID), m_changedComp(inComp), m_oldVelo(inVelo), m_newVelo(inComp->m_Velocity), m_oldAccel(inAccel), m_newAccel(inComp->m_Acceleration),
					m_oldRot(inRot), m_newRot(inComp->m_Rotation), m_oldAngVelo(inAngVelo), m_newAngVelo(inComp->m_AngularVelocity),
					m_oldAngAccel(inAngAccel), m_newAngAccel(inComp->m_AngularAcceleration), m_oldMass(inMass), m_newMass(inComp->m_Mass),
					m_oldInvMass(inInvMass), m_newInvMass(inComp->m_InverseMass), m_oldLinDamp(inLinDamp), m_newLinDamp(inComp->m_LinearDamping),
					m_oldAngDamp(inAngDamp), m_newAngDamp(inComp->m_AngularDamping), m_oldForce(inForce), m_newForce(inComp->m_Force),
					m_oldTorque(inTorque), m_newTorque(inComp->m_Torque), m_oldKine(inKine), m_newKine(inComp->m_IsKinematic), m_oldStatic(inStatic), m_newStatic(inComp->m_IsStatic){}

		void m_UndoAction() override {
			m_changedComp->m_Velocity = m_oldVelo;
			m_changedComp->m_Acceleration = m_oldAccel;
			m_changedComp->m_Rotation = m_oldRot;
			m_changedComp->m_AngularVelocity = m_oldAngVelo;
			m_changedComp->m_AngularAcceleration = m_oldAngAccel;
			m_changedComp->m_Mass = m_oldMass;
			m_changedComp->m_InverseMass = m_oldInvMass;
			m_changedComp->m_LinearDamping = m_oldLinDamp;
			m_changedComp->m_AngularDamping = m_oldAngDamp;
			m_changedComp->m_Force = m_oldForce;
			m_changedComp->m_Torque = m_oldTorque;
			m_changedComp->m_IsKinematic = m_oldKine;
			m_changedComp->m_IsStatic = m_oldStatic;
		}

		void m_RedoAction() override {
			m_changedComp->m_Velocity = m_newVelo;
			m_changedComp->m_Acceleration = m_newAccel;
			m_changedComp->m_Rotation = m_newRot;
			m_changedComp->m_AngularVelocity = m_newAngVelo;
			m_changedComp->m_AngularAcceleration = m_newAngAccel;
			m_changedComp->m_Mass = m_newMass;
			m_changedComp->m_InverseMass = m_newInvMass;
			m_changedComp->m_LinearDamping = m_newLinDamp;
			m_changedComp->m_AngularDamping = m_newAngDamp;
			m_changedComp->m_Force = m_newForce;
			m_changedComp->m_Torque = m_newTorque;
			m_changedComp->m_IsKinematic = m_newKine;
			m_changedComp->m_IsStatic = m_newStatic;
		}
	};

	class ModifySprite : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::SpriteComponent* m_changedComp;
		std::string m_oldFile, m_newFile;
		int m_oldLayer, m_newLayer;
		vector3::Vec3 m_oldClr, m_newClr;
		float m_oldAlpha, m_newAlpha;
		bool m_oldIllum, m_newIllum;
	public:
		ModifySprite(ecs::EntityID inID, ecs::SpriteComponent* inComp, std::string inFile, int inLayer, vector3::Vec3 inClr, float inAlpha, bool inIllum) :
			m_entityID(inID), m_changedComp(inComp), m_oldFile(inFile), m_newFile(inComp->m_imageFile), m_oldLayer(inLayer), m_newLayer(inComp->m_layer),
			m_oldClr(inClr), m_newClr(inComp->m_color), m_oldAlpha(inAlpha), m_newAlpha(inComp->m_alpha), m_oldIllum(inIllum), m_newIllum(inComp->m_isIlluminated) {}

		void m_UndoAction() override {
			m_changedComp->m_imageFile = m_oldFile;
			m_changedComp->m_layer = m_oldLayer;
			m_changedComp->m_color = m_oldClr;
			m_changedComp->m_alpha = m_oldAlpha;
			m_changedComp->m_isIlluminated = m_oldIllum;
		}

		void m_RedoAction() override {
			m_changedComp->m_imageFile = m_newFile;
			m_changedComp->m_layer = m_newLayer;
			m_changedComp->m_color = m_newClr;
			m_changedComp->m_alpha = m_newAlpha;
			m_changedComp->m_isIlluminated = m_newIllum;
		}
	};

	class ModifyText : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::TextComponent* m_changedComp;
		std::string m_oldFile, m_newFile;
		std::string m_oldText, m_newText;
		int m_oldLayer, m_newLayer;
		float m_oldSize, m_newSize;
		vector3::Vec3 m_oldClr, m_newClr;
	public:
		ModifyText(ecs::EntityID inID, ecs::TextComponent* inComp, std::string inFile, std::string inText, int inLayer, float inSize,vector3::Vec3 inClr) :
			m_entityID(inID), m_changedComp(inComp), m_oldFile(inFile), m_newFile(inComp->m_fileName), m_oldText(inText), m_newText(inComp->m_text), 
			m_oldLayer(inLayer), m_newLayer(inComp->m_fontLayer), m_oldSize(inSize), m_newSize(inComp->m_fontSize), m_oldClr(inClr), m_newClr(inComp->m_color){}

		void m_UndoAction() override {
			m_changedComp->m_fileName = m_oldFile;
			m_changedComp->m_text = m_oldText;
			m_changedComp->m_color = m_oldClr;
			m_changedComp->m_fontLayer = m_oldLayer;
			m_changedComp->m_fontSize = m_oldSize;
		}

		void m_RedoAction() override {
			m_changedComp->m_fileName = m_newFile;
			m_changedComp->m_text = m_newText;
			m_changedComp->m_color = m_newClr;
			m_changedComp->m_fontLayer = m_newLayer;
			m_changedComp->m_fontSize = m_newSize;
		}
	};

	class ModifyButton : public Action {
	private:
		ecs::EntityID m_entityID;
		ecs::ButtonComponent* m_changedComp;
		vector2::Vec2 m_oldPos, m_newPos;
		vector2::Vec2 m_oldScale, m_newScale;
		bool m_oldClick, m_newClick;
	public:
		ModifyButton(ecs::EntityID inID, ecs::ButtonComponent* inComp, vector2::Vec2 inPos, vector2::Vec2 inScale, bool inClick) :
			m_entityID(inID), m_changedComp(inComp), m_oldPos(inPos), m_newPos(inComp->m_Position), m_oldScale(inScale), m_newScale(inComp->m_Scale),
			m_oldClick(inClick), m_newClick(inComp->m_IsClick) {}

		void m_UndoAction() override {
			m_changedComp->m_Position = m_oldPos;
			m_changedComp->m_Scale = m_oldScale;
			m_changedComp->m_IsClick = m_oldClick;
		}

		void m_RedoAction() override {
			m_changedComp->m_Position = m_newPos;
			m_changedComp->m_Scale = m_newScale;
			m_changedComp->m_IsClick = m_newClick;
		}
	};


	//class ModifyAudio : public Action {
	//private:
	//	ecs::EntityID m_entityID;
	//	ecs::AudioComponent* m_changedComp;
	//	std::string m_oldName, m_newName;
	//	std::string m_oldFilePath, m_newFilePath;
	//	float m_oldVolume, m_newVolume;
	//	bool m_oldLoop,m_newLoop;
	//	bool m_oldPlayOnS, m_newPlayOns;
	//	bool m_oldHasPlayed, m_newHasPlayed;
	//	float m_oldPan, m_newPan;
	//public:
	//	ModifyAudio(ecs::EntityID inID, ecs::AudioComponent* inComp, std::string inName, std::string inFP, float inVol, bool inLoop, bool inPlay, bool inPlayed, float inPan) :
	//		m_entityID(inID), m_changedComp(inComp), m_oldName(inName) {}//m_newName(), m_filePath(inFP), m_volume(inVol), m_loop(inLoop), m_playOnStart(inPlay), m_hasPlayed(inPlayed), m_pan(inPan) {}

	//	void m_UndoAction() override {

	//		//ecs::AudioFile newAF;
	//		//newAF.m_Name = m_name;
	//		//newAF.m_FilePath = m_filePath;
	//		//newAF.m_Volume = m_volume;
	//		//newAF.m_Loop = m_loop;
	//		//newAF.m_PlayOnStart = m_playOnStart;
	//		//newAF.m_HasPlayed = m_hasPlayed;
	//		//newAF.m_Pan = m_pan;
	//		//m_changedComp->m_AudioFiles.emplace_back(newAF);

	//	}

	//	void m_RedoAction() override {
	//		m_changedComp->m_AudioFiles.emplace_back();

	//	}
	//};

}