#include "stdafx.h"
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "GameObjectManager.h"
#include "Trace.h"
#include "Mesh.h"
#include "AEEngine.h"

#include "LevelManager_depricated.h"

// TODO: Clean up the code in here.

LM_Mode LevelManager_depricated::stateCurr = IDLE;
LM_Mode LevelManager_depricated::stateNext = IDLE;
LM_Parent LevelManager_depricated::p1 = NONE;
LM_Parent LevelManager_depricated::p2 = NONE;
LM_Parent LevelManager_depricated::p3 = NONE;
int LevelManager_depricated::depth = 0;
int LevelManager_depricated::id = 0;

int LevelManager_depricated::genID()
{
	return id++;
}

LevelManager_depricated& LevelManager_depricated::GetInstance()
{
	static LevelManager_depricated lm;

	return lm;
}

LevelManager_depricated::LevelManager_depricated() {}
LevelManager_depricated::~LevelManager_depricated() {}

void LevelManager_depricated::Init()
{
	Trace::GetInstance().GetStream() << "LevelManager_depricated: Init" << std::endl;

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

void LevelManager_depricated::Load(const char* levelName)
{
	std::string str = "Levels\\";
	str.append(levelName);
	str.append(".txt");
	
	Trace::GetInstance().GetStream() << "LevelManager_depricated: Load \"" << str << "\"" << std::endl;

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

void LevelManager_depricated::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	Trace::GetInstance().GetStream() << "LevelManager_depricated: Update" << std::endl;

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
			Trace::GetInstance().GetStream() << "LevelManager_depricated: Starting creation..." << std::endl;
			stateNext = CREATE;
		}

		word = GetNextWord(contents, true);

		if (strs.find(word) != strs.end())
		{
			switch (strs.at(word))
			{
			case GameObject:
				tmpGO = new TmpGO();
				tmpGO->ID = genID();
				tmpGO->type = "GO";
				type = GO;
				break;
			case Mesh:
				if (GetNextWord(contents) != "{")
					break;
				tmpMesh = new TmpMesh();
				tmpMesh->ID = genID();
				tmpMesh->type = "ME";
				type = MESH;
				break;
			case SpriteSource:
				if (GetNextWord(contents) != "{")
					break;
				tmpSpriteSrc = new TmpSpriteSrc();
				tmpSpriteSrc->ID = genID();
				tmpSpriteSrc->type = "SS";
				type = SPRSRC;
				break;
			case Transform:
				tmpTransform = new TmpTransform();
				tmpTransform->ID = genID();
				tmpTransform->type = "TR";
				type = TRANS;
				break;
			case Sprite:
				tmpSprite = new TmpSprite();
				tmpSprite->ID = genID();
				tmpSprite->type = "SP";
				type = SPRITE;
				break;
			case Animation:
				tmpAnim = new TmpAnim();
				tmpAnim->ID = genID();
				tmpAnim->type = "AN";
				type = ANIM;
				break;
			case Physics:
				tmpPhys = new TmpPhys();
				tmpPhys->ID = genID();
				tmpPhys->type = "PH";
				type = PHYS;
				break;
			case Collider:
				tmpColl = new TmpColl();
				tmpColl->ID = genID();
				tmpColl->type = "CO";
				type = COLL;
				break;
			case Behavior:
				tmpBehavior = new TmpBehavior();
				tmpBehavior->ID = genID();
				tmpBehavior->type = "BE";
				type = BEHAV;
				break;
			case Platform:
				tmpPlatform = new TmpPlatform();
				tmpPlatform->ID = genID();
				tmpPlatform->type = "PL";
				type = PLAT;
				break;
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

					objs[activeObj->ID] = activeObj;

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
						tmpSprite->Mesh = (TmpMesh*)objs[getObjByName(GetNextWord(contents, true))];
						break;
					case SpriteSource:
						tmpSprite->SpriteSource = (TmpSpriteSrc*)objs[getObjByName(GetNextWord(contents, true))];
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
					break;
				case COLL:
					break;
				case BEHAV:
					tmpBehavior->Type = word;
					break;
				case PLAT:
					switch (strs.at(word))
					{
					case Jump:
						sstream << GetNextWord(contents, true);
						sstream >> x;

						tmpPlatform->JumpHeight = x;
						break;
					case Speed:
						sstream << GetNextWord(contents, true);
						sstream >> x;

						tmpPlatform->Speed = x;
						break;
					}
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
		for (int i = 0; i < id; i++)
		{
			if (!objs[i])
				continue;
			if (objs[i]->type == "SS")
			{
				// Initialize all components first.
				textures[i] = AEGfxTextureLoad(((TmpSpriteSrc*)objs[i])->Texture.c_str());

				// Create the object & initialize.
				spriteSources[i] = new ::SpriteSource(((TmpSpriteSrc*)objs[i])->Cols, ((TmpSpriteSrc*)objs[i])->Rows, textures[i]);
			}
			else if (objs[i]->type == "ME")
			{
				// Create the object & initialize.
				verts[i] = MeshCreateQuad(((TmpMesh*)objs[i])->HalfSize.X(), ((TmpMesh*)objs[i])->HalfSize.Y(), ((TmpMesh*)objs[i])->UV.X(), ((TmpMesh*)objs[i])->UV.Y(), ((TmpMesh*)objs[i])->Name.c_str());
			}
			else if (objs[i]->type == "GO")
			{
				::GameObject* go = new ::GameObject(((TmpGO*)objs[i])->Name.c_str());

				if (!verts.at(((TmpGO*)objs[i])->Sprite->Mesh->ID))
					verts[i] = MeshCreateQuad(((TmpGO*)objs[i])->Sprite->Mesh->HalfSize.X(), ((TmpGO*)objs[i])->Sprite->Mesh->HalfSize.Y(), ((TmpGO*)objs[i])->Sprite->Mesh->UV.X(), ((TmpGO*)objs[i])->Sprite->Mesh->UV.Y(), ((TmpGO*)objs[i])->Sprite->Mesh->Name.c_str());

				if (!textures.at(((TmpGO*)objs[i])->Sprite->SpriteSource->ID))
				{
					textures[i] = AEGfxTextureLoad(((TmpGO*)objs[i])->Sprite->SpriteSource->Texture.c_str());
					spriteSources[i] = new ::SpriteSource(((TmpGO*)objs[i])->Sprite->SpriteSource->Cols, ((TmpGO*)objs[i])->Sprite->SpriteSource->Rows, textures[i]);
				}

				::Sprite* spr = new ::Sprite(((TmpGO*)objs[i])->Sprite->Name.c_str());
				go->SetSprite(*spr);

				::Animation* an = new ::Animation(go->GetSprite());
				go->SetAnimation(*an);

				::Transform* tr = new ::Transform(((TmpGO*)objs[i])->Transform->Translation.X(), ((TmpGO*)objs[i])->Transform->Translation.Y());
				go->SetTransform(*tr);

				::Collider* co = new ::Collider(*go);
				go->SetCollider(*co);

				::Physics* ph = new ::Physics();
				go->SetPhysics(*ph);

				// TODO: Behaviors?

				// Register with GameObjectManager.
				GameObjectManager::GetInstance().Add(*go);

				gos[i] = go;
			}
			else if (objs[i]->type == "PL")
			{
				// Initialize all components first.
				::Transform* tr = new ::Transform(((TmpPlatform*)objs[i])->Transform->Translation.X(), ((TmpPlatform*)objs[i])->Transform->Translation.Y());

				// TODO: std::vectors
				
				// Create the object & initialize.
				PlatformManager::AddPlatform(*tr, ((TmpPlatform*)objs[i])->JumpHeight, ((TmpPlatform*)objs[i])->Speed);
			}
		}

		stateNext = FINISHED;

		break;
	case FINISHED:
		stateNext = IDLE;
		break;
	}

	if (stateCurr != stateNext)
		stateCurr = stateNext;
}

int LevelManager_depricated::getObjByName(std::string name)
{
	for (int i = 0; i < id; i++)
	{
		__pragma(warning(push))
		__pragma(warning(disable:4127))
		if (sizeof(objs[i]) == sizeof(TmpGO))
		{
			if (((TmpGO*)objs[i])->Name == name)
			{
				return objs[i]->ID;
			}
		}
		else if (sizeof(objs[i]) == sizeof(TmpMesh))
		{
			if (((TmpMesh*)objs[i])->Name == name)
			{
				return objs[i]->ID;
			}
		}
		else if (sizeof(objs[i]) == sizeof(TmpSprite))
		{
			if (((TmpSprite*)objs[i])->Name == name)
			{
				return objs[i]->ID;
			}
		}
		else if (sizeof(objs[i]) == sizeof(TmpSpriteSrc))
		{
			if (((TmpSpriteSrc*)objs[i])->Name == name)
			{
				return objs[i]->ID;
			}
		}
		__pragma(warning(pop))
	}

	return -1;
}

std::string LevelManager_depricated::GetNextWord(std::string& str, bool remove)
{
	if (str.find_first_of(" ") == std::string::npos)
		return "";

	std::string retVal = str.substr(0, str.find_first_of(" "));

	if (remove)
		str = str.substr(str.find_first_of(" ") + 1, str.size());

	return retVal;
}

void LevelManager_depricated::Debug()
{

}