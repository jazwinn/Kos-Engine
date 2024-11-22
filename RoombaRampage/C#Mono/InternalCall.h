/********************************************************************/
/*!
\file      InternalCall.h
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Nov 711, 2024
\brief     This header file defines the `InternalCall` class, which contains
		   functions that allow C# scripts to interact with ECS components.
		   - m_InternalGetTransformComponent: Retrieves transform data of an entity.
		   - m_InternalSetTransformComponent: Sets transform data of an entity.
		   - m_InternalGetVelocity: Retrieves the velocity of an entity.
		   - m_InternalSetVelocity: Sets the velocity of an entity.
		   - m_InternalCallIsKeyPressed: Checks if a key is currently pressed.
		   - m_InternalCallGetDeltaTime: Retrieves the fixed delta time.
		   - m_RegisterInternalCalls: Registers internal calls for Mono scripting.

Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef INTCALL_H
#define INTCALL_H

#include"../ECS/ECS.h"
#include "../Inputs/Keycodes.h"

namespace script {

	class InternalCall {
		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalGetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate)
			\brief     Retrieves the transform component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the transform from.
			\param[out] trans  Pointer to the entity's position vector.
			\param[out] scale  Pointer to the entity's scale vector.
			\param[out] rotate Pointer to the entity's rotation value.
			\return    True if the transform component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalSetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate)
			\brief     Sets the transform component of a specified entity.
			\param[in] entity The ID of the entity to set the transform for.
			\param[in] trans  Pointer to the position vector to set.
			\param[in] scale  Pointer to the scale vector to set.
			\param[in] rotate Pointer to the rotation value to set.
			\return    True if the transform component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate);

		static bool m_InternalGetTranslate(ecs::EntityID entity, vector2::Vec2* trans);

		static bool m_InternalSetTranslate(ecs::EntityID entity, vector2::Vec2* trans);

		static bool m_InternalGetColliderComponent(ecs::EntityID entity, vector2::Vec2* size, vector2::Vec2* offset, bool* drawDebug, float* radius, bool* isCollided);

		static bool m_InternalSetColliderComponent(ecs::EntityID entity, vector2::Vec2* size, vector2::Vec2* offset, bool drawDebug, float radius, bool isCollided);

		static bool m_InternalGetPlayerComponent(ecs::EntityID entity, bool* control);

		static bool m_InternalSetPlayerComponent(ecs::EntityID entity, bool control);

		static bool m_InternalGetRigidBodyComponent(ecs::EntityID entity, vector2::Vec2* velocity, vector2::Vec2* acceleration, float* rotation);

		static bool m_InternalSetRigidBodyComponent(ecs::EntityID entity, vector2::Vec2* velocity, vector2::Vec2* acceleration, float* rotation);

		static bool m_InternalGetTextComponent(ecs::EntityID entity, std::string* text, std::string* fileName, int* fontLayer, float* fontSize, vector3::Vec3* color);

		static bool m_InternalSetTextComponent(ecs::EntityID entity, const std::string& text, const std::string& fileName, int fontLayer, float fontSize, const vector3::Vec3& color);

		static bool m_InternalGetAnimationComponent(ecs::EntityID entity, int* frameNumber, int* framesPerSecond, float* frameTimer, bool* isAnimating);

		static bool m_InternalSetAnimationComponent(ecs::EntityID entity, int frameNumber, int framesPerSecond, float frameTimer, bool isAnimating);

		static bool m_InternalGetCameraComponent(ecs::EntityID entity, float* left, float* right, float* top, float* bottom, float* aspectRatio);

		static bool m_InternalSetCameraComponent(ecs::EntityID entity, float left, float right, float top, float bottom, float aspectRatio);

		static bool m_InternalGetButtonComponent(ecs::EntityID entity, vector2::Vec2* position, vector2::Vec2* scale, bool* isClick);

		static bool m_InternalSetButtonComponent(ecs::EntityID entity, const vector2::Vec2& position, const vector2::Vec2& scale, bool isClick);

		static bool m_InternalGetScriptNames(ecs::EntityID entity, std::vector<std::string>& scripts);

		static bool m_InternalAddScriptInstance(ecs::EntityID entity, const std::string& scriptName, MonoObject* instance);
		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalGetVelocity(ecs::EntityID entity, vector2::Vec2* vec)
			\brief     Retrieves the velocity of a specified entity.
			\param[in] entity The ID of the entity to retrieve the velocity from.
			\param[out] vec   Pointer to the entity's velocity vector.
			\return    True if the velocity component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetVelocity(ecs::EntityID entity, vector2::Vec2* vec);
		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalSetVelocity(ecs::EntityID entity, vector2::Vec2* vec)
			\brief     Sets the velocity of a specified entity.
			\param[in] entity The ID of the entity to set the velocity for.
			\param[in] vec    Pointer to the velocity vector to set.
			\return    True if the velocity component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetVelocity(ecs::EntityID entity, vector2::Vec2* vec);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalCallIsKeyPressed(keyCode key)
			\brief     Checks if a specific key is currently pressed.
			\param[in] key The key code to check.
			\return    True if the specified key is pressed; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalCallIsKeyPressed(keyCode key);

		static vector2::Vec2 m_InternalGetMousePosition();

		static bool m_InternalCallIsKeyReleased(keyCode key);

		static bool m_InternalCallIsKeyTriggered(keyCode key);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalCallGetDeltaTime(float* deltatime)
			\brief     Retrieves the fixed delta time.
			\param[out] deltatime Pointer to store the retrieved delta time value.
			\return    Always returns false for consistency in call handling.
		*/
		/******************************************************************/
		static bool m_InternalCallGetDeltaTime(float* deltatime);

		static int m_InternalCallGetPlayer();

		static float m_InternalCallIsCollided(ecs::EntityID entity);

		static MonoArray* m_InternalCallGetCollidedEntities(ecs::EntityID entity);

		static MonoString* m_InternalCallGetTag(ecs::EntityID entity);

		static void m_InternalCallSetSceneActive(MonoString* monoString);

		static void m_UnloadAllScene();

		static void m_InternalCallLoadScene(MonoString* monoString);

		static int m_InternalCallAddPrefab(MonoString* prefab, const float* x, const float* y, const float* rotation);

		static void m_InternalCallDeleteEntity(ecs::EntityID);

		static bool m_InternalGetSpriteComponent(ecs::EntityID entity, MonoString** imageFile, int* layer, vector3::Vec3* color, float* alpha);

		static bool m_InternalSetSpriteComponent(ecs::EntityID entity, MonoString* imageFile, const int* layer, const vector3::Vec3* color, const float* alpha);

	public:



		/******************************************************************/
		/*!
			\fn        void InternalCall::m_RegisterInternalCalls()
			\brief     Registers internal call functions for Mono scripting,
					   allowing C# scripts to access C++ ECS functions.
		*/
		/******************************************************************/
		static void m_RegisterInternalCalls();
	};

}

#endif