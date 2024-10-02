#ifndef SPRITE_H
#define SPRITE_H
#include <string>

#include "Component.h"


namespace ecs {

	enum SpriteShape {
		RECTANGLE,
		CIRCLE,
		RECTANGLE_LINES,
		CIRCLE_LINES
	};

	class SpriteComponent : public Component {

	public:

		unsigned int m_imageID;
		unsigned int m_frameNumber;
		float m_frameTimer;
		int layer;
		bool m_isAnimating{ true };
	};

}




#endif SPRITE_H
