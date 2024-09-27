#ifndef PHYSICS_H
#define PHYSICS_H

#include "../Math/vector2.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include <new>



namespace Physics {
	
	enum class EntityType {
		Circle,
		Rectangle
	};

	struct AABB {
		Vector2::Vec2 min;
		Vector2::Vec2 max;
	};

	/**********************************
		BASE CLASS
	**********************************/
	class PhysicsData {
	public:
		Vector2::Vec2 scale{};                         // Axis-Aligned Bounding Box
		Vector2::Vec2 position{};                   // Position of the entity's center
		Vector2::Vec2 velocity{};                   // Current velocity of the entity
		int ID = -1;                                // Unique identifier
		EntityType type = EntityType::Rectangle;    // Circle or Rectangle

		virtual ~PhysicsData() = default;

		//Get entity type
		virtual EntityType GetEntity() const = 0;

		//Operator for equality check
		bool operator==(const PhysicsData& other) const {
			return (ID == other.ID);
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
		Circle(float radius, Vector2::Vec2 shape_position, Vector2::Vec2 shape_scale, Vector2::Vec2 shape_velocity, int entity_ID);

		// Overriding GetEntity for Circle
		EntityType GetEntity() const override {
			return EntityType::Circle;
		}
	};

	// Rectangle class derived from PhysicsData
	class Rectangle : public PhysicsData {
	public:
		float height = 0.0f;  // For rectangular entities
		float width = 0.0f;   // For rectangular entities
		AABB boundingBox{};
		Rectangle() = default;
		// Constructor for Rectangle (declaration)
		Rectangle(float rect_height, float rect_width, Vector2::Vec2 shape_position, Vector2::Vec2 shape_scale, Vector2::Vec2 shape_velocity, int entity_ID);

		// Overriding GetEntity for Rectangle
		EntityType GetEntity() const override {
			return EntityType::Rectangle;
		}
	};

	struct LineSegment
	{
		Vector2::Vec2 m_pt0;
		Vector2::Vec2 m_pt1;
		Vector2::Vec2 m_normal;
	};

	class classPhysics {
	private:
		
		static std::vector<std::shared_ptr<PhysicsData>> physicsEntities;
		static std::vector<std::shared_ptr<PhysicsData>> collidedEntities;
		//static std::vector<std::unique_ptr<PhysicsData>> physicsEntities;
		//static std::vector<std::unique_ptr<PhysicsData>> collidedEntities;
		void CalculateBoundingBox();

	public:
		void SendPhysicsData(float rect_height, float rect_width, Vector2::Vec2 position, Vector2::Vec2 scale, Vector2::Vec2 velocity, int ID);
		void SendPhysicsData(float radius, Vector2::Vec2 scale, Vector2::Vec2 position, Vector2::Vec2 velocity, int ID);
		void CollisionCheck(float);
		std::vector<std::shared_ptr<PhysicsData>> RetrievePhysicsData();
		void ClearEntites();
		//static dynamic collision
		bool CollisionIntersection_RectRect(const Rectangle&, const Rectangle&, float );
		bool CollisionIntersection_CircleRect(const Circle&, const Rectangle&);
		bool CollisionIntersection_CircleCircle(const Circle&, const Circle&);
		bool static_CollisionCheck(const AABB, const AABB);
	};
}
#endif
