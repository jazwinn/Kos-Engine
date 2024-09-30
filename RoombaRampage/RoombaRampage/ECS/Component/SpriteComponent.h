#ifndef SPRITE_H
#define SPRITE_H
#include <string>

#include "Component.h"

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
		int layer

	};

}




#endif SPRITE_H
