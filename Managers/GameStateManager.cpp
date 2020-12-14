/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GameStateManager.cpp
	Purpose:			Implementation of the game state manager. Check
						header file for details.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone1
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/27/2020

- End Header --------------------------------------------------------*/

#include "GameStateManager.h"
#include "GlobalManager.h"

GameStateManager* GameStateManager::instance = nullptr;
std::vector<std::string> GameStateManager::mMenuItemNames;
std::list<std::string> GameStateManager::mLivingEnemies;

void GameStateManager::destroySingleton()
{
	mMenuItemNames.clear();
	mLivingEnemies.clear();

	delete instance;
}

// End of singleton stuff. //

GameStateManager::GameStateManager() : 
	DEBUG_VerboseComponents(false), DEBUG_VerboseGameObjects(false), DEBUG_VerboseGOF(false)
{
	srand(time(NULL));
}

void GameStateManager::Update()
{
	InputManager* pIM = GlobalManager::getInputManager();

	if (currentSceneType == SceneType::SCENE_LEVEL && mIsLevelLive)
	{
		// --- Level --- //

		// Handle victory.
		bool victory = testForVictory();
		if (victory && !mIsGamePaused)
		{
			destroyMenus();
			displayVictoryMenu();
		}
		else
		{
			// Handle defeat
			if (isPlayerKilled && !mIsGamePaused)
			{
				destroyMenus();
				displayDefeatMenu();
			}
			else
			{
				// Handle pausing
				if (pIM->IsKeyTriggered(SDL_SCANCODE_ESCAPE))
				{
					// Game is paused, and is being unpaused.
					if (mIsGamePaused)
					{
						destroyMenus();
						mIsGamePaused = false;
					}
					// Game is unpaused, and is being paused.
					else
					{
						displayPauseMenu();
						mIsGamePaused = true;
					}
				}
			}
		}

		// Defeat will be tested for when player game object is destroyed. 

	}
	else
	{
		// --- Menu --- //
	}

}


void GameStateManager::handleMenuItemCommand(std::string command, std::string target = "")
{
	if (command == "RESTART_LEVEL")
	{
		GlobalManager::getResourceManager()->loadLevel(this->currentLevelName);
	}

	if (command == "QUIT_TO_DESKTOP")
	{
		GlobalManager::getGameObjectManager()->deleteAllGameObjects();
		SDL_Event* pQuitEvent = new SDL_Event;
		pQuitEvent->type = SDL_QUIT;
		SDL_PushEvent(pQuitEvent);
	}

	if (command == "BEGIN_LEVEL")
	{
		mIsGamePaused = false;
		auto pGO = GlobalManager::getGameObjectManager()->getGameObject("ClickToBegin");
		// Would be better to mark it as a menu item and call destroy pause menu, but this is okay for now.
		if (pGO != nullptr)
			pGO->mIsMarkedForDelete = true;
	}

	if (command == "GO_TO")
	{
		GlobalManager::getResourceManager()->loadLevel(target);
	}


	if (command == "MENU_DEBUG")
	{
		destroyMenus();
		displayDebugMenu();
	}

	if (command == "MENU_DEBUG_BACK")
	{
		destroyMenus();
		displayPauseMenu();
	}

	if (command == "TOGGLE_COLLISION_DRAW")
	{
		this->DEBUG_DrawCollision = !this->DEBUG_DrawCollision;
	}

	if (command == "DESTEROY_ALL_PROJECTILES")
	{
		DestroyAllProjectilesEvent* pDestroyEvent = new DestroyAllProjectilesEvent;
		GlobalManager::getEventManager()->broadcastEventToSubscribers(pDestroyEvent);
	}

	if (command == "MENU_CONTROLS")
	{
		destroyMenus();
		displayControlsMenu();
	}
}

void GameStateManager::displayPauseMenu()
{
	GameObjectFactory* pGOF = GlobalManager::getGameObjectFactory();
	ResourceManager* pRM = GlobalManager::getResourceManager();

	GlobalManager::getResourceManager()->loadGameObjectArray("Resources\\Menus\\MENU_Pause.json", true);
}

void GameStateManager::destroyMenus()
{
	for (auto name : mMenuItemNames)
	{
		// Storing a vector of pointers would be faster, but more dangerous.
		GameObject* pGO = GlobalManager::getGameObjectManager()->getGameObject(name);
		if (pGO != nullptr)
			pGO->mIsMarkedForDelete = true;
	}
}

void GameStateManager::displayDebugMenu()
{
	GameObjectFactory* pGOF = GlobalManager::getGameObjectFactory();
	ResourceManager* pRM = GlobalManager::getResourceManager();

	GlobalManager::getResourceManager()->loadGameObjectArray("Resources\\Menus\\MENU_Debug_Cheats.json", true);
}

void GameStateManager::displayControlsMenu()
{
	GameObjectFactory* pGOF = GlobalManager::getGameObjectFactory();
	ResourceManager* pRM = GlobalManager::getResourceManager();

	GlobalManager::getResourceManager()->loadGameObjectArray("Resources\\Menus\\MENU_Controls.json", true);
}

bool GameStateManager::testForVictory()
{
	if (mLivingEnemies.size() > 0)
		return false;
	return true;
}

void GameStateManager::displayVictoryMenu()
{
	mIsLevelLive = false;
	mIsGamePaused = true;
	GlobalManager::getResourceManager()->loadGameObjectArray("Resources\\Menus\\MENU_Victory.json", true);
}

void GameStateManager::displayDefeatMenu()
{
	mIsLevelLive = false;
	mIsGamePaused = true;
	GlobalManager::getResourceManager()->loadGameObjectArray("Resources\\Menus\\MENU_Defeat.json",true);
}

float GameStateManager::getRandomFloat()
{
	return (rand());
}


void GameStateManager::readGameConfig()
{
	// Use serializer to read the json in.
	Serializer* pSer = GlobalManager::getSerializer();
	rapidjson::Document doc = pSer->loadJson(GlobalManager::getResourceManager()->pathConfig.c_str());
	if (doc.HasMember("Gamestate Manager") && doc["Gamestate Manager"].IsObject())
	{
		auto currentObj = doc["Gamestate Manager"].GetObject();

		// Debug flags
		if (currentObj.HasMember("DEBUG Verbose GameObjects") && currentObj["DEBUG Verbose GameObjects"].IsBool())
			DEBUG_VerboseGameObjects = currentObj["DEBUG Verbose GameObjects"].GetBool();

		if (currentObj.HasMember("DEBUG Verbose Components") && currentObj["DEBUG Verbose Components"].IsBool())
			DEBUG_VerboseComponents = currentObj["DEBUG Verbose Components"].GetBool();

		if (currentObj.HasMember("DEBUG Verbose GameObject Factory") && currentObj["DEBUG Verbose GameObject Factory"].IsBool())
			DEBUG_VerboseGOF = currentObj["DEBUG Verbose GameObject Factory"].GetBool();

		if (currentObj.HasMember("Initial Scene") && currentObj["Initial Scene"].IsString())
			initialScene = currentObj["Initial Scene"].GetString();
	}

	if (doc.HasMember("Framerate Controller") && doc["Framerate Controller"].IsObject())
	{
		auto currentObj = doc["Framerate Controller"].GetObject();
		if (currentObj.HasMember("Max Framerate") && currentObj["Max Framerate"].IsNumber())
		{
			GlobalManager::getFrameRateController()->setMaxFramerate(
				currentObj["Max Framerate"].GetInt()
			);
		}
	}

	if (doc.HasMember("GameObject Manager") && doc["GameObject Manager"].IsObject())
	{
		auto currentObj = doc["GameObject Manager"].GetObject();
		GameObjectManager* pGOM = GlobalManager::getGameObjectManager();

		if (currentObj.HasMember("Max Particles") && currentObj["Max Particles"].IsNumber())
			pGOM->mMaxParticles = currentObj["Max Particles"].GetInt();

		if (currentObj.HasMember("Max Projectiles") && currentObj["Max Projectiles"].IsNumber())
			pGOM->mMaxProjectiles = currentObj["Max Projectiles"].GetInt();

		if (currentObj.HasMember("Max Debris") && currentObj["Max Debris"].IsNumber())
			pGOM->mMaxDebris = currentObj["Max Debris"].GetInt();
	}

	//if (doc.HasMember("Input Manager") && doc["Input Manager"].IsObject())
	{
	}

	//if (doc.HasMember("Resource Manager") && doc["Resource Manager"].IsObject())
	{
	}

	//if (doc.HasMember("Serializer") && doc["Serializer"].IsObject())
	{
	}

	//if (doc.HasMember("GameObject Factory") && doc["GameObject Factory"].IsObject())
	{
	}

	if (doc.HasMember("Graphics Manager") && doc["Graphics Manager"].IsObject())
	{
		auto currentObj = doc["Graphics Manager"].GetObject();

		// Width and height.
		std::string currentattr = "Window Height";
		if (currentObj.HasMember(currentattr.c_str()) && currentObj[currentattr.c_str()].IsNumber())
		{
			GlobalManager::getGraphicsManager()->mWindowHeight = currentObj[currentattr.c_str()].GetInt();
		}

		currentattr = "Window Width";
		if (currentObj.HasMember(currentattr.c_str()) && currentObj[currentattr.c_str()].IsNumber())
		{
			GlobalManager::getGraphicsManager()->mWindowWidth = currentObj[currentattr.c_str()].GetInt();
		}

		// Zoom stuff
		currentattr = "Zoom Level";
		if (currentObj.HasMember(currentattr.c_str()) && currentObj[currentattr.c_str()].IsNumber())
		{
			GlobalManager::getGraphicsManager()->setZoomLevel( currentObj[currentattr.c_str()].GetFloat() );
		}

		currentattr = "Max Zoom Level";
		if (currentObj.HasMember(currentattr.c_str()) && currentObj[currentattr.c_str()].IsNumber())
		{
			GlobalManager::getGraphicsManager()->setMaxZoomLevel( currentObj[currentattr.c_str()].GetFloat() );
		}

		currentattr = "Min Zoom Level";
		if (currentObj.HasMember(currentattr.c_str()) && currentObj[currentattr.c_str()].IsNumber())
		{
			GlobalManager::getGraphicsManager()->setMinZoomLevel( currentObj[currentattr.c_str()].GetFloat() );
		}
	}

	//if (doc.HasMember("Collision Manager") && doc["Collision Manager"].IsObject())
	{
	}
	
	if (doc.HasMember("Physics Manager") && doc["Physics Manager"].IsObject())
	{
		auto currentObj = doc["Physics Manager"].GetObject();
		PhysicsManager* pPhyM = GlobalManager::getPhysicsManager();

		std::string currentattr = "GameTime Multiplier";
		if (currentObj.HasMember(currentattr.c_str()) && currentObj[currentattr.c_str()].IsNumber())
		{
			pPhyM->gameTimeMultiplier = currentObj[currentattr.c_str()].GetFloat();
		}

		currentattr = "Piercing Threshold";
		if (currentObj.HasMember(currentattr.c_str()) && currentObj[currentattr.c_str()].IsNumber())
		{
			pPhyM->piercingThreshold = currentObj[currentattr.c_str()].GetFloat();
		}

		currentattr = "Deflect Damage Multiplier";
		if (currentObj.HasMember(currentattr.c_str()) && currentObj[currentattr.c_str()].IsNumber())
		{
			pPhyM->deflectDamageMultiplier = currentObj[currentattr.c_str()].GetFloat();
		}

		currentattr = "Pierce Damage Multiplier";
		if (currentObj.HasMember(currentattr.c_str()) && currentObj[currentattr.c_str()].IsNumber())
		{
			pPhyM->piercingDamageMultiplier = currentObj[currentattr.c_str()].GetFloat();
		}

		currentattr = "Explosion Damage Multiplier";
		if (currentObj.HasMember(currentattr.c_str()) && currentObj[currentattr.c_str()].IsNumber())
		{
			pPhyM->explosionDamageMultiplier = currentObj[currentattr.c_str()].GetFloat();
		}

		currentattr = "Explosion Expansion Rate";
		if (currentObj.HasMember(currentattr.c_str()) && currentObj[currentattr.c_str()].IsNumber())
		{
			pPhyM->explosionExpansionRate = currentObj[currentattr.c_str()].GetFloat();
		}

		currentattr = "Explosion Decay Rate";
		if (currentObj.HasMember(currentattr.c_str()) && currentObj[currentattr.c_str()].IsNumber())
		{
			pPhyM->explosionDecayRate = currentObj[currentattr.c_str()].GetFloat();
		}



		currentattr = "Gravitational Constant";
		if (currentObj.HasMember(currentattr.c_str()) && currentObj[currentattr.c_str()].IsNumber())
		{
			pPhyM->universalGravitationalConstant = currentObj[currentattr.c_str()].GetFloat();
		}


	}


	//if (doc.HasMember("Event Manager") && doc["Event Manager"].IsObject())
	{

	}
}


