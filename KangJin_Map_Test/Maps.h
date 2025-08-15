#pragma once
#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/json.hpp"
class GameObject;
class LineCollider;
struct LineData
{
	Vector2 vecdistance;
	Vector2 centerPos;
	Vector2 relativeStart;
	Vector2 relativeEnd;
};

struct StructureData
{
	int type;
	GameObject* gameObject;
	int id;
};

struct ImportStructData
{
	int type;
	Vector2 position;
	int id;
};

class Maps : public Script
{
	std::vector<GameObject*> m_GameObjects;
	std::vector<Vector2> m_ClickPositions;
	std::vector<LineData> m_lineDatas;
	std::vector<StructureData> m_Structrues;
	std::vector<Vector2> m_enemeyPositions;
	std::vector<Vector2> m_eliteEnemyPositions;
	Vector2 m_bossPosition;
	std::vector<GameObject*> m_Lines;
	GameObject* Line;
	LineCollider* linecollider;
	int saveIndex = 1;
public:
	void Initialize() override;
	void Start()override;
	void FixedUpdate()override;
	void Update()override;
	void LateUpdate()override;
	void OnDestroy()override;

	void GetMouseCursorPosition();
	std::string ConvertPositiontoJSON();
	void ExportJsontoPath(std::string filepath);
	void CreateLineCollider();
	void CreateLineCollider(int index);
	void ExportObjectPositionstoPath(std::string filepath);
	void InsertStructure(int type, GameObject* position, int id);
	std::vector<ImportStructData> ImportStructure();
	void PushBackNormalEnemyPosition(Vector2 enemypos);
	void PushBackEliteEnemyPosition(Vector2 eliteEnemyPos);
	void SetBossPosition(Vector2 bossPos);
	void ExportEnemyPositions(std::string filepath);
	void SaveCursorPositionAsNormalEnemyPosition();
	void SaveCursorPositionAsEliteEnemyPosition();
	void SaveCursorPositionAsBossPosition();
};

