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
	
	Trace::GetInstance().GetStream() << "LevelManager: Load \"" << str << "\"" << std::endl;

	std::ifstream file(str);
	std::string line;

	while (std::getline(file, line))
	{
		// Replace all () with spaces.
		if (line.find_first_of(" (") != std::string::npos)
		{
			std::string tmp = line.substr(line.find_first_of("("), line.size());
			line = line.substr(0, line.find_first_of("("));
			line.append(tmp);
		}
		else if(line.find_first_of("(") != std::string::npos)
		{
			std::replace(line.begin(), line.end(), '(', ' ');
		}

		if (line.find_first_of(") ") != std::string::npos)
		{
			std::string tmp = line.substr(line.find_first_of(") "), line.size());
			line = line.substr(0, line.find_first_of(")"));
			line.append(tmp);
		}
		else if (line.find_first_of(")") != std::string::npos)
		{
			std::replace(line.begin(), line.end(), ')', ' ');
		}

		std::replace(line.begin(), line.end(), ',', ' ');

		contents.append(line);
		contents.append(" ");
	}

	stateNext = LOAD;
}

void LevelManager::Update(float dt)
{
	Trace::GetInstance().GetStream() << "LevelManager: Update" << std::endl;

	std::string word;
	std::stringstream sstream;
	LM_Parent type = NONE;
	LM_Parent activeParent;
	LM_Parent activeChild;
	float x, y;

	switch (stateCurr)
	{
	case IDLE:
		break;
	case LOAD:
		word = GetNextWord(contents, true);
		
		switch (strs[word])
		{
		case GameObject:
			tmpGO = new TmpGO();
			type = GO;
			break;
		case str2int("Mesh"):
			if (GetNextWord(contents) != "{")
				break;
			tmpMesh = new TmpMesh();
			type = MESH;
			break;
		case str2int("SpriteSource"):
			if (GetNextWord(contents) != "{")
				break;
			tmpSpriteSrc = new TmpSpriteSrc();
			type = SPRSRC;
			break;
		case str2int("Transform"):
			tmpTransform = new TmpTransform();
			type = TRANS;
			break;
		case str2int("Sprite"):
			tmpSprite = new TmpSprite();
			type = SPRITE;
			break;
		case str2int("Animation"):
			tmpAnim = new TmpAnim();
			type = ANIM;
			break;
		case str2int("Physics"):
			tmpPhys = new TmpPhys();
			type = PHYS;
			break;
		case str2int("Collider"):
			tmpColl = new TmpColl();
			type = COLL;
			break;
		case str2int("Behavior"):
			tmpBehavior = new TmpBehavior();
			type = BEHAV;
			break;
		case str2int("Platform"):
			tmpPlatform = new TmpPlatform();
			type = PLAT;
		case str2int("{"):
			depth++;
			break;
		case str2int("}"):
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
			break;
		default:
			switch (depth)
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
					tmpGO->Name = word.substr(1, word.length() - 1);
					break;
				}
				break;
			case MESH:
				// Is this a name?
				if (word.find_first_of("\"") != std::string::npos)
				{
					tmpMesh->Name = word.substr(1, word.length() - 1);
					break;
				}

				// Parse the word & store.
				switch (str2int(word.c_str()))
				{
				case str2int("HalfSize"):
					sstream << GetNextWord(contents, true) << ' ' << GetNextWord(contents, true);
					sstream >> x >> y;

					tmpMesh->HalfSize = Vector2D(x, y);
					break;
				case str2int("UV"):
					sstream << GetNextWord(contents, true) << ' ' << GetNextWord(contents, true);
					sstream >> x >> y;

					tmpMesh->UV = Vector2D(x, y);
					break;
				}
				break;
			case SPRSRC:
				// Is this a name?
				if (word.find_first_of("\"") != std::string::npos)
				{
					tmpSpriteSrc->Name = word.substr(1, word.length() - 1);
					break;
				}

				// Parse the word & store.
				switch (str2int(word.c_str()))
				{
				case str2int("Size"):
					sstream << GetNextWord(contents, true) << ' ' << GetNextWord(contents, true);
					sstream >> x >> y;

					tmpSpriteSrc->Rows = x;
					tmpSpriteSrc->Cols = x;
					break;
				case str2int("Texture"):
					tmpSpriteSrc->Texture = GetNextWord(contents, true);
					break;
				}
				break;
			case TRANS:
				// Parse the word & store.
				switch (str2int(word.c_str()))
				{
				case str2int("Translation"):
					sstream << GetNextWord(contents, true) << ' ' << GetNextWord(contents, true);
					float x, y;
					sstream >> x >> y;

					tmpTransform->Translation = Vector2D(x, y);
					break;
				case str2int("Scale"):
					sstream << GetNextWord(contents, true) << ' ' << GetNextWord(contents, true);
					sstream >> x >> y;

					tmpTransform->Scale = Vector2D(x, y);
					break;
				case str2int("Rotation"):
					sstream << GetNextWord(contents, true);
					sstream >> x;

					tmpTransform->Rotation = x;
					break;
				case str2int("UIElement"):
					sstream << GetNextWord(contents, true);
					bool uiel;
					sstream >> uiel;

					tmpTransform->UIElement = uiel;
					break;
				}
				break;
			case SPRITE:
				// Is this a name?
				if (word.find_first_of("\"") != std::string::npos)
				{
					tmpSprite->Name = word.substr(1, word.length() - 1);
					break;
				}

				// Parse the word & store.
				switch (str2int(word.c_str()))
				{
				case str2int("Alpha"):
					sstream << GetNextWord(contents, true);
					float a;
					sstream >> a;

					tmpSprite->Alpha = a;
					break;
				case str2int("Frame"):
					sstream << GetNextWord(contents, true);
					int frame;
					sstream >> frame;

					tmpSprite->Frame = frame;
					break;
				case str2int("Mesh"):
					tmpSprite->Mesh = (TmpMesh*)objs[GetNextWord(contents, true)];
					break;
				case str2int("SpriteSoure"):
					tmpSprite->SpriteSource = (TmpSpriteSrc*)objs[GetNextWord(contents, true)];
					break;
				}
				break;
			case ANIM:
				// Parse the word & store.
				switch (str2int(word.c_str()))
				{
				case str2int("FrameDur"):
					sstream << GetNextWord(contents, true);
					float frameDur;
					sstream >> frameDur;

					tmpAnim->FrameDuration = frameDur;
					break;
				case str2int("IsLooping"):
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
	case FIND_CHILDREN:
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
		str = str.substr(str.find_first_of(" ") + 1, str.end);

	return retVal;
}

constexpr unsigned int LevelManager::str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

