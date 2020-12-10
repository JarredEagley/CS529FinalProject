/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Generators.h
	Purpose:			Functions for generating specific types of game
						objects-- ie explosions, reside in here to
						avoid cicular dependancies within the game object
						factory.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/9/2020

- End Header --------------------------------------------------------*/

#include "Managers/GlobalManager.h"

#include "Components/PhysicsBody.h"
#include "Components/Transform.h"
#include "Components/Explosion.h"

/// <summary>
/// Generates an explosion for a physics body using the given input parameters.
/// Note: Does not mark physics body's game object for deletion. That is the job of the caller.
/// </summary>
/// <param name="pExplodingBody">The physics body being told to explode</param>
/// <param name="fileName">name of the explosion image to use in the explosions directory</param>
/// <returns>the explosion's game object.</returns>
GameObject* generateExplosion(PhysicsBody* pExplodingBody, float intensity, std::string fileName)
{
	if (pExplodingBody == nullptr)
		return nullptr;

	// Create the explosion GameObject.
	std::string explosionName = pExplodingBody->mpOwner->mName + "_Explosion";
	std::string explosionPath = GlobalManager::getResourceManager()->pathExplosions + fileName;
	GameObject* explosionGO = GlobalManager::getGameObjectFactory()->createDynamicGameObject(explosionPath, explosionName);

	if (explosionGO == nullptr)
		return nullptr;

	// Send an event to update the trasnform position.
	auto pExplosionTransform = static_cast<Transform*>(explosionGO->GetComponent(ComponentTypes::TYPE_TRANSFORM));
	auto pExplosionPhysics = static_cast<PhysicsBody*>(explosionGO->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
	auto pExplosionComp = static_cast<Explosion*>(explosionGO->GetComponent(ComponentTypes::TYPE_EXPLOSION));
	if (pExplosionPhysics != nullptr && pExplosionTransform != nullptr && pExplosionComp != nullptr)
	{
		pExplosionTransform->setPosition(pExplodingBody->mPosition);

		pExplosionPhysics->mTotalForce = glm::vec2(0.0f);
		pExplosionPhysics->mVelocity = pExplodingBody->mVelocity;
		pExplosionPhysics->mCollisionType = collisionType::NOCLIP;

		pExplosionComp->mIntensity = intensity;
	}

	return explosionGO;
}