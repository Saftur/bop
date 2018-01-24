#pragma once

class LevelManager
{
public:
	static void Load(const char* levelName);
	static LevelManager& GetInstance();

	~LevelManager();
private:
	LevelManager();

	template <class D1 = std::nullptr_t, class D2 = std::nullptr_t, class D3 = std::nullptr_t, class D4 = std::nullptr_t, class D5 = std::nullptr_t, class D6 = std::nullptr_t, class D7 = std::nullptr_t>
	class Object
	{
	public:
		D1 d1;
		D2 d2;
		D3 d3;
		D4 d4;
		D5 d5;
		D6 d6;
		D7 d7;
	};
	
};
