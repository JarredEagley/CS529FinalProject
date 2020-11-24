#pragma once
#include <list>
#include "glm/vec2.hpp"

class PhysicsBody;

class Shape
{
public:
	enum ShapeType
	{
		CIRCLE,
		AABB,

		NUM
	};

	Shape(ShapeType Type);
	virtual ~Shape() = 0;

	virtual bool testPoint(glm::vec2 point) = 0;

public:
	PhysicsBody* mpOwnerPhysics;
	ShapeType mType;

private:
private:
};


class ShapeCircle : public Shape
{
public:
	ShapeCircle(float radius);
	~ShapeCircle() {};

	bool testPoint(glm::vec2 point);

public:
	float mRadius;
private:
private:
};


class ShapeAABB : public Shape
{
public:
	ShapeAABB(float left, float right, float top, float bottom);
	~ShapeAABB() {};

	bool testPoint(glm::vec2 point);

public:
	float mLeft, mRight, mTop, mBottom;
private:
private:
};


// ------------------------------------------------------ //

// Stores everything you need to know about two shapes hitting eachother.
class Contact
{
public:
	Contact()
	{
		mBodies[0] = mBodies[1] = nullptr;
	}
	~Contact() {};
public:
	PhysicsBody* mBodies[2];
};

// ------------------------------------------------------ //

class CollisionManager
{
public:
	static CollisionManager* getInstance()
	{
		if (!instance)
			instance = new CollisionManager;
		return instance;
	};
	void destroySingleton();

	void Reset(); // Deletes all contacts.
	bool checkCollisionAndGenerateContact(Shape* pShape1, glm::vec2 pos1, Shape* pShape2, glm::vec2 pos2 );

public:
	std::list<Contact*> mContacts;

	// 2D Array of function pointers used to store collision functions.
	bool (*CollisionFunctions[Shape::ShapeType::NUM][Shape::ShapeType::NUM])(Shape* pShape1, glm::vec2 pos1, Shape* pShape2, glm::vec2 pos2, std::list<Contact*>& mContacts);

private:
	CollisionManager(); // Private ctor

private:
	static CollisionManager* instance;
};

