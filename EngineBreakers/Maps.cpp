#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/GameObject.h"
#include "../D2DEngineLib/D2DRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/PlayerInputSystem.h"
#include "../D2DEngineLib/Input.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/json.hpp"
#include "../D2DEngineLib/Screen.h"
#include "../D2DEngineLib/LineSegment.h"
#include "../D2DEngineLib/LineCollider.h"
#include "../D2DEngineLib/ResourceManager.h"
#include <fstream>
#include <iostream>
#include "Maps.h"

using json = nlohmann::json;

void Maps::Initialize()
{
}

void Maps::Start()
{
	CreateLineCollider(1);
	CreateLineCollider(2);
	CreateLineCollider(3);
	CreateLineCollider(4);
}

void Maps::FixedUpdate()
{
}

void Maps::Update()
{
	float width = Screen::Get().GetWidth() * 0.5f;
	float height = Screen::Get().GetHeight() * 0.5f;

	float pointX = (float)Input::GetCursorPoint().x - width;
	float pointY = (float)Input::GetCursorPoint().y - height;
	Vector2 clickPoint{ pointX, pointY };
	Vector2 startPos = { 0,0 };
	Vector2 halfpoint;

	if (m_ClickPositions.size() >= 1)
	{
		Matrix3x2 renderMatrix = Matrix3x2::Scale(1.0f, -1.0f);
		Matrix3x2 viewMatrix = Camera::s_mainCamera->GetViewMatrix().Inverse();
		Matrix3x2 finalMatrix = renderMatrix * viewMatrix;
		clickPoint = finalMatrix.TransformPoint(clickPoint);

		startPos = m_ClickPositions.back();
		Vector2 halfvec = (clickPoint - startPos) / 2;
		halfpoint = startPos + halfvec;
		Line->GetTransform()->SetLocalPosition(halfpoint);

		if(startPos!=clickPoint)
		{linecollider->SetLine(startPos-halfpoint, clickPoint-halfpoint);}
	}
}

void Maps::LateUpdate()
{
}

void Maps::OnDestroy()
{
}

void Maps::GetMouseCursorPosition()
{
	float width = Screen::Get().GetWidth() * 0.5f;
	float height = Screen::Get().GetHeight() * 0.5f;

	float pointX = (float)Input::GetCursorPoint().x - width;
	float pointY = (float)Input::GetCursorPoint().y - height;

	Vector2 clickPoint{ pointX, pointY };

	Matrix3x2 renderMatrix = Matrix3x2::Scale(1.0f, -1.0f);
	Matrix3x2 viewMatrix = Camera::s_mainCamera->GetViewMatrix().Inverse();
	Matrix3x2 finalMatrix = renderMatrix * viewMatrix;

	clickPoint = finalMatrix.TransformPoint(clickPoint);

	m_ClickPositions.push_back(clickPoint);

	std::cout << "x : " << clickPoint.x << ", y : " << clickPoint.y << std::endl;
}

std::string Maps::ConvertPositiontoJSON()
{
	json jarr = json::array();
	for (int i = 0; i < m_ClickPositions.size(); ++i)
	{
		if (i + 1 < m_ClickPositions.size())
		{
			Vector2 vecdistance = (m_ClickPositions[i + 1] - m_ClickPositions[i]) / 2;
			Vector2 centerPos = m_ClickPositions[i] + vecdistance;
			Vector2 relativeStart = -vecdistance;
			Vector2 relativeEnd = vecdistance;

			json j;
			j["center"] = { {"x",centerPos.x},{"y", centerPos.y} };
			j["start"] = { {"x", relativeStart.x} ,{"y", relativeStart.y} };
			j["end"] = { {"x", relativeEnd.x}, {"y", relativeEnd.y} };
			jarr.push_back(j);
		}
	}
	json j;
	j["lines"] = jarr;
	std::string jsonstring = j.dump(4);
	return jsonstring;
}

void Maps::ExportJsontoPath(std::string filepath)
{
	std::ofstream outfile(filepath);
	if (outfile.is_open())
	{
		std::string str = ConvertPositiontoJSON();
		outfile << str;
		outfile.close();
	}
}

void Maps::CreateLineCollider()
{
	//std::ifstream infile("Resource/Map/points.json");
	std::ifstream infile(ResourceManager::Get().GetResourcePath()+L"Map/points.json");
	if (infile.is_open())
	{
		json j;
		infile >> j;
		infile.close();

		//json파일의 라인의 개수만큼 크기 조정
		for (auto& line : j["lines"])
		{
			GameObject* gameobject = CreateGameObject(L"LineCollider");

			Vector2 position;
			position.x = line["center"]["x"];
			position.y = line["center"]["y"];
			gameobject->GetTransform()->SetLocalPosition(position);

			LineCollider* linecollider = gameobject->AddComponent<LineCollider>();
			linecollider->SetLayer(CollisionLayer::Wall);
			Vector2 startPoint;
			startPoint.x = line["start"]["x"];
			startPoint.y = line["start"]["y"];

			Vector2 endPoint;
			endPoint.x = line["end"]["x"];
			endPoint.y = line["end"]["y"];
			linecollider->SetLine(startPoint, endPoint);

			std::cout << "x : " << gameobject->GetTransform()->GetLocalPosition().x << ", y :" << gameobject->GetTransform()->GetLocalPosition().y << std::endl;
		}
	}
}

void Maps::CreateLineCollider(int index)
{
	std::wstring jsonfilepath{ ResourceManager::Get().GetResourcePath() + L"Map/points0" + std::to_wstring(index) + L".json" };
	//std::string jsonfilepath{ "Resource/Map/points0" + std::to_wstring(index) + L".json" };
	std::ifstream infile(jsonfilepath);
	if (infile.is_open())
	{
		json j;
		infile >> j;
		infile.close();

		//json파일의 라인의 개수만큼 크기 조정
		for (auto& line : j["lines"])
		{
			GameObject* gameobject = CreateGameObject(L"LineCollider");

			Vector2 position;
			position.x = line["center"]["x"];
			position.y = line["center"]["y"];
			gameobject->GetTransform()->SetLocalPosition(position);

			LineCollider* linecollider = gameobject->AddComponent<LineCollider>();
			linecollider->SetLayer(CollisionLayer::Wall);
			Vector2 startPoint;
			startPoint.x = line["start"]["x"];
			startPoint.y = line["start"]["y"];

			Vector2 endPoint;
			endPoint.x = line["end"]["x"];
			endPoint.y = line["end"]["y"];
			linecollider->SetLine(startPoint, endPoint);

			std::cout << "x : " << gameobject->GetTransform()->GetLocalPosition().x << ", y :" << gameobject->GetTransform()->GetLocalPosition().y << std::endl;
		}
	}
}

void Maps::ExportObjectPositionstoPath(std::string filepath)
{

	json jarr = json::array();
	for (auto& structure : m_Structrues)
	{
		json j;
		j["type"] = structure.type; 
		j["position"] = { {"x", structure.gameObject->GetTransform()->GetLocalPosition().x}, {"y", structure.gameObject->GetTransform()->GetLocalPosition().y} };
		j["id"] = structure.id;
		
		jarr.push_back(j);
	}
	
	json j;
	j["Buildings"] = jarr;
	std::string file = j.dump(4);

	std::ofstream outfile(filepath);
	outfile << file;
	outfile.close();
}

void Maps::InsertStructure(int type, GameObject* gameObject, int id)
{
	StructureData sd;
	sd.type = type;
	sd.gameObject = gameObject;
	sd.id = id;
	m_Structrues.push_back(sd);
}

std::vector<ImportStructData> Maps::ImportStructure()
{
	std::vector<ImportStructData> ImportedData;
	std::ifstream infile(ResourceManager::Get().GetResourcePath() + L"Map/Structure_Positions.json");
	if (infile.is_open())
	{
		json j;
		infile >> j;
		infile.close();

		for (auto& structure : j["Buildings"])
		{
			ImportStructData isd;
			isd.type = structure["type"];
			isd.position = Vector2 {structure["position"]["x"], structure["position"]["y"]};
			isd.id = structure["id"];

			ImportedData.push_back(isd);
		}
	}
	return ImportedData;
}

void Maps::PushBackEnemyPosition(Vector2 enemypos)
{
	m_EnmeyPositions.push_back(enemypos);
}

void Maps::ExportEnemyPositions(std::string filepath)
{
	json jarr = json::array();
	for (auto& pos : m_EnmeyPositions)
	{
		json j;
		j["position"] = { {"x", pos.x},{"y", pos.y} };
		
		jarr.push_back(j);
	}
	json j;
	j["normalEnemy"] = jarr;
	std::string file = j.dump(4);
	std::ofstream outfile(filepath);
	outfile << file;
	outfile.close();
}
