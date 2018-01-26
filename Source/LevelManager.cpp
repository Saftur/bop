#include "stdafx.h"
#include "LevelManager.h"
#include "GameObjectManager.h"
#include "Vector2D.h"

using namespace std;

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
				goto start;
				break;
			case TRANSFORM:
				containers[id++] = new TransformContainer();
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