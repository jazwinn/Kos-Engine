#include "Physics.h"




namespace Physics {

	std::vector<PhysicsData>  classPhysics::physicsEntities;
	std::vector<PhysicsData> classPhysics::collidedEntities;

	bool classPhysics::static_CollisionCheck(const AABB aabb1, const AABB aabb2) {
		//std::cout << "BOUNDING BOX1 MIN X " << aabb1.min.x << " Y " << aabb1.min.y << std::endl;
		//std::cout << "BOUNDING BOX1 MAX X " << aabb1.max.x << " Y " << aabb1.max.y << std::endl;
		//std::cout << "BOUNDING BOX2 MIN X " << aabb2.min.x << " Y " << aabb2.min.y << std::endl;
		//std::cout << "BOUNDING BOX2 MAX X " << aabb2.max.x << " Y " << aabb2.max.y << std::endl;

		if (aabb2.max.x < aabb1.min.x) return 0;
		if (aabb2.min.x > aabb1.max.x) return 0;
		if (aabb2.max.y < aabb1.min.y) return 0;
		if (aabb2.min.y > aabb1.max.y) return 0;
		//std::cout << "Static Collision fail " << std::endl;
		return 1;
	}

	void classPhysics::RetrievePhysicsData(Vector2::Vec2 scale,Vector2::Vec2 position, Vector2::Vec2 velocity, int ID) {
		//get the data store
		PhysicsData obj;
		obj.ID = ID;
		obj.position = position;
		obj.velocity = velocity;
		obj.scale = scale;
		physicsEntities.push_back(obj);
	}


	void classPhysics::CollisionCheck(float dt) {

		//calculate boarding box
		CalculateBoundingBox();

		for (size_t i = 0; i < physicsEntities.size(); ++i) {
			for (size_t j = 0; j < physicsEntities.size(); ++j) {
				if (physicsEntities[i].ID != physicsEntities[j].ID) {
					/*************************************
						CHECK RECT V RECT
					*************************************/
					if (physicsEntities[i].type == EntityType::Rectangle && physicsEntities[j].type == EntityType::Rectangle) {
						if (CollisionIntersection_RectRect(physicsEntities[i], physicsEntities[j], dt)) {
							//checking whether if entity is alr added inside
							if (std::find(collidedEntities.begin(), collidedEntities.end(), physicsEntities[i]) == collidedEntities.end()) {
								//std::cout << "adding Entity i" << physicsEntities[i].ID << std::endl;
								collidedEntities.push_back(physicsEntities[i]);
							}
						}
					}
					/*************************************
						CHECK CIRCLE V RECT
					*************************************/
					else if (physicsEntities[i].type == EntityType::Circle && physicsEntities[j].type == EntityType::Rectangle) {
						if (CollisionIntersection_CircleRect(physicsEntities[i], physicsEntities[j])) {
							if (std::find(collidedEntities.begin(), collidedEntities.end(), physicsEntities[i]) == collidedEntities.end()) {
								//std::cout << "adding Entity i" << physicsEntities[i].ID << std::endl;
								collidedEntities.push_back(physicsEntities[i]);
							}
						}
					}

					/*************************************
						CHECK RECT V CIRCLE
					*************************************/
					else if (physicsEntities[j].type == EntityType::Circle && physicsEntities[i].type == EntityType::Rectangle) {
						if (CollisionIntersection_CircleRect(physicsEntities[j], physicsEntities[i])) {
							if (std::find(collidedEntities.begin(), collidedEntities.end(), physicsEntities[i]) == collidedEntities.end()) {
								collidedEntities.push_back(physicsEntities[i]);
							}
						}
					}
					/*************************************
						CHECK RECT V CIRCLE
					*************************************/
					else if(physicsEntities[i].type == EntityType::Circle && physicsEntities[j].type == EntityType::Circle) {
						if (CollisionIntersection_CircleCircle(physicsEntities[i], physicsEntities[j])) {
							if (std::find(collidedEntities.begin(), collidedEntities.end(), physicsEntities[i]) == collidedEntities.end()) {
								collidedEntities.push_back(physicsEntities[i]);
							}
						}
					}
					else {

					}
				}
				//std::cout << "********************************************************************************" << std::endl;
			}
		}
		//std::cout << "Physics Entities size " << physicsEntities.size() << " collideEntities " << collidedEntities.size() << std::endl;
	}

	std::vector<PhysicsData> classPhysics::PassPhysicsData() {
		std::vector<PhysicsData> TempCollidedEntities =  collidedEntities;
		this->ClearEntites();
		return TempCollidedEntities;
	}

	void classPhysics::ClearEntites(){
		physicsEntities.clear();
		collidedEntities.clear();
	}

	//calculate boundingbox
	void classPhysics::CalculateBoundingBox() {
		//std::cout << "********************************************************************************" << std::endl;
		//std::cout << "UPDATING BOUNDING BOX" << std::endl;
		for (size_t i = 0; i < physicsEntities.size(); ++i) {
			AABB boundingBox; 
			boundingBox.min = { physicsEntities[i].position.x - (physicsEntities[i].scale.x * 0.5f), physicsEntities[i].position.y - (physicsEntities[i].scale.y * 0.5f)};
			boundingBox.max = { physicsEntities[i].position.x + (physicsEntities[i].scale.x * 0.5f), physicsEntities[i].position.y + (physicsEntities[i].scale.y * 0.5f) };
			physicsEntities[i].boundingBox = boundingBox;
			//std::cout << "BOUNDING BOX MIN X " << boundingBox.min.x << " Y " << boundingBox.min.y << std::endl;
			//std::cout << "BOUNDING BOX MAX X " << boundingBox.max.x << " Y " << boundingBox.max.y << std::endl;
		}
		//std::cout << "********************************************************************************" << std::endl;
	}


	//static dynamic collision
	bool classPhysics::CollisionIntersection_RectRect(const PhysicsData& obj1, const PhysicsData& obj2, float dt) {
		//static collision
		Physics::AABB aabb1 = obj1.boundingBox;
		Physics::AABB aabb2 = obj2.boundingBox;
		Vector2::Vec2 obj1_Velocity = obj1.velocity;
		Vector2::Vec2 obj2_Velocity = obj2.velocity;
		Vector2::Vec2 rel_Velocity{};
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
		if (static_CollisionCheck(aabb1, aabb2)) return true;
		//dynamic collision
		if (rel_Velocity.x < 0) {
			if (aabb2.min.x > aabb1.max.x) return 0;
			if (aabb2.max.x < aabb1.min.x) {
				Tfirst = std::max((aabb2.max.x - aabb1.min.x) / rel_Velocity.x, Tfirst);
			}
			if (aabb2.min.x < aabb1.max.x) {// case 4
				Tlast = std::min((aabb2.min.x - aabb1.max.x) / rel_Velocity.x, Tlast);
			}
		}
		else if (rel_Velocity.x > 0) {
			if (aabb2.min.x > aabb1.max.x) {// case 2
				Tfirst = std::max((aabb2.min.x - aabb1.max.x) / rel_Velocity.x, Tfirst);
			}
			if (aabb2.max.x > aabb1.min.x) {// case 2
				Tlast = std::min((aabb2.max.x - aabb1.min.x) / rel_Velocity.x, Tlast);
			}
			if (aabb2.max.x < aabb1.min.x)return 0;//case 3
		}
		else if (rel_Velocity.x == 0) {//case 5
			if (aabb2.max.x < aabb1.min.x) return 0;
			else if (aabb2.min.x > aabb1.max.x) return 0;
		}

		if (Tfirst > Tlast) return 0;


		if (rel_Velocity.y < 0) {
			if (aabb2.min.y > aabb1.max.y) return 0;//case 1
			if (aabb2.max.y < aabb1.min.y) {// case 4
				Tfirst = std::max((aabb2.max.y - aabb1.min.y) / rel_Velocity.y, Tfirst);
			}
			if (aabb2.min.y < aabb1.max.y) {// case 4
				Tlast = std::min((aabb2.min.y - aabb1.max.y) / rel_Velocity.y, Tlast);
			}
		}
		else if (rel_Velocity.y > 0) {
			if (aabb2.min.y > aabb1.max.y) {// case 2
				Tfirst = std::max((aabb2.min.y - aabb1.max.y) / rel_Velocity.y, Tfirst);
			}
			if (aabb2.max.y > aabb1.min.y) {// case 2
				Tlast = std::min((aabb2.max.y - aabb1.min.y) / rel_Velocity.y, Tlast);
			}
			if (aabb2.max.y < aabb1.min.y)return 0;//case 3
		}
		else if (rel_Velocity.y == 0) {
			if (aabb2.max.y < aabb1.min.y) return 0;
			else if (aabb2.min.y > aabb1.max.y) return 0;
		}
		if (Tfirst > Tlast) return 0;

		return true;

	}
	bool classPhysics::CollisionIntersection_CircleCircle(const PhysicsData& circle1, const PhysicsData& circle2) {
		float dx = circle2.position.x - circle1.position.x;
		float dy = circle2.position.y - circle1.position.y;
		float distance_Square = dx * dx + dy * dy;

		float combineRadius = circle1.m_radius + circle2.m_radius;
		return distance_Square <= (combineRadius * combineRadius);
	}

	bool classPhysics::CollisionIntersection_CircleRect(const PhysicsData& circle, const PhysicsData& rect) {
		Vector2::Vec2 shortestDistance{};

		
		if (circle.position.x < rect.boundingBox.min.x) shortestDistance.x = rect.boundingBox.min.x;
		else if (circle.position.x > rect.boundingBox.max.x) shortestDistance.x = rect.boundingBox.max.x;
		else shortestDistance.x = circle.position.x;

		if (circle.position.y < rect.boundingBox.min.y) shortestDistance.y = rect.boundingBox.min.y;
		else if (circle.position.y > rect.boundingBox.max.y) shortestDistance.y = rect.boundingBox.max.y;
		else shortestDistance.y = circle.position.y;


		float Y_Square = static_cast<float>(pow((circle.position.x - shortestDistance.x), 2));
		float X_Square = static_cast<float>(pow((circle.position.y - shortestDistance.y), 2));
		float distance_Square = Y_Square + X_Square;

		return distance_Square <= (circle.m_radius * circle.m_radius);
	}
}