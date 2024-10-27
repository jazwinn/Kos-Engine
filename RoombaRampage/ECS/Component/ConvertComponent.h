#pragma once

#include "ComponentPool.h"

#include <variant>

namespace ecs {

	using ComponentVariant = std::variant<
		std::monostate,
		TransformComponent*,
		AnimationComponent*,
		CameraComponent*,
		ColliderComponent*,
		RigidBodyComponent*,
		ScriptComponent*,
		TextComponent*,
		PlayerComponent*,
		SpriteComponent*
	>;

	class ConvertComponent {
	public:
		template<typename T, int id>
		T* Convert(void*)
		{
			return nullptr;
		}

		template<>
		TransformComponent* Convert<TransformComponent, TYPETRANSFORMCOMPONENT>(void* val)
		{
			return static_cast<TransformComponent*>(val);
		}

		template<>
		AnimationComponent* Convert<AnimationComponent, TYPEANIMATIONCOMPONENT>(void* val)
		{
			return static_cast<AnimationComponent*>(val);
		}

		template<>
		CameraComponent* Convert<CameraComponent, TYPECAMERACOMPONENT>(void* val)
		{
			return static_cast<CameraComponent*>(val);
		}

		template<>
		ColliderComponent* Convert<ColliderComponent, TYPECOLLIDERCOMPONENT>(void* val)
		{
			return static_cast<ColliderComponent*>(val);
		}

		template<>
		RigidBodyComponent* Convert<RigidBodyComponent, TYPERIGIDBODYCOMPONENT>(void* val)
		{
			return static_cast<RigidBodyComponent*>(val);
		}

		template<>
		ScriptComponent* Convert<ScriptComponent, TYPESCRIPTCOMPONENT>(void* val)
		{
			return static_cast<ScriptComponent*>(val);
		}

		template<>
		TextComponent* Convert<TextComponent, TYPETEXTCOMPONENT>(void* val)
		{
			return static_cast<TextComponent*>(val);
		}

		template<>
		PlayerComponent* Convert<PlayerComponent, TYPEPLAYERCOMPONENT>(void* val)
		{
			return static_cast<PlayerComponent*>(val);
		}

		template<>
		SpriteComponent* Convert<SpriteComponent, TYPESPRITECOMPONENT>(void* val)
		{
			return static_cast<SpriteComponent*>(val);
		}

		static ComponentVariant DispatchConvert(int id, void* val) {
			switch (id) {
			case TYPETRANSFORMCOMPONENT:
				return reinterpret_cast<TransformComponent*>(val);
			case TYPEANIMATIONCOMPONENT:
				return reinterpret_cast<AnimationComponent*>(val);
			case TYPECAMERACOMPONENT:
				return reinterpret_cast<CameraComponent*>(val);
			case TYPECOLLIDERCOMPONENT:
				return reinterpret_cast<ColliderComponent*>(val);
			case TYPERIGIDBODYCOMPONENT:
				return reinterpret_cast<RigidBodyComponent*>(val);
			case TYPESCRIPTCOMPONENT:
				return reinterpret_cast<ScriptComponent*>(val);
			case TYPETEXTCOMPONENT:
				return reinterpret_cast<TextComponent*>(val);
			case TYPEPLAYERCOMPONENT:
				return reinterpret_cast<PlayerComponent*>(val);
			case TYPESPRITECOMPONENT:
				return reinterpret_cast<SpriteComponent*>(val);
			default:
				return std::monostate{}; // Return an empty state for unsupported types
			}
		}
	};



}