#include "CollisionManager.h"
#include "../Components/PhysicsBody.h"

CollisionManager* CollisionManager::instance = nullptr;
std::list<Contact*> CollisionManager::mContacts;


void CollisionManager::destroySingleton()
{
	Reset();
	delete instance;
}

// End of singleton stuff //
// ------------------------------------------------------ //

Shape::Shape(ShapeType Type)
{
	mpOwnerBody = nullptr;
	mType = Type;
}

ShapeCircle::ShapeCircle(float radius) : Shape(Shape::ShapeType::CIRCLE)
{
	mRadius = radius;
}

bool ShapeCircle::testPoint(glm::vec2 point)
{
	// Center-point squared distance. GLM might have a built-in function for this but I couldn't find it.
	float sqDist = (point.x - mpOwnerBody->mPosition.x) * (point.x - mpOwnerBody->mPosition.x) 
		+ (point.y - mpOwnerBody->mPosition.y) * (point.y - mpOwnerBody->mPosition.y);
	
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
	left = mpOwnerBody->mPosition.x - mLeft;
	right = mpOwnerBody->mPosition.x + mRight;
	top = mpOwnerBody->mPosition.y + mTop;
	bottom = mpOwnerBody->mPosition.y - mBottom;

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

// Helper function.
void addContact(Shape* pShape1, Shape* pShape2, std::list<Contact*>& mContacts)
{
	// Create the new contact.
	Contact* pNewContact = new Contact();

	// Add the physics bodies.
	pNewContact->mBodies[0] = pShape1->mpOwnerBody;
	pNewContact->mBodies[1] = pShape2->mpOwnerBody;

	// Push new contact onto mContacts.
	mContacts.push_back(pNewContact);
}

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
	addContact(pCircleShape1, pCircleShape2, mContacts);

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
	addContact(pAABBShape1, pAABBShape2, mContacts);

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
	addContact(pCircleShape1, pAABBShape2, mContacts);

	return true;
}

bool checkCollisionAABBCircle(Shape* pAABBShape1, glm::vec2 pos1, Shape* pCircleShape2, glm::vec2 pos2, std::list<Contact*>& mContacts)
{
	return checkCollisionCircleAABB(pCircleShape2, pos2, pAABBShape1, pos1, mContacts);
}

CollisionManager::CollisionManager()
{
	// Initialize the 2d array of collisions function pointers.
	CollisionFunctions[Shape::ShapeType::CIRCLE][Shape::ShapeType::CIRCLE] = checkCollisionCircleCircle;
	CollisionFunctions[Shape::ShapeType::AABB][Shape::ShapeType::AABB] = checkCollisionAABBAABB;
	CollisionFunctions[Shape::ShapeType::CIRCLE][Shape::ShapeType::AABB] = checkCollisionCircleAABB;
	CollisionFunctions[Shape::ShapeType::AABB][Shape::ShapeType::CIRCLE] = checkCollisionAABBCircle;
}


// This is the exposed function which calls the above four!
bool CollisionManager::checkCollisionAndGenerateContact(Shape* pShape1, glm::vec2 pos1, Shape* pShape2, glm::vec2 pos2)
{
	return CollisionFunctions[pShape1->mType][pShape2->mType](pShape1, pos1, pShape2, pos2, mContacts);
}


void CollisionManager::Reset()
{
	for (auto c : mContacts)
		delete c;
	mContacts.clear();
}



