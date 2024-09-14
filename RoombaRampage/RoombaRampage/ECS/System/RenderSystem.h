#ifndef RENDERSYS_H
#define RENDERSYS_H


#include "System.h"
#include "../ECS/Component/SpriteComponent.h"
#include <glew.h>
#include "glfw3.h"
#include <gtc/type_ptr.hpp>

class RenderSystem : public ISystem {

private:
	

public:

	void Init() override;

	void Update() override;

	static void funcTransferData();

};

#endif