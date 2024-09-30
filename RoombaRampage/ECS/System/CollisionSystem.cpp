#include "../ECS.h"

#include "CollisionSystem.h"
#include "../Physics/Physics.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Debugging/Logging.h"

namespace ecs {

	void CollisionSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecColliderComponentPtr.push_back((ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(ID));
			//vecRigidBodyComponentPtr.push_back((RigidBodyComponent*)ecs->ECS_CombinedComponentPool[TypeRigidBodyComponent]->GetEntityComponent(ID));
			//m_vecMovementComponentPtr.push_back((MovementComponent*)ecs->m_ECS_CombinedComponentPool[TYPEMOVEMENTCOMPONENT]->m_GetEntityComponent(ID));
		}
	}

	void CollisionSystem::m_DeregisterSystem(EntityID ID) {
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
		//std::swap(vecRigidBodyComponentPtr[IndexID], vecRigidBodyComponentPtr[IndexLast]);
		//std::swap(m_vecMovementComponentPtr[IndexID], m_vecMovementComponentPtr[IndexLast]);

		//popback the vector;
		m_vecColliderComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		//vecRigidBodyComponentPtr.pop_back();
		//m_vecMovementComponentPtr.pop_back();
	}

	void CollisionSystem::m_Init() {

		// requires both movement component and transform component
		m_SystemSignature.set(TYPECOLLIDERCOMPONENT);
		//m_SystemSignature.set(TYPEMOVEMENTCOMPONENT);

	}

	void CollisionSystem::m_Update() {

		ECS* ecs = ECS::m_GetInstance();

		if (m_vecColliderComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			//std::cout << "Error: Vecotrs container size does not Match" << std::endl;
			LOGGING_ERROR("Error: Vecotrs container size does not Match");
			return;
		}

		//create physics;
		physicspipe::Physics PhysicsPipeline;
		GraphicsPipe* graphicsPipe = GraphicsPipe::funcGetInstance();

		for (int n{}; n < m_vecTransformComponentPtr.size(); n++) {
			//std::cout << "Entity: " << n << "Movement System is getting Updated";

			ColliderComponent* ColComp = m_vecColliderComponentPtr[n];
			TransformComponent* TransComp = m_vecTransformComponentPtr[n];

			EntityID id = ColComp->m_Entity;

			//if movement component is present, do dynamic collision
			vector2::Vec2 velocity{};
			if (ecs->m_ECS_EntityMap[id].test(TYPEMOVEMENTCOMPONENT)) {

				MovementComponent* MovComp = static_cast<MovementComponent*>(ecs->m_ECS_CombinedComponentPool[TYPEMOVEMENTCOMPONENT]->m_GetEntityComponent(id));

				 velocity = MovComp->m_Speed * MovComp->m_Direction;
			}

			if (ColComp->m_type == physicspipe::EntityType::CIRCLE) {
				PhysicsPipeline.m_SendPhysicsData(ColComp->m_radius, TransComp->m_position, ColComp->m_Size * TransComp->m_scale, velocity, id);
			}
			else if (ColComp->m_type == physicspipe::EntityType::RECTANGLE) {
				PhysicsPipeline.m_SendPhysicsData(ColComp->m_Size.m_x, ColComp->m_Size.m_x, TransComp->m_position, ColComp->m_Size * TransComp->m_scale, velocity, id);
			}
			else {
				LOGGING_ERROR("NO ENTITY TYPE");
			}

			if (ColComp->m_drawDebug)
			{
				graphicsPipe->debugBoxData.push_back({ 0, glm::vec2{ColComp->m_Size.m_x * TransComp->m_scale.m_x, ColComp->m_Size.m_y * TransComp->m_scale.m_y}, glm::vec3{TransComp->m_position.m_x + ColComp->m_OffSet.m_x,TransComp->m_position.m_y + ColComp->m_OffSet.m_y, 0} ,0, 0 });
			}
		}

		//check for collision
		if (m_vecColliderComponentPtr.size() > 0) {
			PhysicsPipeline.m_CollisionCheck(ecs->m_DeltaTime);
		}
	
	}
		

}
