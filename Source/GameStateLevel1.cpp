//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel1.c
// Author(s):	Mark Culp
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "GameStateLevel1.h"
#include "GameObject.h"
#include "Trace.h"
#include "Mesh.h"
#include "Transform.h"
#include "SpriteSource.h"
#include "BehaviorCheckpoint.h"
#include "Collider.h"
#include "GameObjectManager.h"
#include "PlatformManager.h"
#include "BehaviorPlayer.h"
#include "BehaviorGoal.h"
#include "LevelManager.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
const float GameStateLevel1::monkeyWalkSpeed = 500.0f;
const float GameStateLevel1::monkeyJumpSpeed = 1000.0f;
const float GameStateLevel1::groundHeight = 150.0f;
const Vector2D GameStateLevel1::gravityNormal(0.0f, -1500.0f);
const Vector2D GameStateLevel1::gravityNone(0.0f, 0.0f);
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
AEGfxTexture* GameStateLevel1::textureMonkey;
AEGfxVertexList* GameStateLevel1::meshQuad;
GameObject* GameStateLevel1::gameObjectMonkey;
SpriteSource* GameStateLevel1::spriteSourceMonkey;

AEGfxTexture* GameStateLevel1::textureGoal;
AEGfxVertexList* GameStateLevel1::meshGoal;
SpriteSource* GameStateLevel1::spriteSourceGoal;
GameObject* GameStateLevel1::gameObjectGoal;

AEGfxTexture* GameStateLevel1::textureCheckpoint;
AEGfxVertexList* GameStateLevel1::meshCheckpoint;
GameObject* GameStateLevel1::gameObjectCheckpoint;
SpriteSource* GameStateLevel1::spriteSourceCheckpoint;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateLevel1::Load()
{
	Trace::GetInstance().GetStream() << "Stub: Load" << std::endl;

	meshQuad = MeshCreateQuad(0.5f, 0.5f, 1.0f / 3, 1.0f / 3, "Mesh3x3");
	textureMonkey = AEGfxTextureLoad("Assets\\MonkeyWalk.png");
	spriteSourceMonkey = new SpriteSource(3, 3, textureMonkey);

	meshCheckpoint = MeshCreateQuad(0.5f, 0.5f, 1.0f / 3, 1.0f / 3, "Mesh3x3");
	textureCheckpoint = AEGfxTextureLoad("Assets\\player.png");
	spriteSourceCheckpoint = new SpriteSource(3, 3, textureCheckpoint);

	meshGoal = MeshCreateQuad(0.5f, 0.5f, 1.0f / 3, 1.0f / 3, "Mesh3x3");
	textureGoal = AEGfxTextureLoad("Assets\\goal.png");
	spriteSourceGoal = new SpriteSource(4, 1, textureGoal);
}

GameObject* GameStateLevel1::CreateMonkey()
{
	gameObjectMonkey = new GameObject("Player");

	Transform* transform = new Transform(0, groundHeight);
	transform->SetScale(Vector2D(100, 100));

	Sprite* sprite2 = new Sprite("Monkey Sprite");
	sprite2->SetMesh(meshQuad);
	sprite2->SetSpriteSource(spriteSourceMonkey);

	Animation* animation2 = new Animation(sprite2);
	animation2->Play(8, 0.25f, true);

	Physics* physics = new Physics();

	gameObjectMonkey->SetAnimation(*animation2);
	gameObjectMonkey->SetPhysics(*physics);
	gameObjectMonkey->SetSprite(*sprite2);
	gameObjectMonkey->SetTransform(*transform);

	Collider* col = new Collider(*gameObjectMonkey);
	gameObjectMonkey->SetCollider(*col);

	Behavior* behavior = (Behavior*)new BehaviorPlayer(*gameObjectMonkey);
	gameObjectMonkey->SetBehavior(*behavior);

	BehaviorCheckpoint::SetPlayer(*gameObjectMonkey);

	return gameObjectMonkey;
}

GameObject* GameStateLevel1::CreateGoal()
{
	gameObjectGoal = new GameObject("Goal");

	Transform* transform = new Transform(0, groundHeight);

	Sprite* sprite2 = new Sprite("Goal Sprite");
	sprite2->SetMesh(meshGoal);
	sprite2->SetSpriteSource(spriteSourceGoal);

	Animation* animation2 = new Animation(sprite2);
	animation2->Play(4, 0.25f, true);

	Physics* physics = new Physics();

	gameObjectGoal->SetAnimation(*animation2);
	gameObjectGoal->SetPhysics(*physics);
	gameObjectGoal->SetSprite(*sprite2);
	gameObjectGoal->SetTransform(*transform);

	Collider* col = new Collider(*gameObjectGoal);
	gameObjectGoal->SetCollider(*col);

	Behavior* behavior = (Behavior*)new BehaviorGoal(*gameObjectGoal, GameStateTable::GsLevel2);
	gameObjectGoal->SetBehavior(*behavior);

	return gameObjectGoal;
}

GameObject* GameStateLevel1::CreateCheckpoint()
{
	gameObjectCheckpoint = new GameObject("Checkpoint");

	Transform* transform = new Transform(0, groundHeight);
	transform->SetScale(Vector2D(100, 100));

	Sprite* sprite2 = new Sprite("Checkpoint Sprite");
	sprite2->SetMesh(meshQuad);
	sprite2->SetSpriteSource(spriteSourceGoal);

	Animation* animation2 = new Animation(sprite2);
	animation2->Play(8, 0.25f, true);

	Behavior* bCh = (Behavior*)new BehaviorGoal(*gameObjectCheckpoint, GameStateTable::GsLevel2);

	Physics* physics = new Physics();

	gameObjectCheckpoint->SetAnimation(*animation2);
	gameObjectCheckpoint->SetPhysics(*physics);
	gameObjectCheckpoint->SetSprite(*sprite2);
	gameObjectCheckpoint->SetTransform(*transform);
	gameObjectCheckpoint->SetBehavior(*bCh);

	Collider* col = new Collider(*gameObjectCheckpoint);
	gameObjectCheckpoint->SetCollider(*col);

	return gameObjectCheckpoint;
}

// Initialize the memory associated with the Stub game state.
void GameStateLevel1::Init()
{
	LevelManager::GetInstance().Load("test1");
	Trace::GetInstance().GetStream() << "Stub: Init" << std::endl;

	CreateMonkey();
	//CreateCheckpoint();
	//CreateGoal();

	GameObjectManager::GetInstance().Add(*gameObjectMonkey);
	//GameObjectManager::GetInstance().Add(*gameObjectCheckpoint);
	//GameObjectManager::GetInstance().Add(*gameObjectGoal);

	AEGfxSetBackgroundColor(1, 1, 1);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	PlatformManager::Init();

	Transform transform = Transform(0, 0);
	transform.SetScale({ 100, 20 });

	PlatformManager::AddPlatform(transform, 100);

	transform.SetTranslation({ 100, 0 });
	vector<Vector2D> path;
	path.push_back({ 100, 50 });
	//path.push_back({ -100, -40 });
	//path.push_back({ 0, -80 });
	PlatformManager::AddPlatform(transform, 0, 50, path);

	/*transform.SetTranslation({ 0, 500 });
	path.clear();
	for (int i = 0; i < 360; i++) {
		path.push_back(Vector2D((float)cos(i) * 500, (float)sin(i)) * 500);
	}
	PlatformManager::AddPlatform(transform, 0, 200, path);*/

	transform.SetTranslation({ 0, -130 });
	transform.SetScale({ 400, 20 });
	PlatformManager::AddPlatform(transform);
}

void GameStateLevel1::MoveMonkey()
{
	Physics* objPhysics = gameObjectMonkey->GetPhysics();
	Transform* objTransform = gameObjectMonkey->GetTransform();

	if (objPhysics == NULL || objTransform == NULL) return;

	Vector2D objVelocity = objPhysics->GetVelocity();

	if (AEInputCheckCurr(VK_LEFT))
	{
		objVelocity.X(-monkeyWalkSpeed);
	}
	else if (AEInputCheckCurr(VK_RIGHT))
	{
		objVelocity.X(monkeyWalkSpeed);
	}
	else
	{
		objVelocity.X(0);
	}

	if (AEInputCheckCurr(VK_UP))
	{
		objVelocity.Y(monkeyJumpSpeed);
		objPhysics->SetAcceleration(GameStateLevel1::gravityNormal);
	}

	if (objTransform->GetTranslation().Y() < groundHeight)
	{
		objTransform->SetTranslation(Vector2D(objTransform->GetTranslation().X(), groundHeight));
		objVelocity.Y(0);
		objPhysics->SetAcceleration(GameStateLevel1::gravityNone);
	}

	objPhysics->SetVelocity(objVelocity);
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel1::Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	Trace::GetInstance().GetStream() << "Stub: Update" << std::endl;

	//MoveMonkey();
	PlatformManager::Update(dt);

	BehaviorCheckpoint::CheckPos();

	if (AEInputCheckCurr('1'))
	{
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsRestart);
	}
	else if (AEInputCheckCurr('2'))
	{
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsLevel2);
	}
	else if (AEInputCheckCurr('3'))
	{
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsAsteroids);
	}
	else if (AEInputCheckCurr('R'))
	{
		BehaviorCheckpoint::ResetUnconditional();
	}
}

void GameStateLevel1::UpdateAO(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	Transform::SetCamTranslation(-gameObjectMonkey->GetTransform()->GetTranslation());
}

// Shutdown any memory associated with the Stub game state.
void GameStateLevel1::Shutdown()
{
	Trace::GetInstance().GetStream() << "Stub: Shutdown" << std::endl;

	GameObjectManager::GetInstance().Shutdown();

	PlatformManager::Shutdown();
}

// Unload the resources associated with the Stub game state.
void GameStateLevel1::Unload()
{
	Trace::GetInstance().GetStream() << "Stub: Unload" << std::endl;

	AEGfxMeshFree(meshQuad);
	AEGfxMeshFree(meshCheckpoint);
	AEGfxMeshFree(meshGoal);
	AEGfxTextureUnload(textureMonkey);
	AEGfxTextureUnload(textureCheckpoint);
	AEGfxTextureUnload(textureGoal);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

