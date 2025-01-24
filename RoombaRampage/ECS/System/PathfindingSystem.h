#pragma once
#pragma once
#ifndef PATHSYS_H
#define PATHSYS_H


#include "System.h"
#include "../ECS/ECSList.h"
#include <glew.h>
#include "glfw3.h"
#include <gtc/type_ptr.hpp>

namespace ecs {

	class PathfindingSystem : public ISystem {

	private:


	public:

		void m_RegisterSystem(EntityID) override;

		void m_DeregisterSystem(EntityID) override;

		void m_Init() override;

		void m_Update(const std::string&) override;



	private:

		std::vector<TransformComponent*> m_vecTransformComponentPtr;

		std::vector<PathfindingComponent*> m_vecPathfindingComponentPtr;


	};

}



#endif