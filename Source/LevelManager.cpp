#include "stdafx.h"

#include <fstream>
#include <algorithm>

#include "LevelManager.h"
#include "GameObjectManager.h"
#include "Vector2D.h"

using namespace std;

int LevelManager::id = 0;
LM_STATE LevelManager::stateCurr = IDLE;
LM_STATE LevelManager::stateNext = IDLE;
unsigned int LevelManager::objCount = 0;
unsigned int LevelManager::objsLoaded = 0;

LevelManager::LevelManager() {}
LevelManager::~LevelManager(){}

void LevelManager::Init()
{
	keywords["GameObject"] = GAMEOBJECT;
	keywords["{"] = OPBR;
	keywords["}"] = CLBR;
	keywords["Mesh"] = MESH;
	keywords["HalfSize"] = HALFSIZE;
	keywords["UV"] = UV;
	keywords["SpriteSource"] = SPRITESOURCE;
	keywords["Size"] = SIZE;
	keywords["Texture"] = TEXTURE;
	keywords["Transform"] = TRANSFORM;
	keywords["Translation"] = TRANSLATION;
	keywords["Scale"] = SCALE;
	keywords["Rotation"] = ROTATION;
	keywords["UIElement"] = UIELEMENT;
	keywords["Sprite"] = SPRITE;
	keywords["Alpha"] = ALPHA;
	keywords["Frame"] = FRAME;
	keywords["Animation"] = ANIMATION;
	keywords["FrameDur"] = FRAMEDUR;
	keywords["IsLooping"] = ISLOOPING;
	keywords["Physics"] = PHYSICS;
	keywords["Collider"] = COLLIDER;
	keywords["Behavior"] = BEHAVIOR;
}

void LevelManager::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	switch (stateCurr)
	{
		case IDLE:
			break;
		case LOAD:
			// Load an object every cycle during the update phase.
			loadObject();
			break;
		case CREATE:
			break;
	}

	// Update state.
	if (stateCurr != stateNext)
	{
		stateCurr = stateNext;
	}
}

void LevelManager::Shutdown()
{
	// TODO: Free up memory and stuff.
}

void LevelManager::LoadLevel(string name, string dir)
{
	// Create the path string and open the file.
	string path = dir + name + ".txt";
	ifstream file(path);

	std::string line;
	int depth = 0;

	// Iterate through the file line by line.
	while (std::getline(file, line))
	{
		// Exclude comments.
		if (line.find_first_of("//", 0, 2) != string::npos)
			continue;

		// Update depth.
		if (line.find_first_of('{') != string::npos)
			depth++;
		else if (line.find_first_of('}') != string::npos)
			depth--;

		// Update objCount.
		if (depth == 0)
		{
			objCount++;
		}

		// Remove undesirable characters from the line.
		RemoveChar(line, '(');
		RemoveChar(line, ')');
		RemoveChar(line, ',');
		line.erase(remove(line.begin(), line.end(), '\t'), line.end());

		// Store line in words.
		line += ' ';
		words += line;
	}

	int index = 0;

	// Correct for double spaces.
	while (words.find_first_of(' ', index) != string::npos && index + 2 < words.length())
	{
		if (words.at(words.find_first_of(' ', index) + 1) == ' ')
			words = words.substr(0, words.find_first_of(' ', index)) + words.substr(words.find_first_of(' ', index) + 1);
		index = words.find_first_of(' ', index) + 1;
	}

	// Update state.
	stateNext = LOAD;
}

void LevelManager::loadObject()
{
	int depth = 0;
	string word;

	do
	{
	// Label used when word is a type.
	start:

		word = getNextWord();

		// Check if this is a name we're looking at.
		if (word.find_first_of('\"') != string::npos)
		{
			if (containers[id - 1]->typeStr == MESH)
			{
				((MeshContainer*)containers[id - 1])->t1 = RemoveChar(word, '\"', false);
			}
			else if (containers[id - 1]->typeStr == SPRITESOURCE)
			{
				((SpriteSourceContainer*)containers[id - 1])->t1 = RemoveChar(word, '\"', false);
			}
			else if (containers[id - 1]->typeStr == GAMEOBJECT)
			{
				((GameObjectContainer*)containers[id - 1])->t1 = RemoveChar(word, '\"', false);
			}
			else if (containers[id - 1]->typeStr == SPRITE)
			{
				((SpriteContainer*)containers[id - 1])->t1 = RemoveChar(word, '\"', false);
			}

			continue;
		}

		// If this word is nonsense, return.
		//if (!keywords[word])
		//	continue;

		stringstream ss;
		float x, y;
		bool b;

		switch (keywords[word])
		{
			case MESH:
				if (getNextWord(false) == "{")
				{
					containers[id++] = new MeshContainer();

					if (depth == 0) objsLoaded++;
					else ((SpriteContainer*)getParentByID(SPRITE))->t4 = ((MeshContainer*)containers[id - 1]);

					goto start;
				}
				else
				{
					// NOTE: This is going to potentially break things going forward.
					// At the time this code was written, the only entities which could have a mesh were Sprites.
					// If this is no longer the case, this code MUST be updated.

					((SpriteContainer*)getParentByID(SPRITE))->t4 = ((MeshContainer*)containers[names[getNextWord()]]);
				}
				break;
			case HALFSIZE:
				ss << getNextWord() << " " << getNextWord();
				ss >> x >> y;

				((MeshContainer*)getParentByID(MESH))->t2 = Vector2D(x, y);
				break;
			case UV:
				ss << getNextWord() << " " << getNextWord();
				ss >> x >> y;

				((MeshContainer*)getParentByID(MESH))->t3 = Vector2D(x, y);
				break;
			case SPRITESOURCE:
				if (getNextWord(false) == "{")
				{
					containers[id++] = new SpriteSourceContainer();

					if (depth == 0) objsLoaded++;
					else ((SpriteContainer*)getParentByID(SPRITE))->t5 = ((SpriteSourceContainer*)containers[id - 1]);

					goto start;
				}
				else
				{
					// NOTE: This is going to potentially break things going forward.
					// At the time this code was written, the only entities which could have a SpriteSource were Sprites.
					// If this is no longer the case, this code MUST be updated.

					((SpriteContainer*)getParentByID(SPRITE))->t5 = ((SpriteSourceContainer*)containers[names[getNextWord()]]);
				}
				break;
			case SIZE:
				ss << getNextWord() << " " << getNextWord();
				ss >> x >> y;

				((SpriteSourceContainer*)getParentByID(SPRITESOURCE))->t2 = Vector2D(x, y);
				break;
			case TEXTURE:
				((SpriteSourceContainer*)getParentByID(SPRITESOURCE))->t3 = getNextWord();
				break;
			case GAMEOBJECT:
				containers[id++] = new GameObjectContainer();
				
				if (depth == 0) objsLoaded++;

				goto start;
			case TRANSFORM:
				containers[id++] = new TransformContainer();

				// NOTE: This is going to potentially break things going forward.
				// At the time this code was written, the only entities which could have a Transform were GameObjects.
				// If this is no longer the case, this code MUST be updated.
				((GameObjectContainer*)getParentByID(TRANSFORM))->t2 = ((TransformContainer*)containers[id - 1]);

				goto start;
			case TRANSLATION:
				ss << getNextWord() << " " << getNextWord();
				ss >> x >> y;

				((TransformContainer*)getParentByID(TRANSFORM))->t1 = Vector2D(x, y);
				break;
			case SCALE:
				ss << getNextWord() << " " << getNextWord();
				ss >> x >> y;

				((TransformContainer*)getParentByID(TRANSFORM))->t2 = Vector2D(x, y);
				break;
			case ROTATION:
				ss << getNextWord();
				ss >> x;

				((TransformContainer*)getParentByID(TRANSFORM))->t3 = x;
				break;
			case UIELEMENT:
				ss << getNextWord();
				ss >> b;

				((TransformContainer*)getParentByID(TRANSFORM))->t4 = b;
				break;
			case SPRITE:
				containers[id++] = new SpriteContainer();
				goto start;
				break;
			case ALPHA:
				ss << getNextWord();
				ss >> x;

				((SpriteContainer*)getParentByID(SPRITE))->t2 = x;
				break;
			case FRAME:
				ss << getNextWord();
				ss >> x;

				((SpriteContainer*)getParentByID(SPRITE))->t3 = (int)x;
				break;
			case ANIMATION:
				containers[id++] = new AnimationContainer();

				// NOTE: This is going to potentially break things going forward.
				// At the time this code was written, the only entities which could have an Animation were GameObjects.
				// If this is no longer the case, this code MUST be updated.
				((GameObjectContainer*)getParentByID(GAMEOBJECT))->t4 = ((AnimationContainer*)containers[id - 1]);

				goto start;
			case FRAMEDUR:
				ss << getNextWord();
				ss >> x;

				((AnimationContainer*)getParentByID(ANIMATION))->t1 = x;
				break;
			case ISLOOPING:
				ss << getNextWord();
				ss >> b;

				((AnimationContainer*)getParentByID(ANIMATION))->t2 = b;
				break;
			case PHYSICS:
				containers[id++] = new PhysicsContainer();

				// NOTE: This is going to potentially break things going forward.
				// At the time this code was written, the only entities which could have a Physics were GameObjects.
				// If this is no longer the case, this code MUST be updated.
				((GameObjectContainer*)getParentByID(GAMEOBJECT))->t5 = ((PhysicsContainer*)containers[id - 1]);

				goto start;
			case COLLIDER:
				containers[id++] = new ColliderContainer();

				// NOTE: This is going to potentially break things going forward.
				// At the time this code was written, the only entities which could have a Collider were GameObjects.
				// If this is no longer the case, this code MUST be updated.
				((GameObjectContainer*)getParentByID(GAMEOBJECT))->t6 = ((ColliderContainer*)containers[id - 1]);

				goto start;
			case BEHAVIOR:
				containers[id++] = new BehaviorContainer();

				// NOTE: This is going to potentially break things going forward.
				// At the time this code was written, the only entities which could have a Behavior were GameObjects.
				// If this is no longer the case, this code MUST be updated.
				((GameObjectContainer*)getParentByID(GAMEOBJECT))->t7 = ((BehaviorContainer*)containers[id - 1]);

				goto start;
			case OPBR:
				depth++;
				break;
			case CLBR:
				depth--;
				break;
		}
	} while (depth != 0);
}

LevelManager::BaseClass* LevelManager::getParentByID(LM_KEYWORD type)
{
	for (int i = id - 1; i >= 0; i--)
	{
		if (containers[i]->typeStr == type)
		{
			return containers[i];
		}
	}

	return nullptr;
}

string LevelManager::getNextWord(bool remove)
{
	if (words.find_first_of(" ") == string::npos)
		return "";

	string retVal = words.substr(0, words.find_first_of(" "));

	if (remove)
		words = words.substr(words.find_first_of(" ") + 1, words.size());

	return retVal;
}

string LevelManager::RemoveChar(string& str, const char toFind, bool doSpaces)
{
	replace(str.begin(), str.end(), toFind, ' ');

	if (!doSpaces)
		str.erase(remove(str.begin(), str.end(), ' '), str.end());

	return str;
}

LevelManager& LevelManager::GetInstance()
{
	static LevelManager lm;

	return lm;
}