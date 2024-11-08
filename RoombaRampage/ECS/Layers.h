/******************************************************************/
/*!
\file      Layers.h
\author    Clarence Boey
\par       c.boey@digipen.edu
\date	   8 Nov, 2024
\brief     This file contains the declaration for the Layer.


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

		MAXLAYER
	};

	class LayerStack {
	
	public:


		using layermap = std::unordered_map<LAYERS, std::pair<std::string,std::vector<ecs::EntityID>>>;

		//constructor
		LayerStack();

		//bool m_CreateLayer(layer);

		//bool m_DeleteLayer(layer);

		void m_ChangeLayerName(LAYERS layer, std::string newName);

		bool m_SwapEntityLayer(LAYERS newlayer, LAYERS oldlayer, ecs::EntityID);

		std::vector<ecs::EntityID> m_RetrieveEntityID(LAYERS layer);

	public:
		layermap m_layerMap;


	};


}



#endif LAYERS_H