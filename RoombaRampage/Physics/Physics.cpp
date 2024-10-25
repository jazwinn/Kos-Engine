/******************************************************************/
/*!
\file      Physics.cpp
\author    Rayner Tan, raynerweichen.tan , 2301449
\par       raynerweichen.tan@digipen.edu
\date      Sept 28, 2024
\brief     This file implements the physics pipeline, including
		   entity creation, bounding box calculations, and
		   collision detection for various entity types such as
		   circles and rectangles.

The file provides functionality to send, retrieve, and check
physics data for entities, using collision algorithms for
circle-circle, circle-rectangle, and rectangle-rectangle interactions.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#include "Physics.h"
namespace physicspipe {

	std::vector<std::shared_ptr<PhysicsData>> Physics::m_physicsEntities;
	std::vector<std::shared_ptr<PhysicsData>> Physics::m_collidedEntities;
	std::map<layer::LAYERS, std::vector<std::shared_ptr<PhysicsData>>> Physics::m_layerToEntities;
	physicslayer::PhysicsLayer* physicsLayer = physicslayer::PhysicsLayer::getInstance(); // Get the PhysicsLayer instance
	std::vector<int> Physics::m_checker{};
	//std::unordered_set <std::pair<std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>>, PairHash> Physics::checker;
	//std::set<std::pair<int, int>>Physics::checker;

	Circle::Circle(float radius, vector2::Vec2 shape_position, vector2::Vec2 shape_scale, vector2::Vec2 shape_velocity, int entity_ID)
		: m_radius(radius)   // Initialize radius
	{
		m_position = shape_position;
		m_scale = shape_scale;
		m_velocity = shape_velocity;
		m_ID = entity_ID;
		type = EntityType::CIRCLE;  // Set type to Circle
	}


	Rectangle::Rectangle(float rect_height, float rect_width, float rect_angle, vector2::Vec2 shape_position, vector2::Vec2 shape_scale, vector2::Vec2 shape_velocity, int entity_ID)
		: m_height(rect_height), m_width(rect_width)   // Initialize height and width
	{
		m_position = shape_position;
		m_scale = shape_scale;
		m_velocity = shape_velocity;
		m_ID = entity_ID;
		m_rotAngle = rect_angle;
		type = EntityType::RECTANGLE;  // Set type to Rectangle
	}

	bool Physics::m_static_CollisionCheck(const AABB aabb1, const AABB aabb2) {
		//std::cout << "BOUNDING BOX1 MIN X " << aabb1.m_min.m_x << " Y " << aabb1.m_min.m_y << std::endl;
		//std::cout << "BOUNDING BOX1 MAX X " << aabb1.m_max.m_x << " Y " << aabb1.m_max.m_y << std::endl;
		//std::cout << "BOUNDING BOX2 MIN X " << aabb2.m_min.m_x << " Y " << aabb2.m_min.m_y << std::endl;
		//std::cout << "BOUNDING BOX2 MAX X " << aabb2.m_max.m_x << " Y " << aabb2.m_max.m_y << std::endl;

		if (aabb2.m_max.m_x < aabb1.m_min.m_x) return 0;
		if (aabb2.m_min.m_x > aabb1.m_max.m_x) return 0;
		if (aabb2.m_max.m_y < aabb1.m_min.m_y) return 0;
		if (aabb2.m_min.m_y > aabb1.m_max.m_y) return 0;
		//std::cout << "Static Collision fail " << std::endl;
		return 1;
	}



	void Physics::m_SendPhysicsData(float rect_height, float rect_width, float rect_angle, vector2::Vec2 position, vector2::Vec2 scale, vector2::Vec2 velocity, int ID, layer::LAYERS layerID) {
		m_physicsEntities.push_back(std::make_shared<Rectangle>(rect_height, rect_width, rect_angle, position, scale, velocity, ID));
		m_layerToEntities[layerID].push_back(std::make_shared<Rectangle>(rect_height, rect_width, rect_angle, position, scale, velocity, ID));
	}

	void Physics::m_SendPhysicsData(float radius, vector2::Vec2 position, vector2::Vec2 scale, vector2::Vec2 velocity, int ID, layer::LAYERS layerID) {
		m_physicsEntities.push_back(std::make_shared<Circle>(radius, position, scale, velocity, ID));
		m_layerToEntities[layerID].push_back(std::make_shared<Circle>(radius, position, scale, velocity, ID));
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
						if (m_CollisionIntersection_RectRect(*dynamic_cast<Rectangle*>(m_physicsEntities[i].get()), *dynamic_cast<Rectangle*>(m_physicsEntities[j].get()), dt)) {
							//checking whether if entity is alr added inside
							if (std::find(m_collidedEntities.begin(), m_collidedEntities.end(), m_physicsEntities[i]) == m_collidedEntities.end()) {
								//std::cout << "Collided" << std::endl;
								//LOGGING_DEBUG("COLLIDED"); // Dont LOG uses up too much process
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
								//std::cout << "Collided" << std::endl;
								//LOGGING_DEBUG("COLLIDED");// Dont LOG uses up too much process
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
								//std::cout << "Collided" << std::endl;
								//LOGGING_DEBUG("COLLIDED"); // Dont LOG uses up too much process
								m_collidedEntities.push_back(m_physicsEntities[i]);
							}
						}
					}
					/*************************************
						CHECK CIRCLE V CIRCLE
					*************************************/
					else if (m_physicsEntities[i]->GetEntity() == EntityType::CIRCLE && m_physicsEntities[j]->GetEntity() == EntityType::CIRCLE) {
						if (m_CollisionIntersection_CircleCircle(*dynamic_cast<Circle*>(m_physicsEntities[i].get()), *dynamic_cast<Circle*>(m_physicsEntities[j].get()))) {
							if (std::find(m_collidedEntities.begin(), m_collidedEntities.end(), m_physicsEntities[i]) == m_collidedEntities.end()) {
								//std::cout << "Collided" << std::endl;
								//LOGGING_DEBUG("COLLIDED");// Dont LOG uses up too much process
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
		//std::cout << "Physics Entities size " << m_physicsEntities.size() << " collideEntities " << m_collidedEntities.size() << std::endl;
	}

	std::vector<std::shared_ptr<PhysicsData>> Physics::m_RetrievePhysicsData() {
		std::vector<std::shared_ptr	<PhysicsData>> TempCollidedEntities = m_collidedEntities;

		this->m_ClearEntites();
		return TempCollidedEntities;
	}

	void Physics::m_ClearEntites() {
		m_physicsEntities.clear();
		m_collidedEntities.clear();
		m_layerToEntities.clear();
		m_checker.clear();
	}


	void Physics::m_CalculateBoundingBox() {
		//std::cout << "********************************************************************************" << std::endl;
		//std::cout << "UPDATING BOUNDING BOX" << std::endl;
		for (size_t i = 0; i < m_physicsEntities.size(); ++i) {
			if (m_physicsEntities[i]->GetEntity() == EntityType::RECTANGLE) {
				AABB boundingBox;
				float width = dynamic_cast<Rectangle*>(m_physicsEntities[i].get())->m_width;
				float height = dynamic_cast<Rectangle*>(m_physicsEntities[i].get())->m_height;
				boundingBox.m_min = { m_physicsEntities[i]->m_position.m_x - (width * 0.5f), m_physicsEntities[i]->m_position.m_y - (height * 0.5f) };
				boundingBox.m_max = { m_physicsEntities[i]->m_position.m_x + (width * 0.5f), m_physicsEntities[i]->m_position.m_y + (height * 0.5f) };
				dynamic_cast<Rectangle*>(m_physicsEntities[i].get())->m_boundingBox = boundingBox;
				//std::cout << "ID " << m_physicsEntities[i].get()->m_ID << " BOUNDING BOX MIN X" << boundingBox.m_min.m_x << " MIN Y " << boundingBox.m_max.m_y << " MAX X " << boundingBox.m_max.m_x << " MAX Y" << boundingBox.m_max.m_y << std::endl;
			}
			else if (m_physicsEntities[i]->GetEntity() == EntityType::CIRCLE) {

			}
			else {

			}
		}

		for (int i = 0; i < physicslayer::size; ++i) {
			for (int j = 0; j < m_layerToEntities[static_cast<layer::LAYERS>(i)].size(); ++j) {
				if ((m_layerToEntities[static_cast<layer::LAYERS>(i)].at(j)).get()->GetEntity() == EntityType::RECTANGLE) {
					AABB boundingBox;
					float width = dynamic_cast<Rectangle*>(m_layerToEntities[static_cast<layer::LAYERS>(i)].at(j).get())->m_width;
					float height = dynamic_cast<Rectangle*>(m_layerToEntities[static_cast<layer::LAYERS>(i)].at(j).get())->m_height;
					boundingBox.m_min = { m_layerToEntities[static_cast<layer::LAYERS>(i)].at(j).get()->m_position.m_x - (width * 0.5f), m_layerToEntities[static_cast<layer::LAYERS>(i)].at(j).get()->m_position.m_y - (height * 0.5f) };
					boundingBox.m_max = { m_layerToEntities[static_cast<layer::LAYERS>(i)].at(j).get()->m_position.m_x + (width * 0.5f), m_layerToEntities[static_cast<layer::LAYERS>(i)].at(j).get()->m_position.m_y + (height * 0.5f) };
					dynamic_cast<Rectangle*>(m_layerToEntities[static_cast<layer::LAYERS>(i)].at(j).get())->m_boundingBox = boundingBox;
					//dynamic_cast<Rectangle*>(m_physicsEntities[i].get())->m_boundingBox = boundingBox;
					//std::cout << "ID " << m_physicsEntities[i].get()->m_ID << " BOUNDING BOX MIN X" << boundingBox.m_min.m_x << " MIN Y " << boundingBox.m_max.m_y << " MAX X " << boundingBox.m_max.m_x << " MAX Y" << boundingBox.m_max.m_y << std::endl;
				}
				else if (m_physicsEntities[i]->GetEntity() == EntityType::CIRCLE) {

				}
				else {

				}
			}
		}
		//std::cout << "********************************************************************************" << std::endl;
	}

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


	void Physics::m_Init() {

	}
	void Physics::m_Update(float dt) {
		/*
			1. Update the layer matrix first
			2. Using the update layer matrix do the collision check
		*/

		m_CollisionCheckUpdate(dt);
	}

	void Physics::m_CollisionCheckUpdate(float dt) {

		if (m_physicsEntities.empty()) return;

		m_CalculateBoundingBox();


		std::vector<std::vector<bool>> collisionMatrix = physicsLayer->getMatrix();
		// Iterate through all pairs of layers using the map keys
		for (auto it1 = m_layerToEntities.begin(); it1 != m_layerToEntities.end(); ++it1) {
			for (auto it2 = it1; it2 != m_layerToEntities.end(); ++it2) {
				// Get the layer enum values
				layer::LAYERS layer1 = it1->first;
				layer::LAYERS layer2 = it2->first;

				// Get the indices for the collision matrix (convert enum to int if necessary)
				int index1 = static_cast<int>(layer1);
				int index2 = static_cast<int>(layer2);

				// Check the collision matrix to see if we need to check for collisions between these layers
				if (collisionMatrix[index1][index2]) {
					// Get the entity vectors for both layers
					const auto& entities1 = it1->second;
					const auto& entities2 = it2->second;

					// Perform collision checks between all entities in entities1 and entities2
					for (const auto& entity1 : entities1) {
						for (const auto& entity2 : entities2) {
							// Ensure you don't check an entity with itself if layer1 == layer2
							if (layer1 == layer2 && entity1 == entity2) continue;

							// Perform the actual collision check between entity1 and entity2
							if (CheckCollision(entity1, entity2, dt)) {
							//if (shouldCollide(entity1, entity2)) {
								std::cout << "Collision detected between entity "
									<< entity1->m_ID << " and entity "
									<< entity2->m_ID << std::endl;

								// Store or process the collided entities as needed.
								if (std::find(m_collidedEntities.begin(), m_collidedEntities.end(), entity1) != m_collidedEntities.end()) {
									m_collidedEntities.push_back(entity1);
								}
								if (std::find(m_collidedEntities.begin(), m_collidedEntities.end(), entity2) != m_collidedEntities.end()) {
									m_collidedEntities.push_back(entity2);
								}
							}
						}
					}
				}
			}
		}
	}
	bool Physics::CheckCollision(const std::shared_ptr<PhysicsData>& entity1, const std::shared_ptr<PhysicsData>& entity2, float dt){
		// Check for collision based on the types of entities.
		if (entity1->GetEntity() == EntityType::CIRCLE && entity2->GetEntity() == EntityType::CIRCLE) {
			return m_CollisionIntersection_CircleCircle(*static_cast<Circle*>(entity1.get()), *static_cast<Circle*>(entity2.get()));
		}
		else if (entity1->GetEntity() == EntityType::RECTANGLE && entity2->GetEntity() == EntityType::RECTANGLE) {
			return m_TestCollisionIntersection_RectRect(*static_cast<Rectangle*>(entity1.get()), *static_cast<Rectangle*>(entity2.get()));
			//return m_CollisionIntersection_RectRect(*static_cast<Rectangle*>(entity1.get()), *static_cast<Rectangle*>(entity2.get()), dt);
		}
		else if (entity1->GetEntity() == EntityType::CIRCLE && entity2->GetEntity() == EntityType::RECTANGLE) {
			return m_CollisionIntersection_CircleRect(*static_cast<Circle*>(entity1.get()), *static_cast<Rectangle*>(entity2.get()));
		}
		else if (entity1->GetEntity() == EntityType::RECTANGLE && entity2->GetEntity() == EntityType::CIRCLE) {
			return m_CollisionIntersection_CircleRect(*static_cast<Circle*>(entity2.get()), *static_cast<Rectangle*>(entity1.get()));
		}

		return false;  // If no valid collision type, return false.
	}

	bool Physics::shouldCollide(const std::shared_ptr<PhysicsData>& entity1, const std::shared_ptr<PhysicsData>& entity2 ) {

		int counter = 0;
		for (int i = 0; i < m_checker.size(); ++i) {
			if (m_checker[i] == entity1.get()->m_ID || m_checker[i] == entity2.get()->m_ID) {
				counter++;
			}
		}

		// Add the pair to the set of checked pairs
		if (counter == 2) {
			return false;
		}
		m_checker.push_back(entity1.get()->m_ID);
		m_checker.push_back(entity2.get()->m_ID);
		//std::cout << m_checker.size() << std::endl;
		return true;
	}

	std::vector<vector2::Vec2> Rectangle::getRotatedVertices() const {
		std::vector<vector2::Vec2> vertices(4);
		float cosTheta = mathlibrary::mathlib::funcCos(mathlibrary::mathlib::funcDegreeToRadian(m_rotAngle));
		float sinTheta = mathlibrary::mathlib::funcSin(mathlibrary::mathlib::funcDegreeToRadian(m_rotAngle));

		// Calculate the half-width and half-height
		float hw = m_width * 0.5f;
		float hh = m_height * 0.5f;

		// Center of the rectangle
		vector2::Vec2 center = m_position;

		// Calculate the rotated positions of each corner
		vertices[0] = { center.m_x - hw * cosTheta + hh * sinTheta, center.m_y - hw * sinTheta - hh * cosTheta }; // Top-left
		vertices[1] = { center.m_x + hw * cosTheta + hh * sinTheta, center.m_y + hw * sinTheta - hh * cosTheta }; // Top-right
		vertices[2] = { center.m_x + hw * cosTheta - hh * sinTheta, center.m_y + hw * sinTheta + hh * cosTheta }; // Bottom-right
		vertices[3] = { center.m_x - hw * cosTheta - hh * sinTheta, center.m_y - hw * sinTheta + hh * cosTheta }; // Bottom-left

		return vertices;
	}

	std::vector<vector2::Vec2> Rectangle::getEdges() const {
		std::vector<vector2::Vec2> vertices = getRotatedVertices();
		std::vector<vector2::Vec2> edges(4);
		for (size_t i = 0; i < 4; ++i) {
			vector2::Vec2 p1 = vertices[i];
			vector2::Vec2 p2 = vertices[(i + 1) % 4];
			edges[i] = { p2.m_x - p1.m_x, p2.m_y - p1.m_y };
		}
		return edges;
	}

	void Physics::projectOntoAxis(const std::vector<vector2::Vec2>& vertices, const vector2::Vec2& axis, float& min, float& max) const {
		//min = max = (vertices[0].m_x * axis.m_x + vertices[0].m_y * axis.m_y);
		min = max = vector2::Vec2::m_funcVec2DDotProduct(vertices[0], axis);
		for (const auto& vertex : vertices) {
			//float projection = vertex.m_x * axis.m_x + vertex.m_y * axis.m_y;
			float projection = vector2::Vec2::m_funcVec2DDotProduct(vertex, axis);
			if (projection < min) min = projection;
			if (projection > max) max = projection;
		}
	}

	bool Physics::m_TestCollisionIntersection_RectRect(const Rectangle& obj1, const Rectangle& obj2) {
		// Get rotated vertices and edges
		std::vector<vector2::Vec2> verticesA = obj1.getRotatedVertices();
		std::vector<vector2::Vec2> verticesB = obj2.getRotatedVertices();
		std::vector<vector2::Vec2> edgesA = obj1.getEdges();
		std::vector<vector2::Vec2> edgesB = obj2.getEdges();

		// List of all the axes (normals of edges)
		std::vector<vector2::Vec2> axes;
		for (const auto& edge : edgesA) {
			axes.push_back({ -edge.m_y, edge.m_x }); // Perpendicular vector
		}
		for (const auto& edge : edgesB) {
			axes.push_back({ -edge.m_y, edge.m_x });
		}

		// Check for overlap on all axes
		for (const auto& axis : axes) {
			float minA, maxA, minB, maxB;
			//using dot product to determine
			projectOntoAxis(verticesA, axis, minA, maxA);
			projectOntoAxis(verticesB, axis, minB, maxB);

			// If there's a gap, no collision
			if (maxA < minB || maxB < minA) {
				return false;
			}
		}
		// No separating axis found, so there's a collision
		return true;
	}
}