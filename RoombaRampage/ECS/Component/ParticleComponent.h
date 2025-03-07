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
		int m_noOfParticles{1};
		float m_lifeSpan{1.f};
		vector2::Vec2 m_velocity{};
		vector2::Vec2 m_acceleration{};
		vector3::Vec3 m_color{};
		float m_coneRotation{}; //Cone Rotation
		float m_coneAngle{360.f};
		float m_randomFactor{};
		std::string m_imageFile{};
		int m_stripCount{};
		int m_frameNumber{};
		int m_layer{};
		float m_friction{};
		

		REFLECTABLE(ParticleComponent, m_willSpawn, m_noOfParticles, m_lifeSpan, m_velocity, m_acceleration,
										m_color, m_coneRotation, m_coneAngle, m_randomFactor, m_stripCount, m_frameNumber, 
										m_layer, m_friction);
	};

}


#endif PARTICLE_H

