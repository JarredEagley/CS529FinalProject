#include "CollisionManager.h"
#include "../Components/PhysicsBody.h"

CollisionManager* CollisionManager::instance = nullptr;

void CollisionManager::destroySingleton()
{
	delete instance;
}

// End of singleton stuff //
// ------------------------------------------------------ //

Shape::Shape(ShapeType Type)
{
	mpOwnerPhysics = nullptr;
	mType = Type;
}

ShapeCircle::ShapeCircle(float radius) : Shape(Shape::ShapeType::CIRCLE)
{
	mRadius = radius;
}

bool ShapeCircle::testPoint(glm::vec2 point)
{
	// Center-point squared distance. GLM might have a built-in function for this but I couldn't find it.
	float sqDist = (point.x - mpOwnerPhysics->mPosition.x) * (point.x - mpOwnerPhysics->mPosition.x) 
		+ (point.y - mpOwnerPhysics->mPosition.y) * (point.y - mpOwnerPhysics->mPosition.y);
	
	// Compare to squared radius.
	if (sqDist > (mRadius * mRadius))
		return false;

	return true;
}


ShapeAABB::ShapeAABB(float left, float right, float top, float bottom) : Shape(Shape::ShapeType::AABB)
{
	mLeft = left;
	mRight = right;
	mTop = top;
	mBottom = bottom;
}

bool ShapeAABB::testPoint(glm::vec2 point)
{
	float left, right, top, bottom;
	left = mpOwnerPhysics->mPosition.x - mLeft;
	right = mpOwnerPhysics->mPosition.x + mRight;
	top = mpOwnerPhysics->mPosition.y + mTop;
	bottom = mpOwnerPhysics->mPosition.y - mBottom;

	if (
		point.x < left ||
		point.x > right ||
		point.y < top ||
		point.y > bottom
		)
		return false;

	return true;
}


// End of shape class stuff //
// ------------------------------------------------------ //



