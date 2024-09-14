#ifndef SPRITE_H
#define SPRITE_H
#include <string>

#include "Component.h"
#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
//#include "../ECS/System/RenderSystem.h"

enum SpriteShape {
	RECTANGLE,
	CIRCLE,
	RECTANGLE_LINES,
	CIRCLE_LINES
};

class SpriteComponent : public Component {

public:

	GLenum primitiveType;
	GLenum drawMode;
	unsigned int vaoId;
	unsigned short indexElementCount;
	unsigned int shaderProgram;

	SpriteShape shape{};
	std::string image{};
	glm::mat3 modelToNDCMatrix{ 1,0,0,0,1,0,0,0,1 };
	float color[4]{};

};


#endif SPRITE_H
