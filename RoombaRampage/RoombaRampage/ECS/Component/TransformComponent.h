#ifndef TRANSCOM_H
#define TRANSCOM_H

#include "Component.h"
#include "../Math/vector2.h"

class TransformComponent : public Component {

public:

	Vector2::Vec2 position{};
	float rotation{};
	float scale{};
};

#endif TRANSCOM_H