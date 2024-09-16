#ifndef PHYSICS_H
#define PHYSICS_H

#include "../Math/vector2.h"
#include <vector>
#include <algorithm>


namespace PHYSICS {
	
	enum class EntityType {
		Circle,
		Rectangle
	};

	struct AABB {
		Vector2::Vec2 min;
		Vector2::Vec2 max;
	};

	struct PhysicsData {
		float m_radius = 0.0f;					 // For circular entities
		AABB boundingBox{};						 // Axis-Aligned Bounding Box
		Vector2::Vec2 center{};					 // Position of the entity's center
		Vector2::Vec2 velocity{};				 // Current velocity of the entity
		float height = 0.0f;					 // For rectangular entities
		float width = 0.0f;						 // For rectangular entities
		int ID = -1.0f;					     // Unique identifier
		EntityType type = EntityType::Rectangle; // Circle or Rectangle
		//for checking 
		bool operator==(const PhysicsData& other) const {
			return (ID == other.ID);
		}
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
		constexpr static float bounding_rect_size = 1.0f; // for AABB
		std::vector<PhysicsData> physicsEntities;
		std::vector<PhysicsData> collidedEntities;
		void funcCalculateBoundingBox();

	public:
		void funcRetrievePhysicsData();
		void funcCollisionCheck();
		std::vector<PhysicsData> funcPassPhysicsData() const;

		//static dynamic collision
		bool funcCollisionIntersection_RectRect(const PhysicsData&, const PhysicsData&);
		bool funcCollisionIntersection_CircleRect(const PhysicsData&, const PhysicsData&);
	};
}
#endif
