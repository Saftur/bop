//------------------------------------------------------------------------------
//
// File Name:	GameStateManager.h
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright � 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "GameStateTable.h"

//------------------------------------------------------------------------------

class GameStateManager
{
public:
	//------------------------------------------------------------------------------
	// Public Function Declarations:
	//------------------------------------------------------------------------------

	// Initialize the game state manager.
	void Init();

	// Update the game state manager.
	void Update(float dt);

	void UpdateAO(float dt);

	// Shutdown the game state manager.
	void Shutdown();

	// Determine if the current game state is being restarted.
	bool IsRestarting() const;

	// Determine if the game is still running.
	bool IsRunning() const;

	// Set the next game state to run.
	void SetNextState(int nextState);

	// Retrieve the instance of the GameStateManager singleton.
	static GameStateManager& GetInstance();

private:
	//------------------------------------------------------------------------------
	// Private Structures:
	//------------------------------------------------------------------------------
	typedef struct
	{
		GameStateTable::GameStates	current;	/* Index of the current game state. */
		GameStateTable::GameStates	previous;	/* Index of the previous game state. */
		GameStateTable::GameStates	next;		/* Index of the next game state. */

	} GameState;

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------
	GameState gameState;

	//------------------------------------------------------------------------------
	// Private Function Declarations:
	//------------------------------------------------------------------------------
	bool StateIsChanging() const;

	// Constructor is private to prevent accidental instantiations.
	GameStateManager();

	// Disable copy constructor and assignment operator
	GameStateManager(const GameStateManager&) = delete;
	GameStateManager& operator=(const GameStateManager&) = delete;
};
//------------------------------------------------------------------------------
