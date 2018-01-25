//------------------------------------------------------------------------------
//
// File Name:	Engine.c
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Engine.h"
#include "GameStateManager.h"
#include "System.h"
#include "Trace.h"
#include "GameObjectManager.h"
#include "Random.h"
#include "SoundManager.h"
#include "IOController.h"
#include "LevelManager.h"

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the game engine.
void Engine::Init()
{
	//--------------------------------------------------------------------------
	// NOTE: Certain modules need to be initialized first.
	//--------------------------------------------------------------------------

	// Initialize the Tracing/Logging module.
	Trace::GetInstance().Init();

	Trace::GetInstance().GetStream() << "Engine: Init" << std::endl;

	// Initialize the System (Windows, Event Handlers).
	System::GetInstance().Init();

	//--------------------------------------------------------------------------
	// NOTE: Other modules can be initialized later and in any order.
	//--------------------------------------------------------------------------

	// Initialize the game state manager.
	GameStateManager::GetInstance().Init();

	GameObjectManager::GetInstance().Init();

	LevelManager::GetInstance().Init();

	SoundManager::GetInstance().Init();

	RandomInit();
}

// Update the game engine.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void Engine::Update(float dt)
{
	Trace::GetInstance().GetStream() << "Engine: Update" << std::endl;

	// Update the System (Windows, Event Handlers).
	System::GetInstance().Update(dt);

	// Update the game state manager.
	GameStateManager::GetInstance().Update(dt);

	// Update the game object manager.
	GameObjectManager::GetInstance().Update(dt);
	GameStateManager::GetInstance().UpdateAO(dt);
	GameObjectManager::GetInstance().CheckCollisions();
	GameObjectManager::GetInstance().Draw();

	// Update the sound manager.
	SoundManager::GetInstance().Update(dt);

	LevelManager::GetInstance().Update(dt);

	// Complete the draw process for the current game loop.
	System::GetInstance().Draw();
}

// Shutdown the game engine.
void Engine::Shutdown()
{
	Trace::GetInstance().GetStream() << "Engine: Shutdown" << std::endl;

	//--------------------------------------------------------------------------
	// NOTE: Some modules can be shutdown in any order.
	//--------------------------------------------------------------------------

	// Shutdown the game state manager.
	GameStateManager::GetInstance().Shutdown();
	GameObjectManager::GetInstance().Shutdown();
	SoundManager::GetInstance().Shutdown();

	//--------------------------------------------------------------------------
	// NOTE: Certain modules need to be shutdown last and in reverse order.
	//--------------------------------------------------------------------------

	// Shutdown the System (Windows, Event Handlers).
	System::GetInstance().Shutdown();

	// Shutdown the Tracing/Logging module.
	Trace::GetInstance().Shutdown();
}

// Retrieve the instance of the Engine singleton
Engine& Engine::GetInstance()
{
	static Engine instance;
	return instance;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Constructor is private to prevent accidental instantiation
Engine::Engine()
{
}
