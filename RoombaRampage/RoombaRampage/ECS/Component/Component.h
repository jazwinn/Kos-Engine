#ifndef COM_H
#define COM_H

#include "../ECS/Component/ComponentList.h"


namespace Ecs {
	class Component {

	public:

		//Entity is assigned to component
		bool IsLive{};

		//Store Entity ID
		EntityID Entity{};

	};

}
//parent class





#endif COM_H