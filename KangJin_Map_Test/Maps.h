#pragma once
#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/json.hpp"

struct LineData
{
	Vector2 vecdistance;
	Vector2 centerPos;
	Vector2 relativeStart;
	Vector2 relativeEnd;
};

class Maps : public Script
{
	std::vector<GameObject*> m_GameObjects;
	std::vector<Vector2> m_Positions;
	std::vector<LineData> m_lineDatas;

public:
	void Initialize() override;
	void Start()override;
	void FixedUpdate()override;
	void Update()override;
	void LateUpdate()override;
	void OnDestroy()override;

	std::string ConvertPositiontoJSON();
	void ExportJsontoPath(std::string filepath);
	void CreateLineCollider();
};

