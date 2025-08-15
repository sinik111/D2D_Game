#pragma once
#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/CircleCollider.h"
#include "../D2DEngineLib/GameObject.h"
#include "../D2DEngineLib/json.hpp"
class Structure : public Script
{
protected :
	std::string jsonfilepath;
	bool isTransparent = false;
	std::vector<Vector2> m_colliderPoints;
	float scale = 1.0f;
	int index = 1;
public :

	//Todo : jsonfile path만 바꾸는 로직 넣어서 하나로 통일하기
	Structure() = default;
	Structure(std::string filepath) :jsonfilepath(filepath){}

	std::vector<CircleCollider*> m_Colliders;

	void Start() override;
	void Update() override;
	void FixedUpdate() override;
	
	void SetSize(int size) { index = size ; }
	const int& GetSize() { return index; }
	
	void OnTriggerStay(const Collision& collision) override;

	std::string GetJsonFilePath() { return jsonfilepath; }
	void SetJsonFilePath(std::string filepath) { jsonfilepath = filepath; }
	void ImportfromJson();
	virtual void ExporttoJson();
};
