#include "Turret.h"

#include "ComponentTypes.h"
#include "Transform.h"
#include "GLRect.h"

#include "PhysicsBody.h"

Turret::Turret() : Component(ComponentTypes::TYPE_TURRET),
mpTransform(nullptr), mpParentTransform(nullptr),mpGLRect(nullptr), mpParentGLRect(nullptr),
mAimPoint(glm::vec2(0.0f)), mAimAngle(0.0f),
mIsShooting(false)
{}

Turret::~Turret()
{
}


void Turret::Initialize()
{
	GlobalManager::getEventManager()->Subscribe(EventType::TRANSFORM_UPDATED, mpOwner);
	GlobalManager::getEventManager()->Subscribe(EventType::TURRET_COMMAND, mpOwner);
}

void Turret::Update()
{
	if (!mIsShooting)
		return;

	if (fireTimer > fireRate)
	{
		GameObject* pBullet = GlobalManager::getGameObjectFactory()->generateProjectile("CoilBullet.json");
		if (pBullet == nullptr)
			return;
		PhysicsBody* pBulletPhys = static_cast<PhysicsBody*>(pBullet->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
		PhysicsBody* pParentPhys = static_cast<PhysicsBody*>(mpOwner->getParent()->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
		Transform* pBulletTransform = static_cast<Transform*>(pBullet->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		if (pBulletPhys != nullptr && pBulletTransform != nullptr && pParentPhys != nullptr)
		{
			pBulletTransform->setPosition(mpParentTransform->getPosition());
			pBulletPhys->mVelocity = pParentPhys->mVelocity; // inherit velocity.
			glm::vec2 fireVec = glm::vec2(2.0f, 1.0f); // This is arbitrary for now.
			pBulletPhys->applyForce(fireVec);
		}
		
		fireTimer = 0;
	}
	++fireTimer;
}


void Turret::handleEvent(Event* pEvent) 
{
	if (pEvent->mType == EventType::TURRET_COMMAND)
	{
		// Try to get the necessary components to talk to.
		if (mpOwner->getParent() == nullptr)
			return;
		if (mpTransform == nullptr || mpParentTransform == nullptr || mpGLRect == nullptr || mpParentGLRect == nullptr)
		{
			mpTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
			mpParentTransform = static_cast<Transform*>(mpOwner->getParent()->GetComponent(ComponentTypes::TYPE_TRANSFORM));
			mpGLRect = static_cast<GLRect*>(mpOwner->GetComponent(ComponentTypes::TYPE_GLRECT));
			mpParentGLRect = static_cast<GLRect*>(mpOwner->getParent()->GetComponent(ComponentTypes::TYPE_GLRECT)); // We will want to mirror color.
			return;
		}

		mpGLRect->setColor(mpParentGLRect->getColor()); // Mirror color.

		TurretCommandEvent* pTurretEvent = static_cast<TurretCommandEvent*>(pEvent);
		this->mAimPoint = pTurretEvent->mAimPoint - glm::vec2(mpParentTransform->getPosition()); // Get aimpoint in local coordinates.

		//std::cout << "aim point is " << mAimPoint.x << ", " << mAimPoint.y << std::endl;

		// Get the aim angle.
		mAimAngle = glm::dot( glm::normalize(mAimPoint), glm::vec2(-1.0f,0.0f ));
		mAimAngle = glm::degrees(mAimAngle);
		//std::cout << "Aim angle is " << mAimAngle << std::endl;
		this->mpTransform->setRotation(mAimAngle);

		
		if (pTurretEvent->mShoot)
			mIsShooting = true;
		else
			mIsShooting = false;
	}
	/*
	else if (pEvent->mType == EventType::TRANSFORM_UPDATED)
	{
		TransformUpdatedEvent* pTransformEvent = static_cast<TransformUpdatedEvent*>(pEvent);
	}
	*/
}


void Turret::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{}

