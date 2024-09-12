#ifndef COM_H
#define COM_H

enum ComponentType {
	TypeTransformComponent,
	TypeMovemmentComponent,


	TotalTypeComponent
};

//parent class
class Component {

public:
	// component is storing data
	bool IsStored{};

	//Entity is assigned to component
	bool IsLive{};

	//Store Entity ID
	EntityID Entity;

};




#endif COM_H