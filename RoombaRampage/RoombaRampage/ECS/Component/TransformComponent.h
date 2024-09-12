#ifndef TRANSCOM_H
#define TRANSCOM_H

#include "Component.h"

class TransformComponent : public Component {

public:
	float position{};
	float roation{};
	float scale{};
};

#endif TRANSCOM_H