#pragma once
#include <map>
#include <sstream>

using namespace std;

enum LM_STATE { IDLE, LOAD, CREATE, DONE };

class LevelManager
{
public:
	void Init();
	void Update(float dt);
	void Shutdown();

	// Get the LevelManager's current state.
	static LM_STATE GetState();
	// Returns the LevelManager's progress as a float ranging from 0-100.
	static float GetProgress();

	// Returns the current LevelManager instance.
	static LevelManager& GetInstance();

	// Loads a level.
	// Params:
	//   name - the name of the level file, excluding file extension.
	//   dir - the directory to search in. Default is root\Levels.
	static void LoadLevel(string name, string dir = "Levels\\");
private:
	// Private constructor to prevent instantiation.
	LevelManager();

	// Enum containing all of the keywords to check for.
	enum LM_KEYWORD { MESH, HALFSIZE, UV, SPRITESOURCE, SIZE, TEXTURE, GAMEOBJECT, TRANSFORM, TRANSLATION, SCALE, ROTATION, UIELEMENT, SPRITE, ALPHA,
					  FRAME, ANIMATION, FRAMEDUR, ISLOOPING, PHYSICS, COLLIDER, BEHAVIOR, OPBR, CLBR };

	// Classes & structs
	struct BaseClass
	{
		string typeStr;
	};

	// Templates
	template<string type, class T1 = nullptr, class T2 = nullptr, class T3 = nullptr, class T4 = nullptr, class T5 = nullptr, class T6 = nullptr, class T7 = nullptr, typename base = BaseClass>
	struct Container : base
	{
		T1 t1;
		T2 t2;
		T3 t3;
		T4 t4;
		T5 t5;
		T6 t6;
		T7 t7;

		typeStr = type;
	};

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
	BaseClass* getParentByID(string typeStr);

	// Data
	string contents;
	map<string, LM_KEYWORD> keywords;
	map<int, BaseClass*> containers;
	map<string, int> names;

	static int id;
	static LM_STATE stateCurr, stateNext;
	static unsigned int objCount, objsLoaded;
	static std::string words;

	// Typedefs
	typedef Container<"Mesh", string, Vector2D, Vector2D> MeshContainer;
	typedef Container<"SpriteSource", string, Vector2D, string> SpriteSourceContainer;
	typedef Container<"Transform", Vector2D, Vector2D, float, bool> TransformContainer;
	typedef Container<"Sprite", string, float, int, MeshContainer*, SpriteSourceContainer*> SpriteContainer;
	typedef Container<"Animation", float, bool> AnimationContainer;
	typedef Container<"Physics"> PhysicsContainer;
	typedef Container<"Collider"> ColliderContainer;
	typedef Container<"Behavior", string> BehaviorContainer;
	typedef Container<"GameObject", string, TransformContainer*, SpriteContainer*, AnimationContainer*, PhysicsContainer*, ColliderContainer*, BehaviorContainer*> GameObjectContainer;
};

