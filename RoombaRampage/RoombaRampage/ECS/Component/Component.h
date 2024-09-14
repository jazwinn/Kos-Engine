#ifndef COM_H
#define COM_H

#include "../ECS/Component/ComponentList.h"

//parent class
class Component {

public:

	//Entity is assigned to component
	bool IsLive{};

	//Store Entity ID
	EntityID Entity{};

};




#endif COM_H