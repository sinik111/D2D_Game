#pragma once

#include <list>
#include <string>
#include <array>

class GameObject;

struct RankingData
{
	int score;
	std::string name;
};

struct RankingRow
{
	GameObject* rank;
	GameObject* score;
	GameObject* name;
};

enum class CharacterSpriteIndex
{
	Num0 = 0,
	Num1 = 1,
	Num2 = 2,
	Num3 = 3,
	Num4 = 4,
	Num5 = 5,
	Num6 = 6,
	Num7 = 7,
	Num8 = 8,
	Num9 = 9,
	AlphaA = 10,
	AlphaB = 11,
	AlphaC = 12,
	AlphaD = 13,
	AlphaE = 14,
	AlphaF = 15,
	AlphaG = 16,
	AlphaH = 17,
	AlphaI = 18,
	AlphaJ = 19,
	AlphaK = 20,
	AlphaL = 21,
	AlphaM = 22,
	AlphaN = 23,
	AlphaO = 24,
	AlphaP = 25,
	AlphaQ = 26,
	AlphaR = 27,
	AlphaS = 28,
	AlphaT = 29,
	AlphaU = 30,
	AlphaV = 31,
	AlphaW = 32,
	AlphaX = 33,
	AlphaY = 34,
	AlphaZ = 35,
	ST = 36,
	ND = 37,
	RD = 38,
	TH = 39
};

inline char ConvertIndexToLetter(CharacterSpriteIndex index)
{
	static const std::array<char, 36> table = {
		'0','1','2','3','4','5','6','7','8','9',
		'A','B','C','D','E','F','G','H','I','J',
		'K','L','M','N','O','P','Q','R','S','T',
		'U','V','W','X','Y','Z'
	};

	return table[static_cast<size_t>(index)];
}

inline std::list<CharacterSpriteIndex> ConvertNumbers(int number)
{
	static const std::array<CharacterSpriteIndex, 10> table = {
		CharacterSpriteIndex::Num0, CharacterSpriteIndex::Num1,
		CharacterSpriteIndex::Num2, CharacterSpriteIndex::Num3,
		CharacterSpriteIndex::Num4, CharacterSpriteIndex::Num5,
		CharacterSpriteIndex::Num6, CharacterSpriteIndex::Num7,
		CharacterSpriteIndex::Num8, CharacterSpriteIndex::Num9
	};

	std::list<CharacterSpriteIndex> indexList;
	if (number == 0) // 0 Ã³¸®
	{
		indexList.push_back(CharacterSpriteIndex::Num0);
		return indexList;
	}

	while (number > 0)
	{
		int digit = number % 10;
		indexList.push_back(table[digit]);
		number /= 10;
	}

	return indexList;
}

inline CharacterSpriteIndex ConvertLetterToIndex(char letter)
{
	if (letter >= '0' && letter <= '9')
	{
		return static_cast<CharacterSpriteIndex>(letter - '0');
	}

	if (letter >= 'A' && letter <= 'Z')
	{
		return static_cast<CharacterSpriteIndex>(10 + (letter - 'A'));
	}

	return CharacterSpriteIndex::Num0;
}

inline std::list<CharacterSpriteIndex> ConvertString(const std::string& str)
{
	std::list<CharacterSpriteIndex> indexList;

	for (char ch : str)
	{
		indexList.push_back(ConvertLetterToIndex(ch));
	}
	return indexList;
}