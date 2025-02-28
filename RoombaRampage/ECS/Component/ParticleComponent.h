/******************************************************************/
/*!
\file      ParticleComponent.h
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      28 February, 2025
\brief



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef PARTICLE_H
#define PARTICLE_H

#include "Component.h"



namespace ecs
{

	class ParticleComponent : public Component {

	public:

		bool m_willSpawn{false};
		int m_noOfParticles{};
		float m_lifeSpan{};
		float m_coneAngle{};
		float m_randomFactor{};
		vector2::Vec2 m_velocity{};
		vector2::Vec2 m_acceleration{};
		vector3::Vec3 m_color{};

		REFLECTABLE(ParticleComponent, m_willSpawn ,m_noOfParticles, m_lifeSpan, m_coneAngle, m_randomFactor ,m_velocity, m_acceleration, m_color);
	};

}


#endif PARTICLE_H

