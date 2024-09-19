#ifndef SPRITE_H
#define SPRITE_H
#include <string>

#include "Component.h"
#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
//#include "../ECS/System/RenderSystem.h"

namespace Ecs {

	enum SpriteShape {
		RECTANGLE,
		CIRCLE,
		RECTANGLE_LINES,
		CIRCLE_LINES
	};

	class SpriteComponent : public Component {

	public:

		unsigned int imageID;
		unsigned int frameNumber;

	};

}




#endif SPRITE_H
