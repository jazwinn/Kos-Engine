/******************************************************************/
/*!
\file      Layers.h
\author    Clarence Boey
\par       c.boey@digipen.edu
\date	   8 Nov, 2024
\brief     This header file declares the `LayerStack` class and associated
		   enums for handling layer management in the ECS framework.
		   - LayerStack: Manages named layers that contain entities.
		   - m_ChangeLayerName: Renames a specified layer.
		   - m_SwapEntityLayer: Moves an entity from one layer to another.
		   - m_RetrieveEntityID: Retrieves all entity IDs within a specified layer.

This file allows flexible layer-based organization of entities in the ECS system,
providing functionality for managing multiple layers for scene composition.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef LAYERS_H
#define LAYERS_H

#include "ECSList.h"

namespace layer{

	enum LAYERS {
		DEFAULT,
		LAYER1,
		LAYER2,
		LAYER3,
		LAYER4,
		LAYER5,
		LAYER6,
		LAYER7,
		LAYER8,
		LAYER9,
		LAYER10,
		LAYER11,
		LAYER12,
		LAYER13,
		LAYER14,
		LAYER15,
		LAYER16,

		MAXLAYER
	};

	class LayerStack {
	
	public:


		using layermap = std::unordered_map<LAYERS, std::pair<std::string,std::vector<ecs::EntityID>>>;

		/******************************************************************/
		/*!
			\fn        LayerStack::LayerStack()
			\brief     Constructs the `LayerStack` and initializes the default layers.
			\details   Sets up predefined layers (DEFAULT through LAYER8) with default names,
					   creating an organized starting point for layer management.
		*/
		/******************************************************************/
		LayerStack();// set all bits to true

		/******************************************************************/
		/*!
			\fn        void m_ChangeLayerName(LAYERS layer, std::string newName)
			\brief     Renames a specified layer.
			\param[in] layer   The enum value of the layer to rename.
			\param[in] newName The new name for the layer.
			\details   Updates the specified layer�s name, if the layer exists.
		*/
		/******************************************************************/
		void m_ChangeLayerName(LAYERS layer, std::string newName);

		/******************************************************************/
		/*!
			\fn        bool m_SwapEntityLayer(LAYERS newlayer, LAYERS oldlayer, ecs::EntityID id)
			\brief     Moves an entity from one layer to another.
			\param[in] newlayer The layer to move the entity to.
			\param[in] oldlayer The layer from which to move the entity.
			\param[in] id       The ID of the entity to move.
			\return    True if the entity was successfully moved; otherwise, false.
			\details   Transfers the entity ID between layers and updates its layer attribute.
		*/
		/******************************************************************/
		bool m_SwapEntityLayer(LAYERS newlayer, LAYERS oldlayer, ecs::EntityID);

		void m_DisableLayer(LAYERS layer);
		void m_EnableLayer(LAYERS layer);

		/******************************************************************/
		/*!
			\fn        std::vector<ecs::EntityID> m_RetrieveEntityID(LAYERS layer)
			\brief     Retrieves all entity IDs within a specified layer.
			\param[in] layer The layer to retrieve entities from.
			\return    A vector containing the entity IDs in the specified layer, or an empty
					   vector if the layer does not exist.
			\details   Allows access to all entities within a specific layer.
		*/
		/******************************************************************/
		std::vector<ecs::EntityID> m_RetrieveEntityID(LAYERS layer);

		//bool m_CreateLayer(layer);

		//bool m_DeleteLayer(layer);

	public:
		layermap m_layerMap;

		// layers that are activated
		std::bitset<MAXLAYER> m_layerBitSet;
	};
}
#endif LAYERS_H