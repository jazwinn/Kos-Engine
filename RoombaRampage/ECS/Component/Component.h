#ifndef COM_H
#define COM_H

#include "../ECS/Component/ComponentList.h"
#include "../ECS/ECSList.h"
#include "../Dependencies/rapidjson/document.h"  // Include rapidjson for serialization


namespace ecs {
	class Component {

	public:

		//Entity is assigned to components
		bool m_IsLive{};

		//Store Entity ID
		EntityID m_Entity{};
	};

}
//parent class

#endif COM_H