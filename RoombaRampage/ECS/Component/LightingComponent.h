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
		vector2::Vec2 m_innerOuterRadius{ 0.f,1.f };
		vector3::Vec3 m_colour{ 1.f,1.f,1.f };

		REFLECTABLE(LightingComponent, m_innerOuterRadius, m_intensity, m_colour);
	};

}


#endif LIGHT_H
