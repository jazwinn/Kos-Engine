/******************************************************************/
/*!
\file      RenderDebugSystem.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the defination of the RenderDebugSystem
		   class. It passes the debauging data into the graphics 
		   pipeline for it to be rendered.



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "../Config/pch.h"
#include "../ECS.h"

#include "RenderDebugSystem.h"
#include "../Physics/Physics.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Graphics/GraphicsCamera.h"
#include "../ECS/Hierachy.h"

namespace ecs {

	void DebugDrawingSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
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

		m_SystemSignature.set(TYPECOLLIDERCOMPONENT);
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);

	}

	void DebugDrawingSystem::m_Update(const std::string& scene) {

		//ECS* ecs = ECS::m_GetInstance();

		if (m_vecTransformComponentPtr.size() != m_vecColliderComponentPtr.size()) {
			LOGGING_ERROR("Error: Vecotrs container size does not Match");
			return;
		}
	

		graphicpipe::GraphicsPipe * graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
		for (int n{}; n < m_vecTransformComponentPtr.size(); n++) {

			TransformComponent* transform = m_vecTransformComponentPtr[n];
			ColliderComponent* collider = m_vecColliderComponentPtr[n];
			//skip component not of the scene
			if (collider->m_scene != scene) continue;

			if (transform->m_haveParent) {
				EntityID parentID = ecs::Hierachy::m_GetParent(transform->m_Entity).value();
				TransformComponent* parentComp{ nullptr };
				ColliderComponent* parentCollider{ nullptr };
				for (auto& com : m_vecTransformComponentPtr) {
					if (com->m_Entity == parentID) {
						parentComp = com;
					}
				}
				for (auto& com : m_vecColliderComponentPtr) {
					if (com->m_Entity == parentID) {
						parentCollider = com;
					}
				}

				if (!parentComp) continue;
				if (!parentCollider) continue;

				vector2::Vec2 pos{}, scale{};
				float rot{};

				mat3x3::Mat3x3 parent_Transform = mat3x3::Mat3Transform(parentComp->m_position, parentComp->m_scale, parentComp->m_rotation);
				mat3x3::Mat3x3 child_Transform = mat3x3::Mat3Transform(transform->m_position + collider->m_OffSet, transform->m_scale * collider->m_Size, transform->m_rotation);
				mat3x3::Mat3x3 final_Transform = parent_Transform * child_Transform;
				mat3x3::Mat3Decompose(final_Transform, pos, scale, rot); //child 

				mat3x3::Mat3x3 parent_Scale{}, parent_Rot{}, parent_Trans{}, c_Scale{}, c_Scale_collider{}, c_Rot{}, c_Trans{};
				mat3x3::Mat3Scale(parent_Scale, parentComp->m_scale.m_x * parentCollider->m_Size.m_x, parentComp->m_scale.m_y * parentCollider->m_Size.m_y);
				mat3x3::Mat3RotDeg(parent_Rot, parentComp->m_rotation);
				mat3x3::Mat3Translate(parent_Trans, parentComp->m_position.m_x, parentComp->m_position.m_y);

				mat3x3::Mat3Scale(c_Scale, transform->m_scale.m_x * collider->m_Size.m_x, transform->m_scale.m_y * collider->m_Size.m_y);
				mat3x3::Mat3Scale(c_Scale_collider, collider->m_Size.m_x, collider->m_Size.m_y);
				mat3x3::Mat3RotDeg(c_Rot, transform->m_rotation);
				mat3x3::Mat3Translate(c_Trans, transform->m_position.m_x, transform->m_position.m_y);
				mat3x3::Mat3x3 final_Transforms = c_Trans + parent_Trans;


				vector2::Vec2 child_Pos{}, child_Scale{};
				float child_Rot{};
				mat3x3::Mat3Decompose(transform->m_transformation, child_Pos, child_Scale, child_Rot);

				if (collider->m_drawDebug && (collider->m_type == physicspipe::EntityType::RECTANGLE))
				{
					//mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(vector2::Vec2{ pos.m_x + collider->m_OffSet.m_x, pos.m_y + collider->m_OffSet.m_y }, scale, rot);
					//mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(vector2::Vec2{ transform->m_transformation.m_e20 + collider->m_OffSet.m_x, transform->m_transformation.m_e21 + collider->m_OffSet.m_y },child_Scale, child_Rot);
					mat3x3::Mat3x3 debugTransformation = final_Transform;

					graphicsPipe->m_debugBoxData.push_back({ glm::mat3{debugTransformation.m_e00,debugTransformation.m_e01,debugTransformation.m_e02,
																	debugTransformation.m_e10,debugTransformation.m_e11, debugTransformation.m_e12,
																debugTransformation.m_e20, debugTransformation.m_e21, debugTransformation.m_e22} ,
															collider->m_isCollided, static_cast<graphicpipe::GraphicsPipe::ShapeType>(collider->m_type) });
				}
				else if (collider->m_drawDebug && (collider->m_type == physicspipe::EntityType::CIRCLE))
				{
					mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(vector2::Vec2{ transform->m_transformation.m_e20 + collider->m_OffSet.m_x, transform->m_transformation.m_e21 + collider->m_OffSet.m_y }, vector2::Vec2{ collider->m_radius * 2.f, collider->m_radius * 2.f }, parentComp->m_rotation);

					graphicsPipe->m_debugBoxData.push_back({ glm::mat3{debugTransformation.m_e00,debugTransformation.m_e01,debugTransformation.m_e02,
																	debugTransformation.m_e10,debugTransformation.m_e11, debugTransformation.m_e12,
																debugTransformation.m_e20, debugTransformation.m_e21, debugTransformation.m_e22} ,
															collider->m_isCollided, static_cast<graphicpipe::GraphicsPipe::ShapeType>(collider->m_type) });
				}
			}
			else {
				if (collider->m_drawDebug && (collider->m_type == physicspipe::EntityType::RECTANGLE))
				{
					mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(vector2::Vec2{ transform->m_transformation.m_e20 + collider->m_OffSet.m_x, transform->m_transformation.m_e21 + collider->m_OffSet.m_y }, collider->m_Size * transform->m_scale, transform->m_rotation);

					graphicsPipe->m_debugBoxData.push_back({ glm::mat3{debugTransformation.m_e00,debugTransformation.m_e01,debugTransformation.m_e02,
																	debugTransformation.m_e10,debugTransformation.m_e11, debugTransformation.m_e12,
																debugTransformation.m_e20, debugTransformation.m_e21, debugTransformation.m_e22} ,
															collider->m_isCollided, static_cast<graphicpipe::GraphicsPipe::ShapeType>(collider->m_type) });
				}
				else if (collider->m_drawDebug && (collider->m_type == physicspipe::EntityType::CIRCLE))
				{
					mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(vector2::Vec2{ transform->m_transformation.m_e20 + collider->m_OffSet.m_x, transform->m_transformation.m_e21 + collider->m_OffSet.m_y }, vector2::Vec2{ collider->m_radius * 2.f, collider->m_radius * 2.f } * transform->m_scale, transform->m_rotation);

					graphicsPipe->m_debugBoxData.push_back({ glm::mat3{debugTransformation.m_e00,debugTransformation.m_e01,debugTransformation.m_e02,
																	debugTransformation.m_e10,debugTransformation.m_e11, debugTransformation.m_e12,
																debugTransformation.m_e20, debugTransformation.m_e21, debugTransformation.m_e22} ,
															collider->m_isCollided, static_cast<graphicpipe::GraphicsPipe::ShapeType>(collider->m_type) });
				}
			}

			
		}
	}


}
