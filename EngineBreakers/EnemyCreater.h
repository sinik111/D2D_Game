#pragma once
#include "../D2DEngineLib/Script.h"
class GameObject;
class EnemyCreater : public Script
{
	GameObject* player = nullptr;
	GameObject* camera = nullptr;
	void Start() override;
public :
	void ImportEnemyPositionFromJSON();
	void SetPlayer(GameObject* object);
	void SetCamera(GameObject* object);
};

