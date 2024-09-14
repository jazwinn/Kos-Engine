#ifndef SPRITE_H
#define SPRITE_H
#include <string>

#include "Component.h"

enum SpriteShape {
	TRIANGLE,
	RECTANGLE,
	CIRCLE,
	TRIANGLE_LINES,
	RECTANGLE_LINES,
	CIRCLE_LINES
};

class SpriteComponent : public Component {

public:
	SpriteShape Shape{};
	std::string texture{};
	//colour?
};


#endif SPRITE_H
