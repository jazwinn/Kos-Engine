#include "../Physics.h"

namespace PHYSICS {
	void classPhysics::funcRetrievePhysicsData() {
		//get the data store
		PhysicsData obj;
		obj.m_radius = 5.0f;
		physicsEntities.push_back(obj);
		//calculate the boundingbox
	}
	void classPhysics::funcCollisionCheck() {
		/*
			loop thru everything
			if player
			following GIT Asteroids
		*/

		//calculate boarding box
		funcCalculateBoundingBox();

		for (size_t i = 0; i < physicsEntities.size(); ++i) {
			for (size_t j = 0; j < physicsEntities.size(); ++j) {
				//checking of which type
				if (physicsEntities[i].ID != physicsEntities[j].ID) {
					if (funcCollisionIntersection_RectRect(physicsEntities[i], physicsEntities[j])) {
						//checking whether if entity is alr added inside
						if (std::find(collidedEntities.begin(), collidedEntities.end(), physicsEntities[i]) == collidedEntities.end()){
							collidedEntities.push_back(physicsEntities[i]);
						}
					}
				}
			}
		}
	}

	std::vector<PhysicsData> classPhysics::funcPassPhysicsData() const{
		return collidedEntities;
	}

	//calculate boundingbox
	void classPhysics::funcCalculateBoundingBox() {
		for (size_t i = 0; i < physicsEntities.size(); ++i) {
			AABB boundingBox; 
			boundingBox.min = { physicsEntities[i].center.x - (bounding_rect_size * 0.5f), physicsEntities[i].center.y - (bounding_rect_size * 0.5f)};
			boundingBox.max = { physicsEntities[i].center.x + (bounding_rect_size * 0.5f), physicsEntities[i].center.y + (bounding_rect_size * 0.5f) };
			physicsEntities[i].boundingBox = boundingBox;
		}
	}


	//static dynamic collision
	bool classPhysics::funcCollisionIntersection_RectRect(const PhysicsData& obj1, const PhysicsData& obj2) {
		//static collision
		if (obj1.boundingBox.max.x < obj2.boundingBox.min.x || obj1.boundingBox.min.x > obj2.boundingBox.max.x) return 0;
		if (obj1.boundingBox.max.y < obj2.boundingBox.min.y || obj1.boundingBox.min.y > obj2.boundingBox.max.y) return 0;


		//dynamic collision
		float bX = obj2.velocity.x;
		float bY = obj2.velocity.y;
		float aX = obj1.velocity.x;
		float aY = obj1.velocity.y;

		float relVel = bX - aX;
		aX -= aX;
		float dFirst = 0.0f, dLast = 0.0f, tFirst = 0.0f, tLast = 0.0f, tFirstXY = 0.0f, tLastXY = 0.0f;
		//Calculate Distance for X only
		dFirst = obj1.boundingBox.min.x - obj2.boundingBox.max.x;
		dLast = obj1.boundingBox.max.x - obj2.boundingBox.min.x;

		if (relVel < 0) {
			if (obj1.boundingBox.min.x > obj2.boundingBox.max.x) return 0;
			if (obj1.boundingBox.max.x < obj2.boundingBox.min.x) {
				tFirstXY = dFirst / relVel;
				tLast = dLast / relVel;
				tFirst = tFirst < tFirstXY ? tFirstXY : tFirst;
				tLast = tLast < tLastXY ? tLast : tLastXY;
			}
		}
		else if (relVel > 0) {
			if (obj1.boundingBox.max.x > obj2.boundingBox.min.x) {
				tFirstXY = (dFirst / relVel);
				tLastXY = (dLast / relVel);
				tFirst = tFirst < tFirst ? tFirstXY : tFirst;
				tLast  = tLastXY > tLast ? tLast : tLastXY;
			}
			//case 3
			if (obj1.boundingBox.max.x < obj2.boundingBox.min.x) return 0;
		}
		else {
			if (obj1.boundingBox.max.x < obj2.boundingBox.min.x) return 0;
			else if (obj1.boundingBox.min.x > obj2.boundingBox.max.x) return 0;
		}

		if (tFirst > tLast) return 0;

		tFirst = 0.0f;
		tLast = 0.0f;

		relVel = bY - aY;
		aY -= aY;

		dFirst = obj1.boundingBox.min.y - obj2.boundingBox.max.y;
		dLast = obj1.boundingBox.max.y - obj2.boundingBox.min.y;
		tFirstXY = 0.0f;
		tLastXY = 0.0f;

		if (relVel < 0) {
			if (obj1.boundingBox.min.y > obj2.boundingBox.max.y) return 0;
			if (obj1.boundingBox.max.y < obj2.boundingBox.min.y) {
				tFirstXY = dFirst / relVel;
				tLastXY = dLast / relVel;
				tFirst = tFirst < tFirstXY ? tFirstXY : tFirst;
				tLast = tLastXY > tLast ? tLast : tLastXY;
			}
		}
		else if (relVel > 0) {
			if (obj1.boundingBox.min.y > obj2.boundingBox.max.y) {
				tFirstXY = dFirst / relVel;
				tLastXY = dLast / relVel;
				tFirst = tFirst < tFirstXY ? tFirstXY : tFirst;
				tLast = tLastXY > tLast ? tLast : tLastXY;
			}
			if (obj1.boundingBox.max.y < obj2.boundingBox.min.y) return 0;
		}
		else {
			if (obj1.boundingBox.max.y < obj2.boundingBox.min.y) return 0;
			else if (obj1.boundingBox.min.y > obj2.boundingBox.max.y) return 0;
		}

		if (tFirst > tLast) return 0;

		return true;

	}


	bool classPhysics::funcCollisionIntersection_CircleRect(const PhysicsData& circle, const PhysicsData& rect) {
		Vector2::Vec2 shortestDistance{};


		if (circle.center.x < rect.boundingBox.min.x) shortestDistance.x = rect.boundingBox.min.x;
		else if (circle.center.x > rect.boundingBox.max.x) shortestDistance.x = rect.boundingBox.max.x;
		else shortestDistance.x = circle.center.x;

		if (circle.center.y < rect.boundingBox.min.y) shortestDistance.y = rect.boundingBox.min.y;
		else if (circle.center.y > rect.boundingBox.max.y) shortestDistance.y = rect.boundingBox.max.y;
		else shortestDistance.y = circle.center.y;


		float Y_Square = static_cast<float>(pow((circle.center.x - shortestDistance.x), 2));
		float X_Square = static_cast<float>(pow((circle.center.y - shortestDistance.y), 2));
		float distance_Square = Y_Square + X_Square;

		return distance_Square <= (circle.m_radius * circle.m_radius);
	}
}