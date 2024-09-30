#ifndef SYSTEM_H
#define SYSTEM_H

#include <bitset>
#include <vector>
#include <algorithm>
#include "../ECS/Component/Component.h"
#include "SystemType.h"

namespace ecs {

	class ISystem {

	public:

		compSignature m_SystemSignature;

		virtual void m_RegisterSystem(EntityID) = 0;

		virtual void m_DeregisterSystem(EntityID) = 0;

		virtual void m_Init() = 0;

		virtual void m_Update() = 0;


	};

}









#endif SYSTEM_H

