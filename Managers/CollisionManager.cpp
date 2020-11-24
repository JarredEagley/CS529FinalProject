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















// ------------------------------------------------------ //


// The four hidden functions which manipulate the collision manager's contacts array.
// (Only collisionManager can see these.)

bool checkCollisionCircleCircle(Shape* pCircleShape1, glm::vec2 pos1, Shape* pCircleShape2, glm::vec2 pos2, std::list<Contact*>& mContacts)
{
	float c1c2DistSq = (pos1.x - pos2.x) * (pos1.x - pos2.x) 
		+ (pos1.y - pos2.y) * (pos1.y - pos2.y);
	float r1 = ((ShapeCircle*)pCircleShape1)->mRadius;
	float r2 = ((ShapeCircle*)pCircleShape2)->mRadius;

	if (c1c2DistSq > ((r1 + r2) * (r1 + r2)))
		return false;

	// Add a new contact.
	Contact* pNewContact = new Contact();
	pNewContact->mBodies[0] = pCircleShape1->mpOwnerPhysics;
	pNewContact->mBodies[1] = pCircleShape2->mpOwnerPhysics;
	mContacts.push_back(pNewContact);

	return true;
}

bool checkCollisionAABBAABB(Shape* pAABBShape1, glm::vec2 pos1, Shape* pAABBShape2, glm::vec2 pos2, std::list<Contact*>& mContacts)
{
	ShapeAABB* pAABB1 = (ShapeAABB*)pAABBShape1;
	ShapeAABB* pAABB2 = (ShapeAABB*)pAABBShape2;

	float left1		= pos1.x + pAABB1->mLeft;
	float right1	= pos1.x + pAABB1->mRight;
	float top1		= pos1.y + pAABB1->mTop;
	float bottom1	= pos1.y + pAABB1->mBottom;
	
	float left2		= pos2.x + pAABB2->mLeft;
	float right2	= pos2.x + pAABB2->mRight;
	float top2		= pos2.y + pAABB2->mTop;
	float bottom2	= pos2.y + pAABB2->mBottom;

	if (
		left1 > right2 ||
		left2 > right1 ||
		top1 > bottom2 ||
		top2 > bottom1
		)
		return false;

	// Add a new contact.
	Contact* pNewContact = new Contact();
	pNewContact->mBodies[0] = pAABBShape1->mpOwnerPhysics;
	pNewContact->mBodies[1] = pAABBShape2->mpOwnerPhysics;
	mContacts.push_back(pNewContact);

	return true;
}

bool checkCollisionCircleAABB(Shape* pCircleShape1, glm::vec2 pos1, Shape* pAABBShape2, glm::vec2 pos2, std::list<Contact*>& mContacts)
{
	ShapeCircle* pCircle1 = (ShapeCircle*)pCircleShape1;
	ShapeAABB* pAABB2 = (ShapeAABB*)pAABBShape2;

	float left2		= pos2.x + pAABB2->mLeft;
	float right2	= pos2.x + pAABB2->mRight;
	float top2		= pos2.y + pAABB2->mTop;
	float bottom2	= pos2.y + pAABB2->mBottom;

	float snappedX, snappedY;

	if (pos1.x < left2)
		snappedX = left2;
	else
		if (pos1.x > right2)
			snappedX = right2;
		else
			snappedX = pos2.x;

	if (pos1.y < top2)
		snappedY = top2;
	else
		if (pos1.y > bottom2)
			snappedY = bottom2;
		else
			snappedY = pos2.y;

	if (false == pCircle1->testPoint(glm::vec2(snappedX, snappedY)))
		return false;

	// Add a new contact.
	Contact* pNewContact = new Contact();
	pNewContact->mBodies[0] = pCircleShape1->mpOwnerPhysics;
	pNewContact->mBodies[1] = pAABBShape2->mpOwnerPhysics;
	mContacts.push_back(pNewContact);

	return true;
}

bool checkCollisionAABBCircle(Shape* pAABBShape1, glm::vec2 pos1, Shape* pCircleShape2, glm::vec2 pos2, std::list<Contact*>& mContacts)
{
	return checkCollisionCircleAABB(pCircleShape2, pos2, pAABBShape1, pos1, mContacts);
}


// This is the exposed function which calls the above four!
bool CollisionManager::checkCollisionAndGenerateContact(Shape* pShape1, glm::vec2 pos1, Shape* pShape2, glm::vec2 pos2)
{
	return CollisionFunctions[pShape1->mType][pShape2->mType](pShape1, pos1, pShape2, pos2, mContacts);
}


