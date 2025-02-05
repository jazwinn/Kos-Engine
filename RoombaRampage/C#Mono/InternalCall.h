/********************************************************************/
/*!
\file      InternalCall.h
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Nov 27, 2024
\brief     This header file defines the `InternalCall` class, which contains
		   functions that allow C# scripts to interact with ECS components.

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

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalGetTranslate(ecs::EntityID entity, vector2::Vec2* trans)
			\brief     Retrieves the position of a specified entity.
			\param[in] entity The ID of the entity to retrieve the position from.
			\param[out] trans  Pointer to the position vector to store the retrieved position.
			\return    True if the position is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetTranslate(ecs::EntityID entity, vector2::Vec2* trans);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalSetTranslate(ecs::EntityID entity, vector2::Vec2* trans)
			\brief     Sets the position of a specified entity.
			\param[in] entity The ID of the entity to set the position for.
			\param[in] trans  Pointer to the position vector to set.
			\return    True if the position is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetTranslate(ecs::EntityID entity, vector2::Vec2* trans);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalGetColliderComponent(ecs::EntityID entity, vector2::Vec2* size, vector2::Vec2* offset, bool* drawDebug, float* radius, int* m_blockedFlag, float* isCollided, bool* collisionCheck)
			\brief     Retrieves the collider component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the collider from.
			\param[out] size   Pointer to the collider size vector.
			\param[out] offset Pointer to the collider offset vector.
			\param[out] drawDebug Pointer to whether debug drawing is enabled.
			\param[out] radius Pointer to the collider's radius.
			\param[out] m_blockedFlag Pointer to the entity's blocked flag.
			\param[out] isCollided Pointer to store the collision state.
			\param[out] collisionCheck Pointer to enable or disable collision checking.
			\return    True if the collider is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetColliderComponent(ecs::EntityID entity, vector2::Vec2* size, vector2::Vec2* offset, bool* drawDebug, float* radius, unsigned int* m_blockedFlag, float* isCollided, bool* collisionCheck, bool* collisionresponse);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalSetColliderComponent(ecs::EntityID entity, vector2::Vec2* size, vector2::Vec2* offset, bool* drawDebug, float* radius, int* m_blockedFlag, float* isCollided, bool* collisionCheck)
			\brief     Sets the collider component of a specified entity.
			\param[in] entity The ID of the entity to set the collider for.
			\param[in] size   Pointer to the collider size vector to set.
			\param[in] offset Pointer to the collider offset vector to set.
			\param[in] drawDebug Pointer to enable or disable debug drawing.
			\param[in] radius Pointer to the collider's radius to set.
			\param[in] m_blockedFlag Pointer to the entity's blocked flag to set.
			\param[in] isCollided Pointer to set the collision state.
			\param[in] collisionCheck Pointer to enable or disable collision checking.
			\return    True if the collider is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetColliderComponent(ecs::EntityID entity, vector2::Vec2* size, vector2::Vec2* offset, bool* drawDebug, float* radius, unsigned int* m_blockedFlag, float* isCollided, bool* collisionCheck, bool* collisionresponse);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalGetPlayerComponent(ecs::EntityID entity, bool* control)
			\brief     Retrieves the player component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the player component from.
			\param[out] control Pointer to the player control flag.
			\return    True if the player component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetEnemyComponent(ecs::EntityID entity, int* enemytag, int* enemytype, int* enemybehaviour);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalSetPlayerComponent(ecs::EntityID entity, bool control)
			\brief     Sets the player component of a specified entity.
			\param[in] entity The ID of the entity to set the player component for.
			\param[in] control The player control flag to set.
			\return    True if the player component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetEnemyComponent(ecs::EntityID entity, int* enemytag, int* enemytype, int* enemybehaviour);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalGetRigidBodyComponent(ecs::EntityID entity, vector2::Vec2* velocity, vector2::Vec2* acceleration, float* rotation)
			\brief     Retrieves the rigid body component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the rigid body from.
			\param[out] velocity Pointer to the velocity vector.
			\param[out] acceleration Pointer to the acceleration vector.
			\param[out] rotation Pointer to the rotation value.
			\return    True if the rigid body component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetRigidBodyComponent(ecs::EntityID entity, vector2::Vec2* velocity, vector2::Vec2* acceleration, float* rotation, vector2::Vec2* previouspos, vector2::Vec2* directionvector);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalSetRigidBodyComponent(ecs::EntityID entity, vector2::Vec2* velocity, vector2::Vec2* acceleration, float* rotation)
			\brief     Sets the rigid body component of a specified entity.
			\param[in] entity The ID of the entity to set the rigid body for.
			\param[in] velocity Pointer to the velocity vector to set.
			\param[in] acceleration Pointer to the acceleration vector to set.
			\param[in] rotation Pointer to the rotation value to set.
			\return    True if the rigid body component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetRigidBodyComponent(ecs::EntityID entity, vector2::Vec2* velocity, vector2::Vec2* acceleration, float* rotation, vector2::Vec2* previouspos, vector2::Vec2* directionvector);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalGetTextComponent(ecs::EntityID entity, MonoString** text, MonoString** imageFile, int* fontLayer, float* fontSize, vector3::Vec3* color)
			\brief     Retrieves the text component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the text component from.
			\param[out] text Pointer to the text string.
			\param[out] imageFile Pointer to the image file name.
			\param[out] fontLayer Pointer to the font layer.
			\param[out] fontSize Pointer to the font size.
			\param[out] color Pointer to the text color vector.
			\return    True if the text component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetTextComponent(ecs::EntityID entity, MonoString** text, MonoString** imageFile, int* fontLayer, float* fontSize, vector3::Vec3* color);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalSetTextComponent(ecs::EntityID entity, MonoString* text, MonoString* fileName, int* fontLayer, float* fontSize, const vector3::Vec3* color)
			\brief     Sets the text component of a specified entity.
			\param[in] entity The ID of the entity to set the text component for.
			\param[in] text Pointer to the text string to set.
			\param[in] fileName Pointer to the image file name to set.
			\param[in] fontLayer Pointer to the font layer to set.
			\param[in] fontSize Pointer to the font size to set.
			\param[in] color Pointer to the text color vector to set.
			\return    True if the text component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetTextComponent(ecs::EntityID entity, MonoString* text, MonoString* fileName, int* fontLayer, float* fontSize, const vector3::Vec3* color);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalGetAnimationComponent(ecs::EntityID entity, int* frameNumber, int* framesPerSecond, float* frameTimer, bool* isAnimating, int* stripcount)
			\brief     Retrieves the animation component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the animation component from.
			\param[out] frameNumber Pointer to the frame number.
			\param[out] framesPerSecond Pointer to the frames per second.
			\param[out] frameTimer Pointer to the frame timer.
			\param[out] isAnimating Pointer to the animation state.
			\param[out] stripcount Pointer to the strip count.
			\return    True if the animation component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetAnimationComponent(ecs::EntityID entity, int* frameNumber, int* framesPerSecond, float* frameTimer, bool* isAnimating, int* stripcount);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalSetAnimationComponent(ecs::EntityID entity, int* frameNumber, int* framesPerSecond, float* frameTimer, bool* isAnimating, int* stripcount)
			\brief     Sets the animation component of a specified entity.
			\param[in] entity The ID of the entity to set the animation component for.
			\param[in] frameNumber Pointer to the frame number to set.
			\param[in] framesPerSecond Pointer to the frames per second to set.
			\param[in] frameTimer Pointer to the frame timer to set.
			\param[in] isAnimating Pointer to the animation state to set.
			\param[in] stripcount Pointer to the strip count to set.
			\return    True if the animation component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetAnimationComponent(ecs::EntityID entity, int* frameNumber, int* framesPerSecond, float* frameTimer, bool* isAnimating, int* stripcount);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalGetSpriteComponent(ecs::EntityID entity, MonoString** imageFile, int* layer, vector3::Vec3* color, float* alpha)
			\brief     Retrieves the sprite component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the sprite component from.
			\param[out] imageFile Pointer to the image file name.
			\param[out] layer Pointer to the layer.
			\param[out] color Pointer to the color vector.
			\param[out] alpha Pointer to the alpha value.
			\return    True if the sprite component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetSpriteComponent(ecs::EntityID entity, MonoString** imageFile, int* layer, vector3::Vec3* color, float* alpha);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalSetSpriteComponent(ecs::EntityID entity, MonoString* imageFile, const int* layer, const vector3::Vec3* color, const float* alpha)
			\brief     Sets the sprite component of a specified entity.
			\param[in] entity The ID of the entity to set the sprite component for.
			\param[in] imageFile Pointer to the image file name to set.
			\param[in] layer Pointer to the layer to set.
			\param[in] color Pointer to the color vector to set.
			\param[in] alpha Pointer to the alpha value to set.
			\return    True if the sprite component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetSpriteComponent(ecs::EntityID entity, MonoString* imageFile, const int* layer, const vector3::Vec3* color, const float* alpha);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalGetCameraComponent(ecs::EntityID entity, float* left, float* right, float* top, float* bottom, float* aspectRatio)
			\brief     Retrieves the camera component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the camera component from.
			\param[out] left Pointer to the left boundary value.
			\param[out] right Pointer to the right boundary value.
			\param[out] top Pointer to the top boundary value.
			\param[out] bottom Pointer to the bottom boundary value.
			\param[out] aspectRatio Pointer to the aspect ratio value.
			\return    True if the camera component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetCameraComponent(ecs::EntityID entity, float* left, float* right, float* top, float* bottom, float* aspectRatio);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalSetCameraComponent(ecs::EntityID entity, float left, float right, float top, float bottom, float aspectRatio)
			\brief     Sets the camera component of a specified entity.
			\param[in] entity The ID of the entity to set the camera component for.
			\param[in] left The left boundary value to set.
			\param[in] right The right boundary value to set.
			\param[in] top The top boundary value to set.
			\param[in] bottom The bottom boundary value to set.
			\param[in] aspectRatio The aspect ratio value to set.
			\return    True if the camera component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetCameraComponent(ecs::EntityID entity, float left, float right, float top, float bottom, float aspectRatio);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalGetButtonComponent(ecs::EntityID entity, vector2::Vec2* position, vector2::Vec2* scale, bool* isClick)
			\brief     Retrieves the button component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the button component from.
			\param[out] position Pointer to the button position vector.
			\param[out] scale Pointer to the button scale vector.
			\param[out] isClick Pointer to the button click state.
			\return    True if the button component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetButtonComponent(ecs::EntityID entity, vector2::Vec2* position, vector2::Vec2* scale, bool* isClick);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalSetButtonComponent(ecs::EntityID entity, const vector2::Vec2& position, const vector2::Vec2& scale, bool isClick)
			\brief     Sets the button component of a specified entity.
			\param[in] entity The ID of the entity to set the button component for.
			\param[in] position Reference to the position vector to set.
			\param[in] scale Reference to the scale vector to set.
			\param[in] isClick The button click state to set.
			\return    True if the button component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetButtonComponent(ecs::EntityID entity, const vector2::Vec2& position, const vector2::Vec2& scale, bool isClick);

		/******************************************************************/
		/*! \fn        MonoArray* InternalCall::m_InternalGetScriptNames(ecs::EntityID entity)
			\brief     Retrieves the script names associated with a specified entity.
			\param[in] entity The ID of the entity to retrieve script names from.
			\return    An array of script names if found; otherwise, null.
		*/
		/******************************************************************/
		static MonoArray* m_InternalGetScriptNames(ecs::EntityID entity);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalAddScriptInstance(ecs::EntityID entity, MonoString* monoScriptName, MonoObject* instance)
			\brief     Adds a script instance to a specified entity.
			\param[in] entity The ID of the entity to add the script to.
			\param[in] monoScriptName Pointer to the name of the script.
			\param[in] instance Pointer to the script instance to add.
			\return    True if the script instance is added successfully; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalAddScriptInstance(ecs::EntityID entity, MonoString* monoScriptName, MonoObject* instance);

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
			\fn        bool InternalCall::m_InternalCallGetDeltaTime(float* deltatime)
			\brief     Retrieves the fixed delta time.
			\param[out] deltatime Pointer to store the retrieved delta time value.
			\return    Always returns false for consistency in call handling.
		*/
		/******************************************************************/
		static float m_InternalCallGetDeltaTime();

		/******************************************************************/
		/*! \fn        int InternalCall::m_InternalCallGetTagID(MonoString* monostring)
			\brief     Retrieves the ID of the first entity associated with a specific tag.
			\param[in] monostring Pointer to the tag as a MonoString.
			\return    The ID of the first entity associated with the tag if found; otherwise, -1.
		*/
		/******************************************************************/
		static int m_InternalCallGetTagID(MonoString* tag);

		/******************************************************************/
		/*! \fn        MonoArray* InternalCall::m_InternalCallGetTagIDs(MonoString* monostring)
			\brief     Retrieves all entity IDs associated with a specific tag.
			\param[in] monostring Pointer to the tag as a MonoString.
			\return    An array of entity IDs associated with the tag if found; otherwise, null.
		*/
		/******************************************************************/
		static MonoArray* m_InternalCallGetTagIDs(MonoString* tag);

		/******************************************************************/
		/*! \fn        MonoArray* InternalCall::m_InternalCallGetCollidedEntities(ecs::EntityID entity)
			\brief     Retrieves the IDs of entities collided with a specified entity.
			\param[in] entity The ID of the entity to check for collisions.
			\return    An array of entity IDs that the specified entity has collided with if any; otherwise, null.
		*/
		/******************************************************************/
		static MonoArray* m_InternalCallGetCollidedEntities(ecs::EntityID entity);

		/******************************************************************/
		/*! \fn        MonoString* InternalCall::m_InternalCallGetTag(ecs::EntityID entity)
			\brief     Retrieves the tag associated with a specified entity.
			\param[in] entity The ID of the entity to retrieve the tag for.
			\return    A MonoString containing the tag of the specified entity if found; otherwise, null.
		*/
		/******************************************************************/
		static MonoString* m_InternalCallGetTag(ecs::EntityID entity);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallSetSceneActive(MonoString* monoString)
			\brief     Activates the specified scene in the application.
			\param[in] monoString Pointer to the MonoString containing the name of the scene to activate.
		*/
		/******************************************************************/
		static void m_InternalCallSetSceneActive(MonoString* monoString);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_UnloadAllScene()
			\brief     Unloads all currently loaded scenes from the application.
		*/
		/******************************************************************/
		static void m_UnloadAllScene();

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallLoadScene(MonoString* monoString)
			\brief     Loads a specified scene into the application.
			\param[in] monoString Pointer to the MonoString containing the name of the scene to load.
		*/
		/******************************************************************/
		static void m_InternalCallLoadScene(MonoString* monoString);

		/******************************************************************/
		/*!
			\fn        int InternalCall::m_InternalCallAddPrefab(MonoString* prefab, const float* x, const float* y, const float* rotation)
			\brief     Adds a prefab instance at the specified position and rotation.
			\param[in] prefab Pointer to the MonoString containing the prefab name.
			\param[in] x Pointer to the x-coordinate for the prefab's position.
			\param[in] y Pointer to the y-coordinate for the prefab's position.
			\param[in] rotation Pointer to the rotation value for the prefab.
			\return    An integer representing the ID of the newly added prefab instance.
		*/
		/******************************************************************/
		static int m_InternalCallAddPrefab(MonoString* prefab, const float* x, const float* y, const float* rotation);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallDeleteEntity(ecs::EntityID entity)
			\brief     Deletes the specified entity from the ECS system.
			\param[in] entity The ID of the entity to delete.
		*/
		/******************************************************************/
		static void m_InternalCallDeleteEntity(ecs::EntityID entity);

		/******************************************************************/
		/*!
			\fn        float InternalCall::m_InternalCallIsCollided(ecs::EntityID entity)
			\brief     Checks whether the specified entity is involved in a collision.
			\param[in] entity The ID of the entity to check for collisions.
			\return    A float representing collision status: 1.0 for collided, 0.0 otherwise.
		*/
		/******************************************************************/
		static float m_InternalCallIsCollided(ecs::EntityID entity);

		/******************************************************************/
		/*!
			\fn        vector2::Vec2 InternalCall::m_InternalGetMousePosition()
			\brief     Retrieves the current position of the mouse in world coordinates.
			\return    A vector2::Vec2 object containing the mouse's position.
		*/
		/******************************************************************/
		static vector2::Vec2 m_InternalGetMousePosition();


		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalCallIsKeyPressed(keyCode key)
			\brief     Checks if a specific key is currently pressed.
			\param[in] key The key code to check.
			\return    True if the specified key is pressed; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalCallIsKeyPressed(keyCode key);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalCallIsKeyReleased(keyCode key)
			\brief     Checks if a specific key has been released.
			\param[in] key The key code to check.
			\return    True if the specified key has been released; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalCallIsKeyReleased(keyCode key);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalCallIsKeyTriggered(keyCode key)
			\brief     Checks if a specific key has been triggered (pressed and then released).
			\param[in] key The key code to check.
			\return    True if the specified key has been triggered; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalCallIsKeyTriggered(keyCode key);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallSetTimeScale(const float x)
			\brief     Sets the time scale for the application, affecting the speed of time progression.
			\param[in] x The time scale multiplier to set (e.g., 1.0 for normal speed, 0.5 for half speed).
		*/
		/******************************************************************/
		static void m_InternalCallSetTimeScale(const float x);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallResetTimeScale()
			\brief     Resets the time scale to its default value (e.g., 1.0 for normal speed).
		*/
		/******************************************************************/
		static void m_InternalCallResetTimeScale();

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalGetWorldMousePosition(vector2::Vec2* mousecord)
			\brief     Retrieves the mouse position in world coordinates.
			\param[out] mousecord Pointer to store the world coordinates of the mouse.
		*/
		/******************************************************************/
		static void m_InternalGetWorldMousePosition(vector2::Vec2* mousecord);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallCloseWindow()
			\brief     Closes the application window.
		*/
		/******************************************************************/
		static void m_InternalCallCloseWindow();

		/******************************************************************/
		/*!
			\fn        MonoArray* InternalCall::m_InternalCallGetChildrenID(ecs::EntityID id, bool* have_children)
			\brief     Retrieves the IDs of child entities for a specified entity.
			\param[in] id The ID of the parent entity.
			\param[out] have_children Pointer to a boolean indicating whether the entity has children.
			\return    A MonoArray containing the IDs of child entities if any; otherwise, null.
		*/
		/******************************************************************/
		static MonoArray* m_InternalCallGetChildrenID(ecs::EntityID);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallPlayAudio(ecs::EntityID id, MonoString* monoString)
			\brief     Plays the specified audio associated with an entity.
			\param[in] id The ID of the entity playing the audio.
			\param[in] monoString Pointer to the MonoString containing the audio file name.
		*/
		/******************************************************************/
		static void m_InternalCallPlayAudio(ecs::EntityID id, MonoString* monoString);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallStopAudio(ecs::EntityID id, MonoString* monoString)
			\brief     Stops the specified audio associated with an entity.
			\param[in] id The ID of the entity stopping the audio.
			\param[in] monoString Pointer to the MonoString containing the audio file name.
		*/
		/******************************************************************/
		static void m_InternalCallStopAudio(ecs::EntityID id, MonoString* monoString);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallStopAllAudio()
			\brief     Stops all audio currently playing in the application.
		*/
		/******************************************************************/
		static void m_InternalCallStopAllAudio();

		static void m_InternalCallPauseAudio(ecs::EntityID id, MonoString* monoString);

		static void m_InternalCallUnPauseAudio(ecs::EntityID id, MonoString* monoString);

		static void m_InternalCallPauseAllAudio();

		static void m_InternalCallUnPauseAllAudio();

		/******************************************************************/
		/*!
		\fn      static bool m_InternalCallIsWindowMinimise()
		\brief   Checks if the application window is minimized.
		\return  True if the window is minimized; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalCallIsWindowMinimise();

		/******************************************************************/
		/*!
		\fn      static float m_getAccumulatedDeltaTime()
		\brief   Retrieves the accumulated delta time for the application.
		\return  The accumulated delta time as a float.
		*/
		/******************************************************************/
		static float m_getAccumulatedDeltaTime();

		/******************************************************************/
		/*!
		\fn      static int m_InternalCallGetSteps()
		\brief   Retrieves the number of steps taken by the application.
		\return  The number of steps as an integer.
		*/
		/******************************************************************/
		static int m_InternalCallGetSteps();

		static float m_InternalCallGetGameTime();

		static void m_EnableScript(ecs::EntityID id, MonoString* monoString);

		static void m_DisableScript(ecs::EntityID id, MonoString* monoString);

		static MonoArray* m_RetrieveCollidableEntities(MonoString* scene);

		static bool m_InternalCallGetRayCast( ecs::EntityID id,  MonoString* monoString, bool* isRaycasting, vector2::Vec2* targetposition, float* m_distance, bool* targetReached, vector2::Vec2* hitposition);

		static bool m_InternalCallSetRayCast(ecs::EntityID id, MonoString* monoString, bool* isRaycasting, vector2::Vec2* targetposition, float* m_distance, bool* targetReached, vector2::Vec2* hitposition);;

		static bool m_InternalGetGridComponent(ecs::EntityID entity, vector2::Vec2* anchor, int* gridRowLength, int* gridColumnLength, bool* setCollidable, int* gridKey);

		static bool m_InternalSetGridComponent(ecs::EntityID entity, vector2::Vec2* anchor, int* gridRowLength, int* gridColumnLength, bool* setCollidable, int* gridKey);

		static bool m_InternalCallGetPathfinding(ecs::EntityID id, vector2::Vec2* m_startpos, vector2::Vec2* m_startend, int* gridkey);

		static void m_InternalCallSetTargetPathfinding(ecs::EntityID id, vector2::Vec2* m_targetgridposition);

		static void m_InternalCallGetPath(int gridKey, int* startX, int* startY, int* targetX, int* targetY, MonoArray** nodeArray_x, MonoArray** nodeArray_y);

		static void m_EnableLayer(unsigned int layer);

		static void m_DisableLayer(unsigned int layer);

		static float m_GetUnfixedDeltaTie();

		static void m_GetNameComponent(ecs::EntityID id, void** outptr);
		
		static int m_InternalGetEntityIdFromGridKey(int gridkey);

		static void m_ChangeLayer(ecs::EntityID id, unsigned int layerid);

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