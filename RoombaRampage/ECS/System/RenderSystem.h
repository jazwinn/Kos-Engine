#ifndef RENDERSYS_H
#define RENDERSYS_H


#include "System.h"
#include "../ECS/Component/SpriteComponent.h"
#include <glew.h>
#include "glfw3.h"
#include <gtc/type_ptr.hpp>

namespace ecs{

	class RenderSystem : public ISystem {

	private:


	public:

		void m_RegisterSystem(EntityID) override;

		void m_DeregisterSystem(EntityID) override;

		void m_Init() override;

		void m_Update() override;


	private:
		//Storage to point to components
		std::vector<TransformComponent*> m_vecTransformComponentPtr;
		std::vector<SpriteComponent*> m_vecSpriteComponentPtr;

	};

}



#endif