/******************************************************************/
/*!
\file      Physics.h
\author    Rayner Tan, raynerweichen.tan , 2301449
\par       raynerweichen.tan@digipen.edu
\date      Sept 28, 2024
\brief     Physics pipeline functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef PHYSICS_H
#define PHYSICS_H

#include "../Math/vector2.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include <new>
#include "../Debugging/Logging.h"



namespace physicspipe {
	
	enum class EntityType {
		CIRCLE,
		RECTANGLE
	};

	struct AABB {
		vector2::Vec2 m_min;
		vector2::Vec2 m_max;
	};

	/**********************************
		BASE CLASS
	**********************************/
	class PhysicsData {
	public:
		vector2::Vec2 m_scale{};                         // Axis-Aligned Bounding Box
		vector2::Vec2 m_position{};                   // Position of the entity's center
		vector2::Vec2 m_velocity{};                   // Current velocity of the entity
		int m_ID = -1;                                // Unique identifier
		EntityType type = EntityType::RECTANGLE;    // Circle or Rectangle

		virtual ~PhysicsData() = default;

		//Get entity type
		virtual EntityType GetEntity() const = 0;

		//Operator for equality check
		bool operator==(const PhysicsData& other) const {
			return (m_ID == other.m_ID);
		}
	};

	/**********************************
		DERIVED CLASS
	**********************************/

	// Circle class derived from PhysicsData
	class Circle : public PhysicsData {
	public:
		float m_radius = 0.0f;
		Circle() = default;
		// Constructor for Circle (declaration)
		Circle(float radius, vector2::Vec2 shape_position, vector2::Vec2 shape_scale, vector2::Vec2 shape_velocity, int entity_ID);

		// Overriding GetEntity for Circle
		EntityType GetEntity() const override {
			return EntityType::CIRCLE;
		}
	};

	// Rectangle class derived from PhysicsData
	class Rectangle : public PhysicsData {
	public:
		float m_height = 0.0f;  // For rectangular entities
		float m_width = 0.0f;   // For rectangular entities
		AABB m_boundingBox{};
		Rectangle() = default;
		// Constructor for Rectangle (declaration)
		Rectangle(float rect_height, float rect_width, vector2::Vec2 shape_position, vector2::Vec2 shape_scale, vector2::Vec2 shape_velocity, int entity_ID);

		// Overriding GetEntity for Rectangle
		EntityType GetEntity() const override {
			return EntityType::RECTANGLE;
		}
	};

	struct LineSegment
	{
		vector2::Vec2 m_pt0;
		vector2::Vec2 m_pt1;
		vector2::Vec2 m_normal;
	};

	class Physics {
	private:
		
		static std::vector<std::shared_ptr<PhysicsData>> m_physicsEntities;
		static std::vector<std::shared_ptr<PhysicsData>> m_collidedEntities;
		void m_CalculateBoundingBox();

	public:
		void m_SendPhysicsData(float rect_height, float rect_width, vector2::Vec2 position, vector2::Vec2 scale, vector2::Vec2 velocity, int ID);
		void m_SendPhysicsData(float radius, vector2::Vec2 position, vector2::Vec2 scale, vector2::Vec2 velocity, int ID);
		void m_CollisionCheck(float);
		std::vector<std::shared_ptr<PhysicsData>> m_RetrievePhysicsData();
		void m_ClearEntites();
		//static dynamic collision
		bool m_CollisionIntersection_RectRect(const Rectangle&, const Rectangle&, float );
		bool m_CollisionIntersection_CircleRect(const Circle&, const Rectangle&);
		bool m_CollisionIntersection_CircleCircle(const Circle&, const Circle&);
		bool m_static_CollisionCheck(const AABB, const AABB);
	};
}
#endif
