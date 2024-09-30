#include "../ECS.h"

#include "DebugDrawingSystem.h"
#include "../Physics/Physics.h"
#include "../Graphics/GraphicsPipe.h"

namespace ecs {

	void DebugDrawingSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecColliderComponentPtr.begin(), m_vecColliderComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecColliderComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecColliderComponentPtr.push_back((ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(ID));
		}
	}

	void DebugDrawingSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecColliderComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecColliderComponentPtr.size() - 1;

		std::swap(m_vecColliderComponentPtr[IndexID], m_vecColliderComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);

		//popback the vector;
		m_vecColliderComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
	}

	void DebugDrawingSystem::m_Init() {

		// requires both collider component and transform component
		m_SystemSignature.set(TYPECOLLIDERCOMPONENT);
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
		//SystemSignature.set();

	}

	void DebugDrawingSystem::m_Update() {

		ECS* ecs = ECS::m_GetInstance();

		if (m_vecTransformComponentPtr.size() != m_vecColliderComponentPtr.size()) {
			LOGGING_ERROR("Error: Vecotrs container size does not Match");
			return;
		}
	
	

		GraphicsPipe* graphicsPipe = GraphicsPipe::funcGetInstance();
		for (int n{}; n < m_vecColliderComponentPtr.size(); n++) {

			TransformComponent* transform = m_vecTransformComponentPtr[n];
			ColliderComponent* collider = m_vecColliderComponentPtr[n];

			if (collider->m_drawDebug)
			{
				graphicsPipe->debugBoxData.push_back({ 0, glm::vec2{collider->m_Size.m_x * transform->m_scale.m_x, collider->m_Size.m_y * transform->m_scale.m_y}, glm::vec3{transform->m_position.m_x + collider->m_OffSet.m_x,transform->m_position.m_y + collider->m_OffSet.m_y, 0} ,collider->m_isCollided, 0 });
			}

		}
	}


}
