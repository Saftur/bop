#include "stdafx.h"
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "GameObjectManager.h"
#include "Trace.h"

#include "LevelManager.h"

LM_Mode LevelManager::stateCurr = IDLE;
LM_Mode LevelManager::stateNext = IDLE;
LM_Parent LevelManager::p1 = NONE;
LM_Parent LevelManager::p2 = NONE;
LM_Parent LevelManager::p3 = NONE;
int LevelManager::depth = 0;

LevelManager& LevelManager::GetInstance()
{
	static LevelManager lm;

	return lm;
}

LevelManager::LevelManager() {}
LevelManager::~LevelManager() {}

void LevelManager::Init()
{
	Trace::GetInstance().GetStream() << "LevelManager: Init" << std::endl;

	tmpAnim = nullptr;
	tmpBehavior = nullptr;
	tmpColl = nullptr;
	tmpGO = nullptr;
	tmpMesh = nullptr;
	tmpPhys = nullptr;
	tmpPlatform = nullptr;
	tmpSprite = nullptr;
	tmpSpriteSrc = nullptr;
	tmpTransform = nullptr;

	strs["GameObject"] = GameObject;
	strs["{"] = OpBr;
	strs["}"] = ClBr;
	strs["Mesh"] = Mesh;
	strs["HalfSize"] = HalfSize;
	strs["UV"] = UV;
	strs["SpriteSource"] = SpriteSource;
	strs["Size"] = Size;
	strs["Texture"] = Texture;
	strs["Transform"] = Transform;
	strs["Translation"] = Translation;
	strs["Scale"] = Scale;
	strs["Rotation"] = Rotation;
	strs["UIElement"] = UIElement;
	strs["Sprite"] = Sprite;
	strs["Alpha"] = Alpha;
	strs["Frame"] = Frame;
	strs["Animation"] = Animation;
	strs["FrameDur"] = FrameDur;
	strs["IsLooping"] = IsLooping;
	strs["Physics"] = Physics;
	strs["Collider"] = Collider;
	strs["Behavior"] = Behavior;
	strs["Platform"] = Platform;
	strs["Jump"] = Jump;
	strs["Speed"] = Speed;
	strs["Vector"] = Vector;
}

void LevelManager::Load(const char* levelName)
{
	std::string str = "Levels\\";
	str.append(levelName);
	str.append(".txt");
	
	Trace::GetInstance().GetStream() << "LevelManager: Load \"" << str << "\"" << std::endl;

	std::ifstream file(str);
	std::string line;

	while (std::getline(file, line))
	{
		// Replace all () with spaces.
		if (line.find_first_of("(") != std::string::npos)
		{
			std::string tmp = line.substr(line.find_first_of("(") + 1, line.size());
			line = line.substr(0, line.find_first_of("("));
			line.append(" ");
			line.append(tmp);
		}

		if (line.find_first_of(")") != std::string::npos)
		{
			std::string tmp = line.substr(line.find_first_of(")") + 1, line.size());
			line = line.substr(0, line.find_first_of(")"));
			line.append(tmp);
		}

		line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());

		std::replace(line.begin(), line.end(), ',', ' ');

		contents.append(line);
		contents.append(" ");
	}

	stateNext = LOAD;
}

void LevelManager::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	Trace::GetInstance().GetStream() << "LevelManager: Update" << std::endl;

	std::string word;
	std::stringstream sstream;
	LM_Parent type = NONE;
	LM_Parent activeParent = NONE;
	LM_Parent activeChild = NONE;
	float x, y;

	switch (stateCurr)
	{
	case IDLE:
		break;
	case LOAD:
		if (contents == "")
		{
			Trace::GetInstance().GetStream() << "LevelManager: Starting creation..." << std::endl;
			stateNext = CREATE;
		}

		word = GetNextWord(contents, true);

		if (strs.find(word) != strs.end())
		{
			switch (strs.at(word))
			{
			case GameObject:
				tmpGO = new TmpGO();
				type = GO;
				break;
			case Mesh:
				if (GetNextWord(contents) != "{")
					break;
				tmpMesh = new TmpMesh();
				type = MESH;
				break;
			case SpriteSource:
				if (GetNextWord(contents) != "{")
					break;
				tmpSpriteSrc = new TmpSpriteSrc();
				type = SPRSRC;
				break;
			case Transform:
				tmpTransform = new TmpTransform();
				type = TRANS;
				break;
			case Sprite:
				tmpSprite = new TmpSprite();
				type = SPRITE;
				break;
			case Animation:
				tmpAnim = new TmpAnim();
				type = ANIM;
				break;
			case Physics:
				tmpPhys = new TmpPhys();
				type = PHYS;
				break;
			case Collider:
				tmpColl = new TmpColl();
				type = COLL;
				break;
			case Behavior:
				tmpBehavior = new TmpBehavior();
				type = BEHAV;
				break;
			case Platform:
				tmpPlatform = new TmpPlatform();
				type = PLAT;
			case OpBr:
				depth++;
				break;
			case ClBr:
				switch (depth)
				{
				case 1:
					activeParent = p1;
					activeChild = p2;
					break;
				case 2:
					activeParent = p2;
					activeChild = p3;
					break;
				}

				switch (activeParent)
				{
				case GO:
					switch (activeChild)
					{
					case TRANS:
						tmpGO->Transform = tmpTransform;
						break;
					case SPRITE:
						tmpGO->Sprite = tmpSprite;
						break;
					case ANIM:
						tmpGO->Animation = tmpAnim;
						break;
					case PHYS:
						tmpGO->Physics = tmpPhys;
						break;
					case COLL:
						tmpGO->Collider = tmpColl;
						break;
					case BEHAV:
						tmpGO->Behavior = tmpBehavior;
						break;
					}
					break;
				case PLAT:
					switch (activeChild)
					{
					case TRANS:
						tmpPlatform->Transform = tmpTransform;
						break;
					}
					break;
				case SPRITE:
					switch (activeChild)
					{
					case MESH:
						tmpSprite->Mesh = tmpMesh;
						break;
					case SPRSRC:
						tmpSprite->SpriteSource = tmpSpriteSrc;
						break;
					}
					break;
				}
				depth--;

				if (depth == 0)
				{
					Base* activeObj = nullptr;

					switch (activeParent)
					{
					case GO:
						activeObj = tmpGO;
						break;
					case PLAT:
						activeObj = tmpPlatform;
						break;
					case SPRITE:
						activeObj = tmpSprite;
						break;
					case MESH:
						activeObj = tmpMesh;
						break;
					case SPRSRC:
						activeObj = tmpSpriteSrc;
						break;
					case TRANS:
						activeObj = tmpTransform;
					}

					if (activeObj->Name == "")
						activeObj->Name = "default";

					objs[activeObj->Name] = activeObj;

					tmpAnim = nullptr;
					tmpBehavior = nullptr;
					tmpColl = nullptr;
					tmpGO = nullptr;
					tmpMesh = nullptr;
					tmpPhys = nullptr;
					tmpPlatform = nullptr;
					tmpSprite = nullptr;
					tmpSpriteSrc = nullptr;
					tmpTransform = nullptr;
				}
				break;
			default:
				switch (depth - 1)
				{
				case 0:
					activeParent = p1;
					break;
				case 1:
					activeParent = p2;
					break;
				case 2:
					activeParent = p3;
					break;
				}

				switch (activeParent)
				{
				case GO:
					break;
				case MESH:
					// Parse the word & store.
					switch (strs.at(word))
					{
					case HalfSize:
						sstream << GetNextWord(contents, true) << ' ' << GetNextWord(contents, true);
						sstream >> x >> y;

						tmpMesh->HalfSize = Vector2D(x, y);
						break;
					case UV:
						sstream << GetNextWord(contents, true) << ' ' << GetNextWord(contents, true);
						sstream >> x >> y;

						tmpMesh->UV = Vector2D(x, y);
						break;
					}
					break;
				case SPRSRC:
					// Parse the word & store.
					switch (strs.at(word))
					{
					case Size:
						sstream << GetNextWord(contents, true) << ' ' << GetNextWord(contents, true);
						sstream >> x >> y;

						tmpSpriteSrc->Rows = (int)x;
						tmpSpriteSrc->Cols = (int)y;
						break;
					case Texture:
						tmpSpriteSrc->Texture = GetNextWord(contents, true);
						break;
					}
					break;
				case TRANS:
					// Parse the word & store.
					switch (strs.at(word))
					{
					case Translation:
						sstream << GetNextWord(contents, true) << ' ' << GetNextWord(contents, true);
						sstream >> x >> y;

						tmpTransform->Translation = Vector2D(x, y);
						break;
					case Scale:
						sstream << GetNextWord(contents, true) << ' ' << GetNextWord(contents, true);
						sstream >> x >> y;

						tmpTransform->Scale = Vector2D(x, y);
						break;
					case Rotation:
						sstream << GetNextWord(contents, true);
						sstream >> x;

						tmpTransform->Rotation = x;
						break;
					case UIElement:
						sstream << GetNextWord(contents, true);
						bool uiel;
						sstream >> uiel;

						tmpTransform->UIElement = uiel;
						break;
					}
					break;
				case SPRITE:
					// Parse the word & store.
					switch (strs.at(word))
					{
					case Alpha:
						sstream << GetNextWord(contents, true);
						float a;
						sstream >> a;

						tmpSprite->Alpha = a;
						break;
					case Frame:
						sstream << GetNextWord(contents, true);
						int frame;
						sstream >> frame;

						tmpSprite->Frame = frame;
						break;
					case Mesh:
						tmpSprite->Mesh = (TmpMesh*)objs[GetNextWord(contents, true)];
						break;
					case SpriteSource:
						tmpSprite->SpriteSource = (TmpSpriteSrc*)objs[GetNextWord(contents, true)];
						break;
					}
					break;
				case ANIM:
					// Parse the word & store.
					switch (strs.at(word))
					{
					case FrameDur:
						sstream << GetNextWord(contents, true);
						float frameDur;
						sstream >> frameDur;

						tmpAnim->FrameDuration = frameDur;
						break;
					case IsLooping:
						sstream << GetNextWord(contents, true);
						bool loop;
						sstream >> loop;

						tmpAnim->IsLooping = loop;
						break;
					}
					break;
				case PHYS:
					tmpPhys->Name = "GOPhysics";
					break;
				case COLL:
					tmpColl->Name = "GOCollider";
					break;
				case BEHAV:
					tmpBehavior->Type = word;
					break;
				}
				break;
			}
		} 
		else
		{
			switch (depth - 1)
			{
			case 0:
				activeParent = p1;
				break;
			case 1:
				activeParent = p2;
				break;
			case 2:
				activeParent = p3;
				break;
			}

			switch (activeParent)
			{
			case GO:
				// Is this a name?
				if (word.find_first_of("\"") != std::string::npos)
				{
					word.erase(std::remove(word.begin(), word.end(), '\"'), word.end());
					tmpGO->Name = word;
					break;
				}
				break;
			case MESH:
				// Is this a name?
				if (word.find_first_of("\"") != std::string::npos)
				{
					word.erase(std::remove(word.begin(), word.end(), '\"'), word.end());
					tmpMesh->Name = word;
					break;
				}
				break;
			case SPRSRC:
				// Is this a name?
				if (word.find_first_of("\"") != std::string::npos)
				{
					word.erase(std::remove(word.begin(), word.end(), '\"'), word.end());
					tmpSpriteSrc->Name = word;
					break;
				}
				break;
			case TRANS:
				break;
			case SPRITE:
				// Is this a name?
				if (word.find_first_of("\"") != std::string::npos)
				{
					word.erase(std::remove(word.begin(), word.end(), '\"'), word.end());
					tmpSprite->Name = word;
					break;
				}
				break;
			case ANIM:
				break;
			case PHYS:
				break;
			case COLL:
				break;
			case BEHAV:
				tmpBehavior->Type = word;
				break;
			}
		}

		if (type != NONE)
		{
			switch (depth)
			{
			case 0:
				p1 = type;
				break;
			case 1:
				p2 = type;
				break;
			case 2:
				p3 = type;
				break;
			}
		}
		break;
	case CREATE:
		for ()
		break;
	case FINISHED:
		stateNext = IDLE;
		break;
	}

	if (stateCurr != stateNext)
		stateCurr = stateNext;
}

std::string LevelManager::GetNextWord(std::string& str, bool remove)
{
	if (str.find_first_of(" ") == std::string::npos)
		return "";

	std::string retVal = str.substr(0, str.find_first_of(" "));

	if (remove)
		str = str.substr(str.find_first_of(" ") + 1, str.size());

	return retVal;
}

void LevelManager::Debug()
{

}