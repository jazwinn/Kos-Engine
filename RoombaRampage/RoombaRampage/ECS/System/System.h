#ifndef SYSTEM_H
#define SYSTEM_H

#include <bitset>
#include <vector>
#include "../ECS/Component/Component.h"


enum TypeSystem {
	TypeMovementSystem,


	TotalTypeSystem
};

class ISystem {

public:

	std::bitset<TotalTypeComponent> SystemSignature;

	virtual void RegisterSystem(EntityID) = 0;

	virtual void Init() = 0;

	virtual void Update() = 0;


};







#endif SYSTEM_H

