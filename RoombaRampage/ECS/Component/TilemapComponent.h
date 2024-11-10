#ifndef TILEMAP_H
#define TILEMAP_H

#include "Component.h"

namespace ecs {

	class TilemapComponent : public Component {

	public:

		std::string m_tilemapFile{};
		int m_tileLayer{};
		vector3::Vec3 m_color{ 1.f,1.f,1.f };
		float m_alpha{ 1.f };
		int m_tileIndex{};
		int m_rows{1};
		int m_columns{1};
		int m_pictureRows{ 1 };
		int m_pictureColumns{ 1 };


		REFLECTABLE(TilemapComponent, m_tileIndex,m_tileLayer, m_rows, m_columns, m_pictureRows, m_pictureColumns);
	};

}




#endif TILEMAP_H
