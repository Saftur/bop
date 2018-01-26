//------------------------------------------------------------------------------
//
// File Name:	GameStatePlatformLevel2.c
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStatePlatformLevel2.h"
#include "Trace.h"
#include "GameObjectManager.h"
#include "BehaviorPlayer.h"
#include "BehaviorCheckpoint.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

AEGfxTexture* GameStatePlatformLevel2::texturePlayer;
AEGfxVertexList* GameStatePlatformLevel2::meshQuad;
GameObject* GameStatePlatformLevel2::gameObjectPlayer;
SpriteSource* GameStatePlatformLevel2::spriteSourcePlayer;

AEGfxTexture* GameStatePlatformLevel2::textureCheckpoint;
AEGfxVertexList* GameStatePlatformLevel2::meshQuad2;
GameObject* GameStatePlatformLevel2::gameObjectCheckpoint;
SpriteSource* GameStatePlatformLevel2::spriteSourceCheckpoint;

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void GameStatePlatformLevel2::CreateGoal(Vector2D loc, int targetLevel)
{
	GameObject* gameObjectGoal = new GameObject("Goal");

	Transform* transform = new Transform(loc.X(), loc.Y());

	Sprite* sprite2 = new Sprite("Goal Sprite");
	sprite2->SetMesh(meshGoal);
	sprite2->SetSpriteSource(spriteSourceGoal);

	Animation* animation2 = new Animation(sprite2);
	animation2->Play(4, 0.25f, true);

	Physics* physics = new Physics();

	gameObjectGoal->SetAnimation(animation2);
	gameObjectGoal->SetPhysics(physics);
	gameObjectGoal->SetSprite(sprite2);
	gameObjectGoal->SetTransform(transform);

	Collider* col = new Collider(gameObjectGoal);
	gameObjectGoal->SetCollider(col);

	Behavior* behavior = (Behavior)new BehaviorGoal(gameObjectGoal, targetLevel);
	gameObjectGoal->SetBehavior(behavior);

	GameObjectManager::GetInstance().Add(gameObjectGoal);
}

void GameStatePlatformLevel2::CreateCheckpoint(Vector2D loc)
{
	GameObject* gameObjectCheckpoint = new GameObject("Checkpoint");

	Transform* transform = new Transform(loc.X(), loc.Y());
	transform->SetScale(Vector2D(100, 100));

	Sprite* sprite2 = new Sprite("Checkpoint Sprite");
	sprite2->SetMesh(meshQuad);
	sprite2->SetSpriteSource(spriteSourceCheckpoint);

	Animation* animation2 = new Animation(sprite2);
	animation2->Play(8, 0.25f, true);

	Behavior* bCh = (Behavior)new BehaviorCheckpoint(gameObjectCheckpoint);

	Physics* physics = new Physics();

	gameObjectCheckpoint->SetAnimation(animation2);
	gameObjectCheckpoint->SetPhysics(physics);
	gameObjectCheckpoint->SetSprite(sprite2);
	gameObjectCheckpoint->SetTransform(transform);
	gameObjectCheckpoint->SetBehavior(bCh);

	Collider col = new Collider(gameObjectCheckpoint);
	gameObjectCheckpoint->SetCollider(col);

	GameObjectManager::GetInstance().Add(*gameObjectCheckpoint);
}

// Load the resources associated with the PlatformLevel2 game state.
void GameStatePlatformLevel2::Load()
{
	Trace::GetInstance().GetStream() << "PlatformLevel2: Load" << std::endl;
}

// Initialize the memory associated with the PlatformLevel2 game state.
void GameStatePlatformLevel2::Init()
{
	Trace::GetInstance().GetStream() << "PlatformLevel2: Init" << std::endl;
}

// Update the PlatformLevel2 game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStatePlatformLevel2::Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	Trace::GetInstance().GetStream() << "PlatformLevel2: Update" << std::endl;

	GameStateManager::GetInstance().SetNextState(GameStateTable::GsQuit);
}

// Shutdown any memory associated with the PlatformLevel2 game state.
void GameStatePlatformLevel2::Shutdown()
{
	Trace::GetInstance().GetStream() << "PlatformLevel2: Shutdown" << std::endl;
}

// Unload the resources associated with the PlatformLevel2 game state.
void GameStatePlatformLevel2::Unload()
{
	Trace::GetInstance().GetStream() << "PlatformLevel2: Unload" << std::endl;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

