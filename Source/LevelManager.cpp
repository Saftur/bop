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

void LevelManager::Init()
{
	// TODO: Initialize maps and stuff
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

void LevelManager::LoadLevel(string name, string dir = "Levels\\")
{
	// Create the path string and open the file.
	string path = dir + name + ".lvl";
	ifstream file(path);

	std::string line;
	int depth = 0;

	// Iterate through the file line by line.
	while (std::getline(file, line))
	{
		// Exclude comments.
		if (line.find_first_of("//", 0, 2))
			continue;

		// Update depth.
		if (line.find_first_of('{'))
			depth++;
		else if (line.find_first_of('}'))
			depth--;

		// Update objCount.
		if (depth == 0)
		{
			objCount++;
		}

		// Remove undesirable characters from the line.
		replace(line.begin(), line.end(), '(', ' ');
		replace(line.begin(), line.end(), ')', ' ');
		replace(line.begin(), line.end(), ',', ' ');
		line.erase(remove(line.begin(), line.end(), '\t'), line.end());

		// Store line in words.
		line += ' ';
		words += line;
	}

	int index = 0;

	// Correct for double spaces.
	while (words.find_first_of(' ', index) != string::npos)
	{
		if (words.at(words.find_first_of(' ', index) + 1) == ' ')
			words = words.substr(0, words.find_first_of(' ', index)) + words.substr(words.find_first_of(' ', index) + 2);
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

		// If this word is nonsense, return.
		if (!keywords[word])
			return;

		stringstream ss;
		float x, y;

		switch (keywords[word])
		{
			case MESH:
				if (getNextWord(false) == "{")
				{
					containers[id++] = new MeshContainer();
					goto start;
				}
				else
				{
					// NOTE: This is going to potentially break things going forward.
					// At the time this code was written, the only entities which could have a mesh were Sprites.
					// If this is no longer the case, this code MUST be updated.

					((SpriteContainer*)getParentByID("Sprite"))->t4 = ((MeshContainer*)containers[names[getNextWord()]]);
				}
				break;
			case HALFSIZE:
				ss << getNextWord() << " " << getNextWord();
				ss >> x >> y;

				((MeshContainer*)getParentByID("Mesh"))->t2 = Vector2D(x, y);
				break;
			case UV:
				ss << getNextWord() << " " << getNextWord();
				ss >> x >> y;

				((MeshContainer*)getParentByID("Mesh"))->t3 = Vector2D(x, y);
				break;
			case SPRITESOURCE:
				if (getNextWord(false) == "{")
				{
					containers[id++] = new SpriteSourceContainer();
					if (depth == 0) objsLoaded++;
					goto start;
				}
				else
				{
					// NOTE: This is going to potentially break things going forward.
					// At the time this code was written, the only entities which could have a SpriteSource were Sprites.
					// If this is no longer the case, this code MUST be updated.

					((SpriteContainer*)getParentByID("Sprite"))->t5 = ((SpriteSourceContainer*)containers[names[getNextWord()]]);
				}
				break;
			case SIZE:
				ss << getNextWord() << " " << getNextWord();
				ss >> x >> y;

				((SpriteSourceContainer*)getParentByID("SpriteSource"))->t2 = Vector2D(x, y);
				break;
			case TEXTURE:
				((SpriteSourceContainer*)getParentByID("SpriteSource"))->t3 = getNextWord();
				break;
			case GAMEOBJECT:
				containers[id++] = new GameObjectContainer();
				if (depth == 0) objsLoaded++;
				goto start;
				break;
			case TRANSFORM:
				containers[id++] = new TransformContainer();
				if (depth == 0) objsLoaded++;
				goto start;
				break;
			case TRANSLATION:
				ss << getNextWord() << " " << getNextWord();
				ss >> x >> y;

				((TransformContainer*)getParentByID("Transform"))->t1 = Vector2D(x, y);
				break;
			case SCALE:
				ss << getNextWord() << " " << getNextWord();
				ss >> x >> y;

				((TransformContainer*)getParentByID("Transform"))->t2 = Vector2D(x, y);
				break;
			case ROTATION:
				ss << getNextWord();
				ss >> x;

				((TransformContainer*)getParentByID("Transform"))->t3 = x;
				break;
			case UIELEMENT:
				bool b;
				ss << getNextWord();
				ss >> b;

				((TransformContainer*)getParentByID("Transform"))->t4 = b;
				break;
			case SPRITE:
				containers[id++] = new SpriteContainer();
				goto start;
				break;
			case ALPHA:
				ss << getNextWord();
				ss >> x;

				((SpriteContainer*)getParentByID("Sprite"))->t2 = x;
				break;
			case FRAME:
				ss << getNextWord();
				ss >> x;

				((SpriteContainer*)getParentByID("Sprite"))->t3 = (int)x;
				break;
			case ANIMATION:
				containers[id++] = new AnimationContainer();
				goto start;
				break;
			case FRAMEDUR:
				ss << getNextWord();
				ss >> x;

				((AnimationContainer*)getParentByID("Animation"))->t1 = x;
				break;
			case ISLOOPING:
				bool b;
				ss << getNextWord();
				ss >> b;

				((AnimationContainer*)getParentByID("Animation"))->t2 = b;
				break;
			case PHYSICS:
				containers[id++] = new PhysicsContainer();
				goto start;
				break;
			case COLLIDER:
				containers[id++] = new ColliderContainer();
				goto start;
				break;
			case BEHAVIOR:
				containers[id++] = new BehaviorContainer();
				goto start;
				break;
			case OPBR:
				depth++;
				break;
			case CLBR:
				depth--;
				break;
		}
	} while (depth != 0);
}

LevelManager::BaseClass* LevelManager::getParentByID(string type)
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