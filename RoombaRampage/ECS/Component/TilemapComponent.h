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
		int m_tileIndex{0};
		int m_rowLength{1};
		int m_columnLength{1};
		int m_pictureRowLength{ 1 };
		int m_pictureColumnLength{ 1 };
		std::vector<std::vector<int>> m_tilePictureIndex;

		bool m_setCollidable{ true };
		std::vector<std::vector<int>> m_isWall;

		REFLECTABLE(TilemapComponent, m_tileIndex,m_tileLayer, m_rowLength, m_columnLength, m_pictureRowLength, m_pictureColumnLength);
	};

}




#endif TILEMAP_H
