/******************************************************************/
/*!
\file      Physics.cpp
\author    Rayner Tan, raynerweichen.tan , 2301449
\par       raynerweichen.tan@digipen.edu
\date      Sept 28, 2024
\brief     Physics pipeline functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "Physics.h"
namespace physicspipe {

	std::vector<std::shared_ptr<PhysicsData>> Physics::m_physicsEntities;
	std::vector<std::shared_ptr<PhysicsData>> Physics::m_collidedEntities;

// Constructor for Circle (definition)
Circle::Circle(float radius, vector2::Vec2 shape_position, vector2::Vec2 shape_scale, vector2::Vec2 shape_velocity, int entity_ID)
    : m_radius(radius)   // Initialize radius
{
	m_position = shape_position;
	m_scale = shape_scale;
	m_velocity = shape_velocity;
	m_ID = entity_ID;
    type = EntityType::CIRCLE;  // Set type to Circle
}

// Constructor for Rectangle (definition)
Rectangle::Rectangle(float rect_height, float rect_width, vector2::Vec2 shape_position, vector2::Vec2 shape_scale, vector2::Vec2 shape_velocity, int entity_ID)
    : m_height(rect_height), m_width(rect_width)   // Initialize height and width
{
	m_position = shape_position;
	m_scale = shape_scale;
	m_velocity = shape_velocity;
	m_ID = entity_ID;
    type = EntityType::RECTANGLE;  // Set type to Rectangle
}

	bool Physics::m_static_CollisionCheck(const AABB aabb1, const AABB aabb2) {
		//std::cout << "BOUNDING BOX1 MIN X " << aabb1.min.x << " Y " << aabb1.min.y << std::endl;
		//std::cout << "BOUNDING BOX1 MAX X " << aabb1.max.x << " Y " << aabb1.max.y << std::endl;
		//std::cout << "BOUNDING BOX2 MIN X " << aabb2.min.x << " Y " << aabb2.min.y << std::endl;
		//std::cout << "BOUNDING BOX2 MAX X " << aabb2.max.x << " Y " << aabb2.max.y << std::endl;

		if (aabb2.m_max.m_x < aabb1.m_min.m_x) return 0;
		if (aabb2.m_min.m_x > aabb1.m_max.m_x) return 0;
		if (aabb2.m_max.m_y < aabb1.m_min.m_y) return 0;
		if (aabb2.m_min.m_y > aabb1.m_max.m_y) return 0;
		//std::cout << "Static Collision fail " << std::endl;
		return 1;
	}

	void Physics::m_SendPhysicsData(float rect_height, float rect_width, vector2::Vec2 position, vector2::Vec2 scale, vector2::Vec2 velocity, int ID) {
		m_physicsEntities.push_back(std::make_shared<Rectangle>(rect_height, rect_width, position, scale, velocity, ID));
	}

	void Physics::m_SendPhysicsData(float radius, vector2::Vec2 position, vector2::Vec2 scale, vector2::Vec2 velocity, int ID){
		m_physicsEntities.push_back(std::make_shared<Circle>(radius, position, scale, velocity, ID));
	}
	

	void Physics::m_CollisionCheck(float dt) {

		//calculate boarding box
		m_CalculateBoundingBox();

		for (size_t i = 0; i < m_physicsEntities.size(); ++i) {
			for (size_t j = 0; j < m_physicsEntities.size(); ++j) {
				if (m_physicsEntities[i]->m_ID != m_physicsEntities[j]->m_ID) {
					/*************************************
						CHECK RECT V RECT
					*************************************/
					if ((m_physicsEntities[i]->GetEntity() == EntityType::RECTANGLE) && (m_physicsEntities[j]->GetEntity() == EntityType::RECTANGLE)) {
						if (m_CollisionIntersection_RectRect(*dynamic_cast<Rectangle*>(m_physicsEntities[i].get()), *dynamic_cast<Rectangle*>(m_physicsEntities[i].get()), dt)) {
							//checking whether if entity is alr added inside
							if (std::find(m_collidedEntities.begin(), m_collidedEntities.end(), m_physicsEntities[i]) == m_collidedEntities.end()) {
								m_collidedEntities.push_back(m_physicsEntities[i]);
							}
						}
					}
					/*************************************
						CHECK CIRCLE V RECT
					*************************************/
					else if ((m_physicsEntities[i]->GetEntity() == EntityType::CIRCLE) && (m_physicsEntities[j]->GetEntity() == EntityType::RECTANGLE)) {
						if (m_CollisionIntersection_CircleRect(*dynamic_cast<Circle*>(m_physicsEntities[i].get()), *dynamic_cast<Rectangle*>(m_physicsEntities[j].get()))) {
							if (std::find(m_collidedEntities.begin(), m_collidedEntities.end(), m_physicsEntities[i]) == m_collidedEntities.end()) {
								m_collidedEntities.push_back(m_physicsEntities[i]);
							}
						}
					}

					/*************************************
						CHECK RECT V CIRCLE
					*************************************/
					else if (m_physicsEntities[j]->GetEntity() == EntityType::CIRCLE && m_physicsEntities[i]->GetEntity() == EntityType::RECTANGLE) {
						if (m_CollisionIntersection_CircleRect(*dynamic_cast<Circle*>(m_physicsEntities[j].get()), *dynamic_cast<Rectangle*>(m_physicsEntities[i].get()))) {
							if (std::find(m_collidedEntities.begin(), m_collidedEntities.end(), m_physicsEntities[i]) == m_collidedEntities.end()) {
								m_collidedEntities.push_back(m_physicsEntities[i]);
							}
						}
					}
					/*************************************
						CHECK CIRCLE V CIRCLE
					*************************************/
					else if(m_physicsEntities[i]->GetEntity() == EntityType::CIRCLE && m_physicsEntities[j]->GetEntity() == EntityType::CIRCLE) {
						if (m_CollisionIntersection_CircleCircle(*dynamic_cast<Circle*>(m_physicsEntities[i].get()), *dynamic_cast<Circle*>(m_physicsEntities[j].get()))) {
							if (std::find(m_collidedEntities.begin(), m_collidedEntities.end(), m_physicsEntities[i]) == m_collidedEntities.end()) {
								m_collidedEntities.push_back(m_physicsEntities[i]);
							}
						}
					}
					else {
						LOGGING_ERROR("NO CASE FOR CHECKING");
					}
				}
				//std::cout << "********************************************************************************" << std::endl;
			}
		}
		//std::cout << "Physics Entities size " << physicsEntities.size() << " collideEntities " << collidedEntities.size() << std::endl;
	}

	std::vector<std::shared_ptr<PhysicsData>> Physics::m_RetrievePhysicsData() {
		std::vector<std::shared_ptr	<PhysicsData>> TempCollidedEntities = m_collidedEntities;
		this->m_ClearEntites();
		return TempCollidedEntities;
	}

	void Physics::m_ClearEntites(){
		m_physicsEntities.clear();
		m_collidedEntities.clear();
	}

	//calculate boundingbox
	void Physics::m_CalculateBoundingBox() {
		//std::cout << "********************************************************************************" << std::endl;
		//std::cout << "UPDATING BOUNDING BOX" << std::endl;
		for (size_t i = 0; i < m_physicsEntities.size(); ++i) {
			if (m_physicsEntities[i]->GetEntity() == EntityType::RECTANGLE) {
				AABB boundingBox;
				boundingBox.m_min = { m_physicsEntities[i]->m_position.m_x - (m_physicsEntities[i]->m_scale.m_x * 0.5f), m_physicsEntities[i]->m_position.m_y - (m_physicsEntities[i]->m_scale.m_y * 0.5f) };
				boundingBox.m_max = { m_physicsEntities[i]->m_position.m_x + (m_physicsEntities[i]->m_scale.m_x * 0.5f), m_physicsEntities[i]->m_position.m_y + (m_physicsEntities[i]->m_scale.m_y * 0.5f) };
				dynamic_cast<Rectangle*>(m_physicsEntities[i].get())->m_boundingBox = boundingBox;
			}
			else if (m_physicsEntities[i]->GetEntity() == EntityType::CIRCLE) {

			}
			else {

			}
		}
		//std::cout << "********************************************************************************" << std::endl;
	}


	//static dynamic collision
	bool Physics::m_CollisionIntersection_RectRect(const Rectangle& obj1, const Rectangle& obj2, float dt) {
		//static collision
		AABB aabb1 = obj1.m_boundingBox;
		AABB aabb2 = obj2.m_boundingBox;
		vector2::Vec2 obj1_Velocity = obj1.m_velocity;
		vector2::Vec2 obj2_Velocity = obj2.m_velocity;
		vector2::Vec2 rel_Velocity{};
		float Tfirst{}, Tlast = dt;
		rel_Velocity = obj1_Velocity - obj2_Velocity;


		/*std::cout << "CHECKING" << std::endl;
		std::cout << "ID " << obj1.ID << " ID " << obj2.ID << std::endl;
		std::cout << "********************************************************************************" << std::endl;
		std::cout << "obj1_Velocity x " << obj1_Velocity.x << " y " << obj1_Velocity.y << std::endl;
		std::cout << "obj2_Velocity x " << obj2_Velocity.x << " y " << obj2_Velocity.y << std::endl;
		std::cout << "rel_Velocity x " << rel_Velocity.x << " y " << rel_Velocity.y << std::endl;
		std::cout << "********************************************************************************" << std::endl;
		std::cout << "AABB1 min x " << aabb1.min.x << " y " << aabb1.min.y << std::endl;
		std::cout << "AABB1 max x " << aabb1.max.x << " y " << aabb1.max.x << std::endl;
		std::cout << "AABB2 min x " << aabb2.min.x << " y " << aabb2.min.y << std::endl;
		std::cout << "AABB2 max x " << aabb2.max.x << " y " << aabb2.max.x << std::endl;
		std::cout << "********************************************************************************" << std::endl;*/


		//static collision
		if (m_static_CollisionCheck(aabb1, aabb2)) return true;
		//dynamic collision
		if (rel_Velocity.m_x < 0) {
			if (aabb2.m_min.m_x > aabb1.m_max.m_x) return 0;
			if (aabb2.m_max.m_x < aabb1.m_min.m_x) {
				Tfirst = std::max((aabb2.m_max.m_x - aabb1.m_min.m_x) / rel_Velocity.m_x, Tfirst);
			}
			if (aabb2.m_min.m_x < aabb1.m_max.m_x) {// case 4
				Tlast = std::min((aabb2.m_min.m_x - aabb1.m_max.m_x) / rel_Velocity.m_x, Tlast);
			}
		}
		else if (rel_Velocity.m_x > 0) {
			if (aabb2.m_min.m_x > aabb1.m_max.m_x) {// case 2
				Tfirst = std::max((aabb2.m_min.m_x - aabb1.m_max.m_x) / rel_Velocity.m_x, Tfirst);
			}
			if (aabb2.m_max.m_x > aabb1.m_min.m_x) {// case 2
				Tlast = std::min((aabb2.m_max.m_x - aabb1.m_min.m_x) / rel_Velocity.m_x, Tlast);
			}
			if (aabb2.m_max.m_x < aabb1.m_min.m_x)return 0;//case 3
		}
		else if (rel_Velocity.m_x == 0) {//case 5
			if (aabb2.m_max.m_x < aabb1.m_min.m_x) return 0;
			else if (aabb2.m_min.m_x > aabb1.m_max.m_x) return 0;
		}

		if (Tfirst > Tlast) return 0;


		if (rel_Velocity.m_y < 0) {
			if (aabb2.m_min.m_y > aabb1.m_max.m_y) return 0;//case 1
			if (aabb2.m_max.m_y < aabb1.m_min.m_y) {// case 4
				Tfirst = std::max((aabb2.m_max.m_y - aabb1.m_min.m_y) / rel_Velocity.m_y, Tfirst);
			}
			if (aabb2.m_min.m_y < aabb1.m_max.m_y) {// case 4
				Tlast = std::min((aabb2.m_min.m_y - aabb1.m_max.m_y) / rel_Velocity.m_y, Tlast);
			}
		}
		else if (rel_Velocity.m_y > 0) {
			if (aabb2.m_min.m_y > aabb1.m_max.m_y) {// case 2
				Tfirst = std::max((aabb2.m_min.m_y - aabb1.m_max.m_y) / rel_Velocity.m_y, Tfirst);
			}
			if (aabb2.m_max.m_y > aabb1.m_min.m_y) {// case 2
				Tlast = std::min((aabb2.m_max.m_y - aabb1.m_min.m_y) / rel_Velocity.m_y, Tlast);
			}
			if (aabb2.m_max.m_y < aabb1.m_min.m_y)return 0;//case 3
		}
		else if (rel_Velocity.m_y == 0) {
			if (aabb2.m_max.m_y < aabb1.m_min.m_y) return 0;
			else if (aabb2.m_min.m_y > aabb1.m_max.m_y) return 0;
		}
		if (Tfirst > Tlast) return 0;

		return true;

	}
	bool Physics::m_CollisionIntersection_CircleCircle(const Circle& circle1, const Circle& circle2) {
		float dx = circle2.m_position.m_x - circle1.m_position.m_x;
		float dy = circle2.m_position.m_y - circle1.m_position.m_y;
		float distance_Square = dx * dx + dy * dy;

		float combineRadius = circle1.m_radius + circle2.m_radius;
		return distance_Square <= (combineRadius * combineRadius);
	}

	bool Physics::m_CollisionIntersection_CircleRect(const Circle& circle, const Rectangle& rect) {
		vector2::Vec2 shortestDistance{};

		
		if (circle.m_position.m_x < rect.m_boundingBox.m_min.m_x) shortestDistance.m_x = rect.m_boundingBox.m_min.m_x;
		else if (circle.m_position.m_x > rect.m_boundingBox.m_max.m_x) shortestDistance.m_x = rect.m_boundingBox.m_max.m_x;
		else shortestDistance.m_x = circle.m_position.m_x;

		if (circle.m_position.m_y < rect.m_boundingBox.m_min.m_y) shortestDistance.m_y = rect.m_boundingBox.m_min.m_y;
		else if (circle.m_position.m_y > rect.m_boundingBox.m_max.m_y) shortestDistance.m_y = rect.m_boundingBox.m_max.m_y;
		else shortestDistance.m_y = circle.m_position.m_y;


		float Y_Square = static_cast<float>(pow((circle.m_position.m_x - shortestDistance.m_x), 2));
		float X_Square = static_cast<float>(pow((circle.m_position.m_y - shortestDistance.m_y), 2));
		float distance_Square = Y_Square + X_Square;

		return distance_Square <= (circle.m_radius * circle.m_radius);
	}
}