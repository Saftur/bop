//------------------------------------------------------------------------------
//
// File Name:	GameStatePlatformLevel2.h
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
class GameStatePlatformLevel2
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

	// Load the resources associated with the PlatformLevel2 game state.
	static void Load();

	// Initialize the memory associated with the PlatformLevel2 game state.
	static void Init();

	// Update the PlatformLevel2 game state.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	static void Update(float dt);

	// Shutdown any memory associated with the PlatformLevel2 game state.
	static void Shutdown();

	// Unload the resources associated with the PlatformLevel2 game state.
	static void Unload();

private:
	static AEGfxTexture* texturePlayer;
	static AEGfxVertexList* meshQuad;
	static GameObject* gameObjectPlayer;
	static SpriteSource* spriteSourcePlayer;

	static AEGfxTexture* textureCheckpoint;
	static AEGfxVertexList* meshQuad2;
	static GameObject* gameObjectCheckpoint;
	static SpriteSource* spriteSourceCheckpoint;

	GameObject* CreatePlayer();
	GameObject* CreateCheckpoint();
	void CreateGoal(Vector2D loc, int targetLevel);
	void CreateCheckpoint(Vector2D loc);
};
/*----------------------------------------------------------------------------*/
