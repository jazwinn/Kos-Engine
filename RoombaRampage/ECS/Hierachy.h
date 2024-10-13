#ifndef HIERACHY_H

#include "ECS.h"

namespace ecs {


	class Hierachy {

	public:
		static void m_SetParent(EntityID parent, EntityID child);

		static void m_RemoveParent(EntityID child);

		static std::optional<EntityID> m_GetParent(EntityID child);

		static std::optional<std::vector<EntityID>> m_GetChild(EntityID parent);

	};

}



#define HIERACHY_H
#endif HIREACHY_H

