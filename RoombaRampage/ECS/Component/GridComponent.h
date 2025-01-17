#pragma once
#ifndef GRIDCOM_H
#define GRIDCOM_H

#include "Component.h"
#include <vector>

namespace ecs {

	class GridComponent : public Component {

	public:

		int m_GridRowLength{ 1 };
		int m_GridColumnLength{ 1 };
		bool m_SetCollidable{ true };
		std::vector<std::vector<int>> m_IsWall;

		REFLECTABLE(GridComponent, m_GridRowLength, m_GridColumnLength, m_SetCollidable)
	};
}

#endif GRIDCOM_H
