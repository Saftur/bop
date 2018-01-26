//------------------------------------------------------------------------------
//
// File Name:	GameStateStub.h
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
class GameStateLoading
{
public:
	//------------------------------------------------------------------------------
	// Public Consts:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Load the resources associated with the Loading game state.
	static void Load();

	// Initialize the memory associated with the Loading game state.
	static void Init();

	// Update the Loading game state.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	static void Update(float dt);

	// Shutdown any memory associated with the Loading game state.
	static void Shutdown();

	// Unload the resources associated with the Loading game state.
	static void Unload();
};
/*----------------------------------------------------------------------------*/
