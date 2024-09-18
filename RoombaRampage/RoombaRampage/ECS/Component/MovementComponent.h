#ifndef MOVECOM_H
#define MOVECOM_H

#include "Component.h"

class MovementComponent : public Component {

public:
	float Speed{};
	Vector2::Vec2 Direction{};

};

#endif MOVECOM_H
