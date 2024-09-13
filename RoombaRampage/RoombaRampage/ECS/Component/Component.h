#ifndef COM_H
#define COM_H

enum ComponentType {
	TypeTransformComponent,
	TypeMovemmentComponent,
	TypeSpriteComponent,


	TotalTypeComponent
};

//parent class
class Component {

public:

	//Entity is assigned to component
	bool IsLive{};

	//Store Entity ID
	EntityID Entity;

};




#endif COM_H