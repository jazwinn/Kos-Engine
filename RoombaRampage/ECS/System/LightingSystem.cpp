#include "../Config/pch.h"

#include "../ECS.h"

#include "../ECS/System/LightingSystem.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Asset Manager/AssetManager.h"

namespace ecs {

	void LightingSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecLightingComponentPtr.push_back((LightingComponent*)ecs->m_ECS_CombinedComponentPool[TYPELIGHTINGCOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}

	}

	void LightingSystem::m_DeregisterSystem(EntityID ID) {

		//search element location for the entity
		size_t IndexID{};
		for (auto& LightingComponentPtr : m_vecLightingComponentPtr) {
			if (LightingComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecLightingComponentPtr.size() - 1;
		std::swap(m_vecLightingComponentPtr[IndexID], m_vecLightingComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);

		//popback the vector;
		m_vecLightingComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void LightingSystem::m_Init()
	{
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
		m_SystemSignature.set(TYPELIGHTINGCOMPONENT);
	}

	void LightingSystem::m_Update(const std::string& scene)
	{
		ECS* ecs = ECS::m_GetInstance();
		graphicpipe::GraphicsPipe* graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
		//assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();

		if (m_vecLightingComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			LOGGING_ERROR("Error: Vectors container size does not Match");
			return;
		}

		//loops through all vecoters pointing to component
		for (int n{}; n < m_vecLightingComponentPtr.size(); n++) 
		{
			TransformComponent* transform = m_vecTransformComponentPtr[n];
			LightingComponent* light = m_vecLightingComponentPtr[n];
			NameComponent* nc = m_vecNameComponentPtr[n];
			//skip component not of the scene
			if ((light->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(nc->m_Layer)) continue;

			//Clamp between 0.f and 1.f
			light->m_innerOuterRadius.m_x = light->m_innerOuterRadius.m_x > 1.f ? 1.f : light->m_innerOuterRadius.m_x < 0.f ? 0.f : light->m_innerOuterRadius.m_x;

			light->m_innerOuterRadius.m_y = light->m_innerOuterRadius.m_y > 1.f ? 1.f : light->m_innerOuterRadius.m_y < 0.f ? 0.f : light->m_innerOuterRadius.m_y;

			graphicsPipe->m_lightingData.push_back({ { transform->m_transformation.m_e00,transform->m_transformation.m_e01,transform->m_transformation.m_e02,
													   transform->m_transformation.m_e10,transform->m_transformation.m_e11, transform->m_transformation.m_e12,
													   transform->m_transformation.m_e20, transform->m_transformation.m_e21, transform->m_transformation.m_e22 },
													{  light->m_colour.m_x,light->m_colour.m_y,light->m_colour.m_z ,1.f }, { light->m_innerOuterRadius.m_x ,light->m_innerOuterRadius.m_y },
													   light->m_intensity, light->m_lightType });

		}


	}


}

