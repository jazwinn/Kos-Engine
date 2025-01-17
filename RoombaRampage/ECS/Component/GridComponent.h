#pragma once
#ifndef GRIDCOM_H
#define GRIDCOM_H

#include "Component.h"
#include <vector>

namespace ecs {

	class GridComponent : public Component {

	public:

		int m_GridRow{ 1 };
		int m_GridColumn{ 1 };
		bool m_IsCollidable{ true };
		std::vector<std::vector<int>> m_IsWall;

		REFLECTABLE(GridComponent, m_GridRow, m_GridColumn, m_IsCollidable)
	};
}

#endif GRIDCOM_H
