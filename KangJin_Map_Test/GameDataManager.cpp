#include "../D2DEngineLib/framework.h"
#include "GameDataManager.h"

GameDataManager& GameDataManager::Get()
{
	static GameDataManager instance;
	return instance;
}
