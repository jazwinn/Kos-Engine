#ifndef SYSTYPE_H
#define SYSTYPE_H
namespace Ecs {

	enum TypeSystem {
		TypeMovementSystem,
		TypeControlSystem,




		TypeCollisionSystem,
		TypeCollisionResponseSystem,
		//render system called the last
		TypeRenderSystem,

		TotalTypeSystem
	};
}

#endif