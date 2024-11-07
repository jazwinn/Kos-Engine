#include "../Config/pch.h"
#include "Layers.h"
#include "../Debugging/Logging.h"
#include "../ECS/ECS.h"

namespace layer {


	//bool LayerStack::m_CreateLayer(std::string layer)
	//{
	//	// check if layer is already existing
	//	if (m_layerMap.find(layer) != m_layerMap.end()) {
	//		LOGGING_WARN("Layer of same name has already been created");
	//		return false;
	//	}

	//	//creates new layer
	//	m_layerMap[layer];


	//	return true;
	//}

	//bool LayerStack::m_DeleteLayer(std::string layer)
	//{
	//	ecs::ECS* ecs = ecs::ECS::m_GetInstance();
	//	// check if layer is already existing
	//	if (m_layerMap.find(layer) == m_layerMap.end()) {
	//		LOGGING_WARN("Layer does no exist");
	//		return false;
	//	}

	//	for (ecs::EntityID& x : m_layerMap[layer]) {
	//		static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(x))->Layer = "Default";
	//	
	//	}

	//	return false;
	//}

	LayerStack::LayerStack()
	{
		//set max layers
		m_layerMap[DEFAULT].first = "Default";
		m_layerMap[LAYER1].first = "Layer 1";
		m_layerMap[LAYER2].first = "Layer 2";
		m_layerMap[LAYER3].first = "Layer 3";
		m_layerMap[LAYER4].first = "Layer 4";
		m_layerMap[LAYER5].first = "Layer 5";
		m_layerMap[LAYER6].first = "Layer 6";
		m_layerMap[LAYER7].first = "Layer 7";
		m_layerMap[LAYER8].first = "Layer 8";

	}

	void LayerStack::m_ChangeLayerName(LAYERS layer, std::string newName)
	{
		if (m_layerMap.find(layer) == m_layerMap.end()) {

			LOGGING_WARN("Layer does no exist");
			return ;
		}

		m_layerMap[layer].first = newName;

	}

	bool LayerStack::m_SwapEntityLayer(LAYERS newlayer, LAYERS oldlayer, ecs::EntityID id)
	{
		// same layer, do nothing
		if (newlayer == oldlayer) return true;

		if (m_layerMap.find(newlayer) == m_layerMap.end()) {
			
			LOGGING_WARN("Layer does no exist");
			return false;
		}

		if (m_layerMap.find(oldlayer) == m_layerMap.end()) {

			LOGGING_WARN("Layer does no exist");
			return false;
		}

		// add id to new layer
		m_layerMap[newlayer].second.push_back(id);
		m_layerMap[oldlayer].second.erase(std::find(m_layerMap[oldlayer].second.begin(), m_layerMap[oldlayer].second.end(), id));

		//assign ecs layer
		ecs::NameComponent* nc = (ecs::NameComponent*)(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));
		nc->m_Layer = newlayer;

		return true;
	}

	std::vector<ecs::EntityID> LayerStack::m_RetrieveEntityID(LAYERS layer)
	{
		if (m_layerMap.find(layer) == m_layerMap.end()) {

			LOGGING_WARN("Layer does no exist");
			return std::vector<ecs::EntityID>();
		}

		return m_layerMap[layer].second;
	}


}