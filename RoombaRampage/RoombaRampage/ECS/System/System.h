#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>

enum TypeSystem {
	TypeMovementSystem,


	TotalTypeSystem
};

class ISystem {

public:

	//virtual void RegisterSystem;

	virtual void Init() = 0;

	virtual void Update() = 0;


};







#endif SYSTEM_H

