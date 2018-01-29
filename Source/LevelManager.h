#pragma once
#include <map>
#include <sstream>
#include "Vector2D.h"

using namespace std;

enum LM_STATE { IDLE, LOAD, CREATE, DONE };

class LevelManager
{
public:
	void Init();
	void Update(float dt);
	void Shutdown();

	// Get the LevelManager's current state.
	LM_STATE GetState();
	// Returns the LevelManager's progress as a float ranging from 0-100.
	float GetProgress();

	// Returns the current LevelManager instance.
	static LevelManager& GetInstance();

	// Loads a level.
	// Params:
	//   name - the name of the level file, excluding file extension.
	//   dir - the directory to search in. Default is root\Levels.
	void LoadLevel(string name, string dir = "Levels\\");

	~LevelManager();
private:
	// Private constructor to prevent instantiation.
	LevelManager();

	// Enum containing all of the keywords to check for.
	enum LM_KEYWORD { MESH = 0, HALFSIZE = 1, UV = 2, SPRITESOURCE = 3, SIZE = 4, TEXTURE = 5, GAMEOBJECT = 6, TRANSFORM = 7, TRANSLATION = 8, SCALE = 9, ROTATION = 10, UIELEMENT = 11, SPRITE = 12, ALPHA = 13,
					  FRAME = 14, ANIMATION = 15, FRAMEDUR = 16, ISLOOPING = 17, PHYSICS = 18, COLLIDER = 19, BEHAVIOR = 20, OPBR = 21, CLBR = 22 };

	// Classes & structs
	struct BaseClass
	{
	};

	// Templates
	template<LM_KEYWORD type, typename T1 = nullptr_t, typename T2 = nullptr_t, typename T3 = nullptr_t, typename T4 = nullptr_t, typename T5 = nullptr_t, typename T6 = nullptr_t, typename T7 = nullptr_t, typename base = BaseClass>
	struct Container : base
	{
		T1 t1;
		T2 t2;
		T3 t3;
		T4 t4;
		T5 t5;
		T6 t6;
		T7 t7;

		LM_KEYWORD typeStr;
	};

	template<LM_KEYWORD type, typename T1 = nullptr_t, typename T2 = nullptr_t, typename T3 = nullptr_t, typename T4 = nullptr_t, typename T5 = nullptr_t, typename T6 = nullptr_t, typename T7 = nullptr_t, typename base = BaseClass>
	LM_KEYWORD Container<type, T1, T2, T3, T4, T5, T6, T7>::typeStr = MESH;

	// Gets the next word from the given string. If the word is empty (ie, a space),
	// removes and returns the next word instead.
	// Params:
	//   remove - should we remove the word from the string? Default is true.
	// Returns:
	//   The found word.
	string getNextWord(bool remove = true);

	// Loads the next object into memory.
	void loadObject();

	// Finds the entity most probable to be the parent.
	// Params:
	//   typeStr - what type of container to look for.
	// Returns:
	//   A pointer to the found object, or nullptr if none was found.
	BaseClass* getParentByID(LM_KEYWORD type);

	string RemoveChar(string& str, const char toFind, bool doSpaces = true);

	// Data
	map<string, LM_KEYWORD> keywords;
	map<int, BaseClass*> containers;
	map<string, int> names;

	static int id;
	static LM_STATE stateCurr, stateNext;
	static unsigned int objCount, objsLoaded;
	std::string words;

	// Typedefs
	typedef Container<MESH, string, Vector2D, Vector2D> MeshContainer;
	typedef Container<SPRITESOURCE, string, Vector2D, string> SpriteSourceContainer;
	typedef Container<TRANSFORM, Vector2D, Vector2D, float, bool> TransformContainer;
	typedef Container<SPRITE, string, float, int, MeshContainer*, SpriteSourceContainer*> SpriteContainer;
	typedef Container<ANIMATION, float, bool> AnimationContainer;
	typedef Container<PHYSICS> PhysicsContainer;
	typedef Container<COLLIDER> ColliderContainer;
	typedef Container<BEHAVIOR, string> BehaviorContainer;
	typedef Container<GAMEOBJECT, string, TransformContainer*, SpriteContainer*, AnimationContainer*, PhysicsContainer*, ColliderContainer*, BehaviorContainer*> GameObjectContainer;
};

