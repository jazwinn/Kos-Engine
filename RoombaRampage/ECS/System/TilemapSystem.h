#ifndef TILEMAPSYS_H
#define TILEMAPSYS_H


#include "System.h"
#include "../ECS/ECSList.h"
#include <glew.h>
#include "glfw3.h"
#include <gtc/type_ptr.hpp>

namespace ecs {

	class TilemapSystem : public ISystem {

	private:


	public:

		void m_RegisterSystem(EntityID) override;

		void m_DeregisterSystem(EntityID) override;

		void m_Init() override;

		void m_Update(const std::string&) override;


	private:
		//Storage to point to components
		std::vector<TransformComponent*> m_vecTransformComponentPtr;
		std::vector<TilemapComponent*> m_vecTilemapComponentPtr;

	};

}



#endif