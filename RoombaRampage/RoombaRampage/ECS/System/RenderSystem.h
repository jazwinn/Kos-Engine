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

	void RegisterSystem(EntityID) override;

	void DeregisterSystem(EntityID) override;

	void Init() override;

	void Update() override;

	static void funcTransferData();

private:
	//Storage to point to components
	std::vector<TransformComponent*> vecTransformComponentPtr;
	std::vector<SpriteComponent*> vecSpriteComponentPtr;

};

#endif