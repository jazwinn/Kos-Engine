#ifndef ECSLIST_H
#define ECSLIST_H
#include <bitset>
#include "../ECS/Component/ComponentList.h"

namespace Ecs {

	using EntityID = unsigned int;
	using compSignature = std::bitset<TotalTypeComponent>;

	// Max number of entities allowed to be created
	static EntityID MaxEntity = 3000;

}



#endif ECSLIST_H