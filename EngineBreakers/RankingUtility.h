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
	AlphaA = 0,
	AlphaB = 1,
	AlphaC = 2,
	AlphaD = 3,
	AlphaE = 4,
	AlphaF = 5,
	AlphaG = 6,
	AlphaH = 7,
	AlphaI = 8,
	AlphaJ = 9,
	AlphaK = 10,
	AlphaL = 11,
	AlphaM = 12,
	AlphaN = 13,
	AlphaO = 14,
	AlphaP = 15,
	AlphaQ = 16,
	AlphaR = 17,
	AlphaS = 18,
	AlphaT = 19,
	AlphaU = 20,
	AlphaV = 21,
	AlphaW = 22,
	AlphaX = 23,
	AlphaY = 24,
	AlphaZ = 25,

	Num0 = 26,
	Num1 = 27,
	Num2 = 28,
	Num3 = 29,
	Num4 = 30,
	Num5 = 31,
	Num6 = 32,
	Num7 = 33,
	Num8 = 34,
	Num9 = 35,


	ST = 36,
	Slash = 37,
	ND = 38,
	RD = 39,
	TH = 40,

	PageNum0 = 41,
	PageNum1 = 42,
	PageNum2 = 43,
	PageNum3 = 44,
	PageNum4 = 45,
	PageNum5 = 46,
	PageNum6 = 47,
	PageNum7 = 48,
	PageNum8 = 49,
	PageNum9 = 50,

	LeftArrow = 51,
	RightArrow = 52,

	Blank = 53
};

enum class CharacterSpriteIndexOtherNumbers
{
	SmallNum0 = 36,
	SmallNum1 = 37,
	SmallNum2 = 38,
	SmallNum3 = 39,
	SmallNum4 = 40,
	SmallNum5 = 41,
	SmallNum6 = 42,
	SmallNum7 = 43,
	SmallNum8 = 44,
	SmallNum9 = 45,

	SmallBoldNum0 = 46,
	SmallBoldNum1 = 47,
	SmallBoldNum2 = 48,
	SmallBoldNum3 = 49,
	SmallBoldNum4 = 50,
	SmallBoldNum5 = 51,
	SmallBoldNum6 = 52,
	SmallBoldNum7 = 53,
	SmallBoldNum8 = 54,
	SmallBoldNum9 = 55,

	Colon = 56
};

enum class ScoreSpriteIndex
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

	Colon = 10
};

inline char ConvertIndexToLetter(CharacterSpriteIndex index)
{
	if (index == CharacterSpriteIndex::Blank)
	{
		return ' ';
	}

	static const std::array<char, 36> table = {
		'A','B','C','D','E','F','G','H','I','J',
		'K','L','M','N','O','P','Q','R','S','T',
		'U','V','W','X','Y','Z',
		'0','1','2','3','4','5','6','7','8','9'
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
	if (number == 0) // 0 贸府
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

inline std::list<ScoreSpriteIndex> ConvertScoreNumbers(int number)
{
	static const std::array<ScoreSpriteIndex, 10> table = {
		ScoreSpriteIndex::Num0, ScoreSpriteIndex::Num1,
		ScoreSpriteIndex::Num2, ScoreSpriteIndex::Num3,
		ScoreSpriteIndex::Num4, ScoreSpriteIndex::Num5,
		ScoreSpriteIndex::Num6, ScoreSpriteIndex::Num7,
		ScoreSpriteIndex::Num8, ScoreSpriteIndex::Num9
	};

	std::list<ScoreSpriteIndex> indexList;
	if (number == 0) // 0 贸府
	{
		indexList.push_back(ScoreSpriteIndex::Num0);
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

inline std::list<CharacterSpriteIndexOtherNumbers> ConvertNumbersSmall(int number)
{
	static const std::array<CharacterSpriteIndexOtherNumbers, 10> table = {
		CharacterSpriteIndexOtherNumbers::SmallNum0, CharacterSpriteIndexOtherNumbers::SmallNum1,
		CharacterSpriteIndexOtherNumbers::SmallNum2, CharacterSpriteIndexOtherNumbers::SmallNum3,
		CharacterSpriteIndexOtherNumbers::SmallNum4, CharacterSpriteIndexOtherNumbers::SmallNum5,
		CharacterSpriteIndexOtherNumbers::SmallNum6, CharacterSpriteIndexOtherNumbers::SmallNum7,
		CharacterSpriteIndexOtherNumbers::SmallNum8, CharacterSpriteIndexOtherNumbers::SmallNum9
	};

	std::list<CharacterSpriteIndexOtherNumbers> indexList;
	if (number == 0) // 0 贸府
	{
		indexList.push_back(CharacterSpriteIndexOtherNumbers::SmallNum0);
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

inline std::list<CharacterSpriteIndexOtherNumbers> ConvertNumbersSmallBold(int number)
{
	static const std::array<CharacterSpriteIndexOtherNumbers, 10> table = {
		CharacterSpriteIndexOtherNumbers::SmallBoldNum0, CharacterSpriteIndexOtherNumbers::SmallBoldNum1,
		CharacterSpriteIndexOtherNumbers::SmallBoldNum2, CharacterSpriteIndexOtherNumbers::SmallBoldNum3,
		CharacterSpriteIndexOtherNumbers::SmallBoldNum4, CharacterSpriteIndexOtherNumbers::SmallBoldNum5,
		CharacterSpriteIndexOtherNumbers::SmallBoldNum6, CharacterSpriteIndexOtherNumbers::SmallBoldNum7,
		CharacterSpriteIndexOtherNumbers::SmallBoldNum8, CharacterSpriteIndexOtherNumbers::SmallBoldNum9
	};

	std::list<CharacterSpriteIndexOtherNumbers> indexList;
	if (number == 0) // 0 贸府
	{
		indexList.push_back(CharacterSpriteIndexOtherNumbers::SmallBoldNum0);
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

inline std::list<CharacterSpriteIndex> ConvertPageNumbersLeftToRight(int number)
{
	static const std::array<CharacterSpriteIndex, 10> table = {
		CharacterSpriteIndex::PageNum0, CharacterSpriteIndex::PageNum1,
		CharacterSpriteIndex::PageNum2, CharacterSpriteIndex::PageNum3,
		CharacterSpriteIndex::PageNum4, CharacterSpriteIndex::PageNum5,
		CharacterSpriteIndex::PageNum6, CharacterSpriteIndex::PageNum7,
		CharacterSpriteIndex::PageNum8, CharacterSpriteIndex::PageNum9
	};

	std::list<CharacterSpriteIndex> indexList;
	if (number == 0) // 0 贸府
	{
		indexList.push_back(CharacterSpriteIndex::PageNum0);
		return indexList;
	}

	while (number > 0)
	{
		int digit = number % 10;
		indexList.push_front(table[digit]);
		number /= 10;
	}

	return indexList;
}

inline std::list<CharacterSpriteIndex> ConvertPageNumbersRightToLeft(int number)
{
	static const std::array<CharacterSpriteIndex, 10> table = {
		CharacterSpriteIndex::PageNum0, CharacterSpriteIndex::PageNum1,
		CharacterSpriteIndex::PageNum2, CharacterSpriteIndex::PageNum3,
		CharacterSpriteIndex::PageNum4, CharacterSpriteIndex::PageNum5,
		CharacterSpriteIndex::PageNum6, CharacterSpriteIndex::PageNum7,
		CharacterSpriteIndex::PageNum8, CharacterSpriteIndex::PageNum9
	};

	std::list<CharacterSpriteIndex> indexList;
	if (number == 0) // 0 贸府
	{
		indexList.push_back(CharacterSpriteIndex::PageNum0);
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
		return static_cast<CharacterSpriteIndex>(26 + (letter - '0'));
	}

	if (letter >= 'A' && letter <= 'Z')
	{
		return static_cast<CharacterSpriteIndex>(letter - 'A');
	}

	if (letter == ' ')
	{
		return CharacterSpriteIndex::Blank;
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