#ifndef SYSTEM_H
#define SYSTEM_H

enum TypeSystem {
	TypeMovementSystem,


	TotalTypeSystem
};

class ISystem {

public:

	virtual void Init() = 0;

	virtual void Update() = 0;


};







#endif SYSTEM_H

