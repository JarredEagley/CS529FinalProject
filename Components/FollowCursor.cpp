#include "FollowCursor.h"
#include "ComponentTypes.h"
#include "../Managers/GlobalManager.h"

FollowCursor::FollowCursor() : Component(ComponentTypes::TYPE_FOLLOWCURSOR),
mpCameraComponent(nullptr), mpTransform(nullptr)
{
}

FollowCursor::~FollowCursor()
{}


void FollowCursor::Initialize()
{}

void FollowCursor::Update()
{
	EventManager* pEM = GlobalManager::getEventManager();
	InputManager* pIM = GlobalManager::getInputManager();
	GraphicsManager* pGM = GlobalManager::getGraphicsManager();

	// Need the camera for its projection matrix.
	if (mpCameraComponent == nullptr || mpTransform == nullptr || mpCameraTransformComponent == nullptr)
	{
		mpCameraComponent = static_cast<Camera*>(pGM->getCurrentCameraGO()->GetComponent(ComponentTypes::TYPE_CAMERA));
		mpCameraTransformComponent = static_cast<Transform*>(pGM->getCurrentCameraGO()->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		mpTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		return;
	}

	// Get mouse position.
	int mouseX, mouseY;
	pIM->getMousePosition(mouseX, mouseY);

	// Map -1 to 1.
	glm::vec2 mousePos = glm::vec2(
		(((float)mouseX / (float)pGM->mWindowWidth) - 0.5f) * 2.0f,
		( 0.5f - ((float)mouseY / (float)pGM->mWindowHeight)) * 2.0f
	);

	//std::cout << "(" << mousePos.x << ", " << mousePos.y << ")" << std::endl;

	// Project mouse position to world coordinates.
	auto projMtx = mpCameraComponent->getProjMatrix();
	auto transfMtx = mpCameraComponent->getTransMatrix();
	auto invMtx = projMtx * transfMtx;
	invMtx = glm::inverse(invMtx);

	glm::vec4 near = glm::vec4(mousePos.x, mousePos.y, -1.0f, 1.0f);
	glm::vec4 far = glm::vec4(mousePos.x, mousePos.y, 1.0f, 1.0f);

	glm::vec4 nearR = invMtx * near;
	nearR /= nearR.w;
	glm::vec4 farR = invMtx * far;
	farR /= farR.w;

	//float depth = mpCameraComponent;

	//std::cout << "Depth = " << depth << std::endl;

	//glm::vec4 pos = glm::vec4(mousePos.x, mousePos.y, depth, 1.0f ) * invMtx;

	//mpTransform->setPosition(dir);

}

void FollowCursor::handleEvent(Event* pEvent)
{}
