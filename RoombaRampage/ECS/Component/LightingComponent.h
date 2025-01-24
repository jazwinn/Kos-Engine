#ifndef LIGHT_H
#define LIGHT_H

#include "Component.h"
#include "../Graphics/GraphicsLighting.h"



namespace ecs
{

	class LightingComponent : public Component {

	public:

		graphicpipe::LightType m_lightType{ graphicpipe::LightType::SPOT };
		float m_intensity{ 1.f };
		vector2::Vec2 m_innerOuterRadius;
		vector3::Vec3 m_colour;

		REFLECTABLE(LightingComponent, m_innerOuterRadius, m_lightType);
	};

}


#endif LIGHT_H
