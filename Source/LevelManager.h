#pragma once

#include <map>

#include "stdafx.h"
#include "GameObjectManager.h"
#include "SpriteSource.h"
#include "PlatformManager.h"

enum LM_Mode { IDLE = 0, LOAD = 1, CREATE = 2, FINISHED = 3 };
enum LM_Parent { NONE = 0, GO = 1, TRANS = 2, SPRITE = 3, ANIM = 4, SPRSRC = 5, MESH = 6, PHYS = 7, COLL = 8, BEHAV = 9, PLAT = 10 };

// TODO: Clean up the code in here.

class LevelManager
{
public:
	void Init();
	void Load(const char* levelName);
	void Update(float dt);
	void Shutdown();
	static LevelManager& GetInstance();
	bool IsLoaded();
	void Debug();

	~LevelManager();
private:
	enum LM_Type {
		GameObject, OpBr, ClBr, Mesh, HalfSize, UV, SpriteSource, Size, Texture, Translation, Scale, Rotation, UIElement, Sprite, Alpha, Frame, Animation, FrameDur, IsLooping, Physics,
		Collider, Behavior, Platform, Transform, Jump, Speed, Vector
	};

	LevelManager();

	std::string GetNextWord(std::string& str, bool remove = false);
	int genID();
	int getObjByName(std::string name);

	struct Base
	{
		virtual ~Base() {}
		int ID;
	};

	struct TmpMesh : public Base
	{
		std::string Name;
		Vector2D HalfSize;
		Vector2D UV;
	};

	struct TmpSpriteSrc : public Base
	{
		std::string Name;
		int Rows;
		int Cols;
		std::string Texture;
	};

	struct TmpTransform : public Base
	{
		Vector2D Translation;
		Vector2D Scale;
		float Rotation;
		bool UIElement;
	};

	struct TmpSprite : public Base
	{
		std::string Name;
		float Alpha;
		int Frame;
		TmpMesh* Mesh;
		TmpSpriteSrc* SpriteSource;
	};

	struct TmpAnim : public Base
	{
		float FrameDuration;
		bool IsLooping;
	};

	struct TmpPhys : public Base
	{
	};

	struct TmpColl : public Base
	{
	};

	struct TmpBehavior : public Base
	{
		std::string Type;
	};

	struct TmpGO : public Base
	{
		std::string Name;
		TmpTransform* Transform;
		TmpSprite* Sprite;
		TmpAnim* Animation;
		TmpPhys* Physics;
		TmpColl* Collider;
		TmpBehavior* Behavior;
	};

	struct TmpPlatform : public Base
	{
		TmpTransform* Transform;
		float JumpHeight;
		float Speed;
	};

	static LM_Mode stateCurr;
	static LM_Mode stateNext;

	std::string contents;

	TmpMesh* tmpMesh;
	TmpSpriteSrc* tmpSpriteSrc;
	TmpAnim* tmpAnim;
	TmpBehavior* tmpBehavior;
	TmpColl* tmpColl;
	TmpGO* tmpGO;
	TmpPhys* tmpPhys;
	TmpPlatform* tmpPlatform;
	TmpSprite* tmpSprite;
	TmpTransform* tmpTransform;

	static LM_Parent p1, p2, p3;

	std::map<int, Base*> objs;
	std::map<int, AEGfxTexture*> textures;
	std::map<int, ::SpriteSource*> spriteSources;
	std::map<int, AEGfxVertexList*> verts;
	std::map<std::string, LM_Type> strs;

	static int depth;
	static int id;
};
