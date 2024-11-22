/******************************************************************/
/*!
\file      Physics.h
\author    Rayner Tan, raynerweichen.tan , 2301449
\par       raynerweichen.tan@digipen.edu
\date      Sept 28, 2024
\brief     This header file defines the physics pipeline classes,
		   including entities like Circle and Rectangle, and the
		   Physics class for managing physics data, collision detection,
		   and bounding box calculations.

The Physics system includes methods to handle collisions between
rectangular and circular entities and stores information about
collided objects.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef PHYSICS_H
#define PHYSICS_H

#include "PhysicsLayer.h"
#include "../Debugging/Logging.h"


namespace physicspipe {
	
	enum class EntityType {
		RECTANGLE,
		CIRCLE
	};

	struct AABB {
		vector2::Vec2 m_min;
		vector2::Vec2 m_max;
	};

	/**********************************
		BASE CLASS
	**********************************/
	/******************************************************************/
	/*!
	\class     PhysicsData
	\brief     A base class for physics entities, including data members
			   for position, scale, velocity, and unique entity ID.
	*/
	/******************************************************************/
	class PhysicsData {
	public:
		vector2::Vec2 m_scale{};                         // Axis-Aligned Bounding Box
		vector2::Vec2 m_position{};                   // Position of the entity's center
		vector2::Vec2 m_velocity{};                   // Current velocity of the entity
		int m_ID = -1;                                // Unique identifier
		EntityType type = EntityType::RECTANGLE;    // Circle or Rectangle
		int m_layerID = -1;

		virtual ~PhysicsData() = default;

		//Get entity type
		virtual EntityType GetEntity() const = 0;
		virtual float GetBoundingRadius() const = 0;
		//Operator for equality check
		bool operator==(const PhysicsData& other) const {
			return (m_ID == other.m_ID);
		}
	};

	/**********************************
		DERIVED CLASS
	**********************************/


	/******************************************************************/
	/*!
	\class     Circle
	\brief     A derived class from PhysicsData representing a circle entity
			   in the physics system. It includes methods to return the entity
			   type and store circle-specific data like radius.
	*/
	/******************************************************************/
	class Circle : public PhysicsData {
	public:
		float m_radius = 0.0f;
		Circle() = default;
		// Constructor for Circle (declaration)
		/******************************************************************/
		/*!
		\fn        Circle::Circle(float radius, vector2::Vec2 shape_position, vector2::Vec2 shape_scale, vector2::Vec2 shape_velocity, int entity_ID)
		\brief     Constructor for creating a circle physics entity.
		\param[in] radius       Radius of the circle.
		\param[in] shape_position Position of the circle entity.
		\param[in] shape_scale  Scale of the circle entity.
		\param[in] shape_velocity Velocity of the circle entity.
		\param[in] entity_ID    Unique ID for the entity.
		*/
		/******************************************************************/
		Circle(float radius, vector2::Vec2 shape_position, vector2::Vec2 shape_scale, vector2::Vec2 shape_velocity, int entity_ID, int layer_ID);

		// Overriding GetEntity for Circle
		EntityType GetEntity() const override {
			return EntityType::CIRCLE;
		}

		float GetBoundingRadius() const;
	};

	/******************************************************************/
	/*!
	\class     Rectangle
	\brief     A derived class from PhysicsData representing a rectangular
			   entity in the physics system. It includes methods to return
			   the entity type and store rectangle-specific data like
			   height, width, and bounding box.
	*/
	/******************************************************************/
	class Rectangle : public PhysicsData {
	public:
		float m_height = 0.0f;  // For rectangular entities
		float m_width = 0.0f;   // For rectangular entities
		float m_rotAngle = 0.0f;
		AABB m_boundingBox{};
		Rectangle() = default;
		// Constructor for Rectangle (declaration)
		/******************************************************************/
		/*!
		\fn        Rectangle::Rectangle(float rect_height, float rect_width, vector2::Vec2 shape_position, vector2::Vec2 shape_scale, vector2::Vec2 shape_velocity, int entity_ID)
		\brief     Constructor for creating a rectangle physics entity.
		\param[in] rect_height  Height of the rectangle.
		\param[in] rect_width   Width of the rectangle.
		\param[in] shape_position Position of the rectangle entity.
		\param[in] shape_scale  Scale of the rectangle entity.
		\param[in] shape_velocity Velocity of the rectangle entity.
		\param[in] entity_ID    Unique ID for the entity.
		*/
		/******************************************************************/
		Rectangle(float rect_height, float rect_width, float rect_angle, vector2::Vec2 shape_position, vector2::Vec2 shape_scale,
			vector2::Vec2 shape_velocity, int entity_ID, int layer_ID);

		// Overriding GetEntity for Rectangle
		EntityType GetEntity() const override {
			return EntityType::RECTANGLE;
		}

		//FOR SAT
		/******************************************************************/
		/*!
		\fn        std::vector<vector2::Vec2> Rectangle::m_getRotatedVertices() const
		\brief     Calculates and returns the vertices of the rectangle after
				   applying rotation based on the current rotation angle.
		\return    A vector of `Vec2` points representing the rotated vertices of the rectangle.
		*/
		/******************************************************************/
		std::vector<vector2::Vec2> m_getRotatedVertices() const;
		/******************************************************************/
		/*!
		\fn        std::vector<vector2::Vec2> Rectangle::m_getEdges() const
		\brief     Calculates and returns the edges of the rectangle based on
					its vertices, useful for collision detection.
		\return    A vector of `Vec2` points representing the edges of the rectangle.
		*/
		/******************************************************************/
		std::vector<vector2::Vec2> m_getEdges() const;
		float GetBoundingRadius() const;
	};

	struct LineSegment
	{
		vector2::Vec2 m_pt0;
		vector2::Vec2 m_pt1;
		vector2::Vec2 m_normal;
	};

	void m_BuildLineSegment(LineSegment& lineSegment, const vector2::Vec2& p0, const vector2::Vec2& p1);

	/******************************************************************/
	/*!
	\class     Physics
	\brief     A class that manages the physics pipeline, including adding
			   entities, detecting collisions, and clearing physics data.
			   It supports circle-rectangle, circle-circle, and
			   rectangle-rectangle collisions.
	*/
	/******************************************************************/
	const int MAX_ENTITIES = 1024;
	class Physics {
	private:
		
		static std::vector<std::shared_ptr<PhysicsData>> m_physicsEntities;
		static std::vector<std::shared_ptr<PhysicsData>> m_collidedEntities;
		static std::vector < std::pair<std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>>> m_collidedEntitiesPair;
		static std::map<layer::LAYERS, std::vector<std::shared_ptr<PhysicsData>>> m_layerToEntities;
		static std::vector<int> m_checker;
		static std::unique_ptr<Physics> m_instance;
		std::bitset<MAX_ENTITIES> collidedEntities;

		/******************************************************************/
		/*!
		\fn        void Physics::m_CalculateBoundingBox()
		\brief     Calculates and updates the axis-aligned bounding boxes
				   (AABB) for all entities in the system.
		*/
		/******************************************************************/
		void m_CalculateBoundingBox();
		void logCollision(int entityID);
		bool hasCollided(int entityID) const;

	public:
		//SINGLETON
		static Physics* getInstance() {
			if (!m_instance) {
				m_instance = std::make_unique<Physics>();
			}
			return m_instance.get();
		}

		/******************************************************************/
		/*!
		\fn        void Physics::m_SendPhysicsData(float rect_height, float rect_width, vector2::Vec2 position, vector2::Vec2 scale, vector2::Vec2 velocity, int ID)
		\brief     Adds a rectangle entity to the physics system.
		\param[in] rect_height  Height of the rectangle.
		\param[in] rect_width   Width of the rectangle.
		\param[in] position     Position of the rectangle.
		\param[in] scale        Scale of the rectangle.
		\param[in] velocity     Velocity of the rectangle.
		\param[in] ID           Unique ID of the rectangle entity.
		*/
		/******************************************************************/
		void m_SendPhysicsData(float rect_height, float rect_width, float rect_angle, vector2::Vec2 position, vector2::Vec2 scale,
			vector2::Vec2 velocity, int ID, layer::LAYERS layerID);
		/******************************************************************/
		/*!
		\fn        void Physics::m_SendPhysicsData(float radius, vector2::Vec2 position, vector2::Vec2 scale, vector2::Vec2 velocity, int ID)
		\brief     Adds a circle entity to the physics system.
		\param[in] radius    Radius of the circle.
		\param[in] position  Position of the circle.
		\param[in] scale     Scale of the circle.
		\param[in] velocity  Velocity of the circle.
		\param[in] ID        Unique ID of the circle entity.
		*/
		/******************************************************************/
		void m_SendPhysicsData(float radius, vector2::Vec2 position, vector2::Vec2 scale, vector2::Vec2 velocity, int ID, layer::LAYERS layerID);

		void m_CollisionCheck(float);
		/******************************************************************/
		/*!
		\fn        std::vector<std::shared_ptr<PhysicsData>> Physics::m_RetrievePhysicsData()
		\brief     Retrieves the list of physics entities that have collided
				   in the system and clears the current entities list.
		\return    A vector of shared pointers to collided physics entities.
		*/
		/******************************************************************/
		std::vector<std::shared_ptr<PhysicsData>> m_RetrievePhysicsData();
		std::vector < std::pair<std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>>> m_RetrievePhysicsDataPair();

		/******************************************************************/
		/*!
		\fn        void Physics::m_ClearEntites()
		\brief     Clears all stored physics entities and collided entities
				   from the physics system.
		*/
		/******************************************************************/
		void m_ClearEntites();
		/******************************************************************/
		/*!
		\fn        bool Physics::m_CollisionIntersection_RectRect(const Rectangle& obj1, const Rectangle& obj2, float dt)
		\brief     Checks for dynamic and static collision between two rectangle entities.
		\param[in] obj1 The first rectangle entity.
		\param[in] obj2 The second rectangle entity.
		\param[in] dt   The time step for the collision detection process.
		\return    True if a collision is detected, false otherwise.
		*/
		/******************************************************************/
		bool m_CollisionIntersection_RectRect_AABB(const Rectangle&, const Rectangle&, float );
		/******************************************************************/
		/*!
		\fn        bool Physics::m_CollisionIntersection_CircleRect(const Circle& circle, const Rectangle& rect)
		\brief     Checks for collision between a circle and a rectangle entity.
		\param[in] circle The circle entity.
		\param[in] rect   The rectangle entity.
		\return    True if a collision is detected, false otherwise.
		*/
		/******************************************************************/
		bool m_CollisionIntersection_CircleRect_AABB(const Circle&, const Rectangle&);
		bool m_CollisionIntersection_CircleRect_SAT(const Circle&, const Rectangle&);
		/******************************************************************/
		/*!
		\fn        bool Physics::m_CollisionIntersection_CircleCircle(const Circle& circle1, const Circle& circle2)
		\brief     Checks for collision between two circle entities.
		\param[in] circle1 The first circle entity.
		\param[in] circle2 The second circle entity.
		\return    True if a collision is detected, false otherwise.
		*/
		/******************************************************************/
		bool m_CollisionIntersection_CircleCircle(const Circle&, const Circle&);
		/******************************************************************/
		/*!
		\fn        bool Physics::m_static_CollisionCheck(const AABB aabb1, const AABB aabb2)
		\brief     Performs a static collision check between two axis-aligned bounding boxes (AABB).
		\param[in] aabb1  The first AABB for comparison.
		\param[in] aabb2  The second AABB for comparison.
		\return    True if a collision is detected, false otherwise.
		*/
		/******************************************************************/
		bool m_static_CollisionCheck(const AABB, const AABB);


		void m_Init();
		void m_Update();

		/******************************************************************/
		/*!
		\fn        void Physics::m_CollisionCheckUpdate()
		\brief     Updates the collision detection process by checking for
				   potential collisions among all entities in the physics system.
		*/
		/******************************************************************/
		void m_CollisionCheckUpdate();
		/******************************************************************/
		/*!
		\fn        bool Physics::m_CheckCollision(const std::shared_ptr<PhysicsData>& entity1, const std::shared_ptr<PhysicsData>& entity2)
		\brief     Checks for a collision between two specified entities.
		\param[in] entity1 A shared pointer to the first entity.
		\param[in] entity2 A shared pointer to the second entity.
		\return    True if a collision is detected between the two entities, false otherwise.
		*/
		/******************************************************************/
		bool m_CheckCollision(const std::shared_ptr<PhysicsData>& entity1, const std::shared_ptr<PhysicsData>& entity2);
		/******************************************************************/
		/*!
		\fn        void Physics::m_projectOntoAxis(const std::vector<vector2::Vec2>& vertices, const vector2::Vec2& axis, float& min, float& max) const
		\brief     Projects the vertices of a shape onto a specified axis and
				   calculates the minimum and maximum scalar values along that axis.
		\param[in] vertices A vector of vertices representing the shape to project.
		\param[in] axis     The axis onto which the vertices are projected.
		\param[out] min     The minimum scalar value on the axis after projection.
		\param[out] max     The maximum scalar value on the axis after projection.
		*/
		/******************************************************************/
		void m_projectOntoAxis(const std::vector<vector2::Vec2>& vertices, const vector2::Vec2& axis, float& min, float& max) const;
		/******************************************************************/
		/*!
		\fn        bool Physics::m_CollisionIntersection_RectRect_SAT(const Rectangle& obj1, const Rectangle& obj2)
		\brief     Checks for collision between two rectangle entities using the Separating Axis Theorem (SAT).
		\param[in] obj1 The first rectangle entity.
		\param[in] obj2 The second rectangle entity.
		\return    True if a collision is detected, false otherwise.
		*/
		/******************************************************************/
		bool m_CollisionIntersection_RectRect_SAT(const Rectangle& obj1, const Rectangle& obj2);
		bool m_withinBoundingRadius(const std::shared_ptr<PhysicsData>& entity1, const std::shared_ptr<PhysicsData>& entity2);
		void m_addCollidedEntity(const std::shared_ptr<PhysicsData>& entity); 
		void m_clearPair();

		void m_projectOntoCircle(vector2::Vec2 center, float radius, vector2::Vec2 axis, float& min, float& max);

		int m_findClosestPointOnPolygon(vector2::Vec2 circle_pos, std::vector<vector2::Vec2> vertices);

};
}
#endif
