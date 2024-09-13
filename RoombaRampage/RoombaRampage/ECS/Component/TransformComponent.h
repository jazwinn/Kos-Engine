#ifndef TRANSCOM_H
#define TRANSCOM_H

#include "Component.h"
#include "../Math/vector2.h"

class TransformComponent : public Component {

public:
	Vec2 position{};
	float roation{};
	float scale{};
};

#endif TRANSCOM_H