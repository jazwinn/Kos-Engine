#ifndef MOVECOM_H
#define MOVECOM_H

#include "Component.h"

class MovementComponent : public Component {

public:
	float Speed{};
	float velocity{};
	float resistance;
};

#endif MOVECOM_H
