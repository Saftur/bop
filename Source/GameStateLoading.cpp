//------------------------------------------------------------------------------
//
// File Name:	GameStateLoading.c
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateLoading.h"
#include "Trace.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Loading game state.
void GameStateLoading::Load()
{
	Trace::GetInstance().GetStream() << "Loading: Load" << std::endl;
}

// Initialize the memory associated with the Loading game state.
void GameStateLoading::Init()
{
	Trace::GetInstance().GetStream() << "Loading: Init" << std::endl;
}

// Update the Loading game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLoading::Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	Trace::GetInstance().GetStream() << "Loading: Update" << std::endl;

	GameStateManager::GetInstance().SetNextState(GameStateTable::GsQuit);
}

// Shutdown any memory associated with the Loading game state.
void GameStateLoading::Shutdown()
{
	Trace::GetInstance().GetStream() << "Loading: Shutdown" << std::endl;
}

// Unload the resources associated with the Loading game state.
void GameStateLoading::Unload()
{
	Trace::GetInstance().GetStream() << "Loading: Unload" << std::endl;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

