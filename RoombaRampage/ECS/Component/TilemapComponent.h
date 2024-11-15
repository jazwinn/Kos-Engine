#ifndef TILEMAP_H
#define TILEMAP_H

#include "Component.h"
#include <vector>


namespace ecs {

	class TilemapComponent : public Component {

	public:

		std::string m_tilemapFile{};
		int m_tileLayer{};
		vector3::Vec3 m_color{ 1.f,1.f,1.f };
		float m_alpha{ 1.f };
		int m_tileIndex{-100};
		int m_rowLength{1};
		int m_columnLength{1};
		int m_pictureRows{ 1 };
		int m_pictureColumns{ 1 };
		std::vector<std::vector<int>> m_tilePictureIndex;


		REFLECTABLE(TilemapComponent, m_tileIndex,m_tileLayer, m_rowLength, m_columnLength, m_pictureRows, m_pictureColumns);
	};

}




#endif TILEMAP_H
