//------------------------------------------------------------------------------
//
// File Name:	GameStateManager.c
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateTable.h"
#include "Trace.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the game state manager.
void GameStateManager::Init()
{
	Trace::GetInstance().GetStream() << "GSM: Init" << std::endl;

	// Set the previous and current game states to invalid.
	gameState.previous = (GameStateTable::GameStates) GameStateTable::GsInvalid;
	gameState.current = (GameStateTable::GameStates) GameStateTable::GsInvalid;

	// Set the initial game state to launch the game.
	gameState.next = GameStateTable::GetInstance().GsInitial;
}

// Update the game state manager.
void GameStateManager::Update(float dt)
{
	Trace::GetInstance().GetStream() << "GSM: Update" << std::endl;

	// Update the current game state.
	GameStateTable::GetInstance().ExecuteUpdate(gameState.current, dt);

	// Check for a game state change.
	if (StateIsChanging())
	{
		// Shutdown the current game state.
		GameStateTable::GetInstance().ExecuteShutdown(gameState.current);

		// TODO: Implement code to properly handle unloading the current game state.
		if (gameState.next != GameStateTable::GetInstance().GsRestart)
			GameStateTable::GetInstance().ExecuteUnload(gameState.current);

		// Update the recorded states.
		gameState.previous = gameState.current;
		gameState.current = gameState.next;

		// TODO: Implement code to properly handle loading the new game state.
		if (gameState.current == GameStateTable::GetInstance().GsRestart)
		{
			// If we are restarting, current and next states should be the same as the previous
			gameState.current = gameState.previous;
			gameState.next = gameState.previous;
		}
		else
		{
			// Otherwise, load the next state
			GameStateTable::GetInstance().ExecuteLoad(gameState.current);
		}

		// Initialize the new game state.
		GameStateTable::GetInstance().ExecuteInit(gameState.current);
	}
}

void GameStateManager::UpdateAO(float dt)
{
	GameStateTable::GetInstance().ExecuteUpdateAO(gameState.current, dt);
}

// Shutdown the game state manager.
void GameStateManager::Shutdown()
{
	Trace::GetInstance().GetStream() << "GSM: Shutdown" << std::endl;

	// Nothing needs to be done here.
}

// Determine if the current game state is being restarted.
bool GameStateManager::IsRestarting() const
{
	return gameState.next == GameStateTable::GetInstance().GsRestart;
}

// Determine if the game is still running.
bool GameStateManager::IsRunning() const
{
	return gameState.current != GameStateTable::GetInstance().GsQuit;
}

// Set the next game state to run.
void GameStateManager::SetNextState(int nextState)
{
	if (GameStateTable::GetInstance().StateIsValid((GameStateTable::GameStates)nextState) || GameStateTable::GetInstance().StateIsSpecial((GameStateTable::GameStates)nextState))
	{
		gameState.next = (GameStateTable::GameStates)nextState;
	}
	else
	{
#ifdef _DEBUG
		/* In debug mode, print a message that an error has occurred. */
		Trace::GetInstance().GetStream() << "ERROR: Invalid game state: " << nextState << std::endl;
#endif
	}
}

// Retrieve the instance of the GameStateManager singleton.
GameStateManager& GameStateManager::GetInstance()
{
	static GameStateManager instance;
	return instance;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

// Determine if a game state change has been requested.
bool GameStateManager::StateIsChanging() const
{
	return (gameState.current != gameState.next);
}

// Constructor is private to prevent accidental instantiations.
GameStateManager::GameStateManager()
{
}
