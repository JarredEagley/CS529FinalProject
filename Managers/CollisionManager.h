#pragma once
#include "glm/vec2.hpp"

class PhysicsDynamic;

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
	PhysicsDynamic* mpOwnerPhysics;
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

public:


private:
	CollisionManager();

private:
	static CollisionManager* instance;
};

