#include <string>
#include <fstream>
#include <sstream>

#include "GameObjectManager.h"

#include "LevelManager.h"

LevelManager& LevelManager::GetInstance()
{
	static LevelManager lm;

	return lm;
}

void LevelManager::Load(const char* levelName)
{
	std::string str = "Levels\\";
	str.append(levelName);

	std::ifstream file(str);
	std::string contents;
	std::string line;

	while (std::getline(file, line))
	{
		contents.append(line);
		contents.append(" ");
	}

	int offset = 0;

	while (offset != contents.find_last_of(" "))
	{
		offset = contents.find_first_of(" ");
		std::string word = contents.substr(0, offset);
	}
}
