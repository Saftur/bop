#pragma once

#include <map>

#include "stdafx.h"
#include "GameObjectManager.h"
#include "PlatformManager.h"

enum LM_Mode { IDLE = 0, LOAD = 1, FIND_CHILDREN = 2, CREATE = 3, FINISHED = 4 };
enum LM_Parent { NONE = 0, GO = 1, TRANS = 2, SPRITE = 3, ANIM = 4, SPRSRC = 5, MESH = 6, PHYS = 7, COLL = 8, BEHAV = 9, PLAT = 10 };
enum LM_Type { GameObject, OpBr, ClBr, Mesh, HalfSize, UV, SpriteSource, Size, Texture, Translation, Scale, Rotation, UIElement, Sprite, Alpha, Frame, Animation, FrameDur, IsLooping, Physics,
				Collider, Behavior, Platform, Transform, Jump, Speed, Vector };

class LevelManager
{
public:
	static void Init();
	static void Load(const char* levelName);
	static void Update(float dt);
	static void Unload();
	static LevelManager& GetInstance();
	static bool IsLoaded();

	~LevelManager();
private:
	LevelManager();

	static std::string GetNextWord(std::string& str, bool remove = false);
	static constexpr unsigned int str2int(const char* str, int h = 0);

	struct Base
	{
		virtual ~Base() {}
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
		std::string Name;
	};

	struct TmpColl : public Base
	{
		std::string Name;
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

	struct TmpPlatform
	{
		TmpTransform* Transform;
		float JumpHeight;
		float Speed;
	};

	static LM_Mode stateCurr;
	static LM_Mode stateNext;

	static std::string contents;

	static TmpMesh* tmpMesh;
	static TmpSpriteSrc* tmpSpriteSrc;
	static TmpAnim* tmpAnim;
	static TmpBehavior* tmpBehavior;
	static TmpColl* tmpColl;
	static TmpGO* tmpGO;
	static TmpPhys* tmpPhys;
	static TmpPlatform* tmpPlatform;
	static TmpSprite* tmpSprite;
	static TmpTransform* tmpTransform;

	static LM_Parent p1;
	static LM_Parent p2;
	static LM_Parent p3;

	static std::map<std::string, Base*> objs;
	static std::map<std::string, LM_Type> strs;

	static int depth;
};
