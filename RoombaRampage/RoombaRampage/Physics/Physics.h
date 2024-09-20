#ifndef PHYSICS_H
#define PHYSICS_H

#include "../Math/vector2.h"
#include <vector>
#include <algorithm>
#include <iostream>



namespace Physics {
	
	enum class EntityType {
		Circle,
		Rectangle
	};

	struct AABB {
		Vector2::Vec2 min;
		Vector2::Vec2 max;
	};

	//class PhysicsData {
	//public:
	//	Vector2::Vec2 scale{};                         // Axis-Aligned Bounding Box
	//	Vector2::Vec2 position{};                   // Position of the entity's center
	//	Vector2::Vec2 velocity{};                   // Current velocity of the entity
	//	int ID = -1;                                // Unique identifier
	//	EntityType type = EntityType::Rectangle;    // Circle or Rectangle

	//	virtual ~PhysicsData() = default;

	//	 Get entity type
	//	virtual EntityType GetEntity() const = 0;

	//	 Operator for equality check
	//	bool operator==(const PhysicsData& other) const {
	//		return (ID == other.ID);
	//	}
	//};

	// Circle class derived from PhysicsData
	//class Circle : public PhysicsData {
	//public:
	//	float m_radius = 0.0f;

	//	 Constructor for Circle
	//	Circle(float radius, Vector2::Vec2 shape_position, Vector2::Vec2 shape_velocity, int entity_ID) {
	//		m_radius = radius;
	//		position = shape_position;
	//		velocity = shape_velocity;
	//		ID = entity_ID;
	//		type = EntityType::Circle; // Directly set the type as Circle
	//	}

	//	 Overriding GetEntity for Circle
	//	EntityType GetEntity() const override {
	//		return EntityType::Circle;
	//	}
	//};

	// Rectangle class derived from PhysicsData
	//class Rectangle : public PhysicsData {
	//public:
	//	float height = 0.0f;  // For rectangular entities
	//	float width = 0.0f;   // For rectangular entities
	//	AABB boundingBox{};
	//	 Constructor for Rectangle
	//	Rectangle(float rect_height, float rect_width, Vector2::Vec2 shape_position, Vector2::Vec2 shape_velocity, int entity_ID) {
	//		height = rect_height;
	//		width = rect_width;
	//		position = shape_position;
	//		velocity = shape_velocity;
	//		ID = entity_ID;
	//		type = EntityType::Rectangle; // Directly set the type as Rectangle
	//	}

	//	 Overriding GetEntity for Rectangle
	//	EntityType GetEntity() const override {
	//		return EntityType::Rectangle;
	//	}
	//};



	struct PhysicsData {
	public:
		Vector2::Vec2 scale{};
		float width = 0.0f;
		float height = 0.0f;
		float m_radius = 0.0f;
		AABB boundingBox{};						 // Axis-Aligned Bounding Box
		Vector2::Vec2 position{};				 // Position of the entity's center
		Vector2::Vec2 velocity{};				 // Current velocity of the entity
		int ID = -1.0f;							 // Unique identifier
		EntityType type = EntityType::Rectangle; // Circle or Rectangle
		//for checking 
		bool operator==(const PhysicsData& other) const {
			return (ID == other.ID);
		}

		//virtual ~PhysicsData() = default;
		//get entity type
		//virtual EntityType funcGetEntity() const = 0;
	};


	struct LineSegment
	{
		Vector2::Vec2 m_pt0;
		Vector2::Vec2 m_pt1;
		Vector2::Vec2 m_normal;
	};

	/*
		player + wall first
	*/

	class classPhysics {
	private:
		
		std::vector<PhysicsData> physicsEntities;
		std::vector<PhysicsData> collidedEntities;
		void CalculateBoundingBox();

	public:
		void RetrievePhysicsData(Vector2::Vec2 scale, Vector2::Vec2 position, Vector2::Vec2 velocity, int ID);
		void CollisionCheck(float);
		std::vector<PhysicsData> PassPhysicsData(); 
		void ClearEntites();

		//static dynamic collision
		bool CollisionIntersection_RectRect(const PhysicsData&, const PhysicsData&, float );
		bool CollisionIntersection_CircleRect(const PhysicsData&, const PhysicsData&);
		bool CollisionIntersection_CircleCircle(const PhysicsData&, const PhysicsData&);
		bool static_CollisionCheck(const AABB, const AABB);
		void debug(PhysicsData&);
	};
}
#endif
